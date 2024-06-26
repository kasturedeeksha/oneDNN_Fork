/*******************************************************************************
* Copyright 2023-2024 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "ref_partition.hpp"
#include "cpu/platform.hpp"
#include "dnnl_common.hpp"
#include "utils/compare.hpp"

namespace graph {

ref_partition_t::ref_partition_t(const deserialized_graph &dg,
        const dnnl::graph::partition &par,
        const std::vector<dnnl::graph::logical_tensor> &ins,
        const std::vector<dnnl::graph::logical_tensor> &outs)
    : dg_(&dg), data_displacer(dg, par) {
    const auto &op_ids = par.get_ops();
    const std::unordered_set<size_t> op_ids_set(op_ids.begin(), op_ids.end());

    // dg.ops_ needs make sure its Topo order to first idx, first executed.
    for (const auto &aop : dg.ops_) {
        if (op_ids_set.find(aop.id_) == op_ids_set.end()) continue;

        auto aop_ref = std::ref(aop);
        partition_ops_ref_.emplace_back(aop_ref);
        for (const auto &in_lt : aop.in_lts_) {
            in_lt_2_ops_[in_lt.id_].emplace_back(aop_ref);
            lt_id_2_lt_.emplace(in_lt.id_, in_lt);
        }
        for (const auto &out_lt : aop.out_lts_) {
            out_lt_2_op_.emplace(out_lt.id_, aop_ref);
            lt_id_2_lt_.emplace(out_lt.id_, out_lt);
        }
    }

    for (const auto &in : ins) {
        partition_in_ids_.emplace_back(in.get_id());
    }
    for (const auto &out : outs) {
        partition_out_ids_.emplace_back(out.get_id());
    }
};

int ref_partition_t::init_ref(const std::vector<size_t> &graph_in_ports,
        partition_mem_map_t &partition_mem_map, res_t *res) {
    for (const auto &par_op_ref : partition_ops_ref_) {
        // res should be independent from op to op
        res->state = UNTESTED;

        auto ref_prim = ::std::make_shared<ref_primitive_t>(par_op_ref.get());

        ref_prims_.emplace(par_op_ref.get().id_, ref_prim);
        SAFE(ref_prim->init_prb(res), WARN);

        SAFE_V(ref_prim->init_prim(::get_test_engine(), res));
        ref_prim->init_memory_args(::get_test_engine());
        SAFE_V(ref_prim->init_ref_memory_args(::get_test_engine(), res));

        // store the memory for each logical tensor
        // op `emplace` will keep the first memory it met for each id
        bool use_dst = ::graph::eltwise::get_flag_use_dst_for_bwd_compute(
                par_op_ref);
        for (size_t i = 0; i < par_op_ref.get().in_lts_.size(); i++) {
            const auto &lt = par_op_ref.get().in_lts_[i];
            int arg = get_prim_arg_name_from_graph_op_input_offset(
                    ref_prim->get_kind(), i, use_dst);
            lt_id_2_mems_.emplace(lt.id_, ref_prim->get_arg(arg));
        }
        for (size_t i = 0; i < par_op_ref.get().out_lts_.size(); i++) {
            const auto &lt = par_op_ref.get().out_lts_[i];
            int arg = get_prim_arg_name_from_graph_op_output_offset(
                    ref_prim->get_kind(), i);
            if (arg == 0) {
                fake_lt_ids_.insert(lt.id_);
            } else if (arg > 0) {
                lt_id_2_mems_.emplace(lt.id_, ref_prim->get_arg(arg));
            }
        }

        // Displace the data generated by the driver filling functions with
        // values supplied from the dg object. Otherwise, the values for
        // reference would diverge from the values passed to the Graph API.
        SAFE(ref_prim->displace_scales(), WARN);

        // Initialze the rest ops if current status is UNTESTED or EXECUTED
        // otherwise there is no need to init memory for the rest ops.
        if (res->state != UNTESTED && res->state != EXECUTED) {
            // But for perf mode, when the tensors in the current op is not
            // the graph in/out, continue, otherwise return.
            if (has_bench_mode_bit(mode_bit_t::perf)) {
                for (const auto &d_lt : par_op_ref.get().in_lts_) {
                    auto iter_find = std::find(graph_in_ports.begin(),
                            graph_in_ports.end(), d_lt.id_);
                    if (iter_find != graph_in_ports.end()) { return FAIL; }
                }
                // If all op ids are not graph inputs, the op failure doesn't
                // affect the perf mode.
                continue;
            } else {
                return FAIL;
            }
        }
    }

    // displace data if needed
    for (const auto &entry : lt_id_2_mems_) {
        SAFE_V(data_displacer.displace_input_data(
                entry.first, const_cast<dnn_mem_t &>(entry.second), res));
    }

    // init graph input/oputput memory from lt_id_2_mems_
    for (const auto &id : partition_in_ids_) {
        if (lt_id_2_mems_.find(id) == lt_id_2_mems_.end()) {
            BENCHDNN_PRINT(0, "Fail: cannot find memory for %zu\n", id);
            res->state = FAILED;
            return FAIL;
        }
        partition_mem_map.emplace(id,
                dnn_graph_mem_t(
                        lt_id_2_mems_.at(id), lt_id_2_lt_.at(id), true));
    }
    for (const auto &id : partition_out_ids_) {
        if (fake_lt_ids_.find(id) != fake_lt_ids_.end()) {
            partition_mem_map.emplace(
                    id, dnn_graph_mem_t({}, lt_id_2_lt_.at(id), false, true));
        } else if (lt_id_2_mems_.find(id) == lt_id_2_mems_.end()) {
            BENCHDNN_PRINT(0, "Fail: cannot find memory for %zu\n", id);
            res->state = FAILED;
            return FAIL;
        } else
            partition_mem_map.emplace(id,
                    dnn_graph_mem_t(
                            lt_id_2_mems_.at(id), lt_id_2_lt_.at(id), false));
    }

    return OK;
}

void ref_partition_t::exec_ops(res_t *res) {
    for (const auto &par_op_ref : partition_ops_ref_) {
        const auto &op = par_op_ref.get();
        auto ref_prim = ref_prims_.at(op.id_);

        // link args && replace the memory before execution
        bool use_dst = ::graph::eltwise::get_flag_use_dst_for_bwd_compute(
                par_op_ref);
        for (size_t i = 0; i < op.in_lts_.size(); i++) {
            const auto &lt = op.in_lts_[i];
            int arg = get_prim_arg_name_from_graph_op_input_offset(
                    ref_prim->get_kind(), i, use_dst);
            ref_prim->replace_arg(arg, lt_id_2_mems_.at(lt.id_));
        }
        for (size_t i = 0; i < op.out_lts_.size(); i++) {
            const auto &lt = op.out_lts_[i];
            // skip replace for fake output tensor
            if (fake_lt_ids_.find(lt.id_) != fake_lt_ids_.end()) continue;
            int arg = get_prim_arg_name_from_graph_op_output_offset(
                    ref_prim->get_kind(), i);
            ref_prim->replace_arg(arg, lt_id_2_mems_.at(lt.id_));
        }

        // There are unfusable operations (such as Softmax) inside a partition
        // that are executed with user-requested data type. To have correctness
        // validation working as expected, the data for such operations should
        // be adjusted accordingly in case of low precision data types. E.g.,
        // if pattern is bfloat16 only, the output of a matmul op is bfloat16.
        // Having a float reference implies that is should use "same" bfloat16
        // data, otherwise, the output from bfloat16 softmax inside the graph
        // and float softmax inside the reference will mismatch, which happens
        // due to the property of softmax, and exponent part in particular.
        const bool unfusable_transcendental_op
                = ref_prim->get_kind() == dnnl::graph::op::kind::SoftMax;

        // However, this practice must be limited to the cases when it's
        // mandatory. The requirement for input adjustment is having a parent
        // op, since there's an assumption the current op is unfusable.
        //
        // Note: Compiler backend doesn't down-convert to the lower precision
        // data type when pass data to a transcendental op. However, this
        // conversion can't be disabled without additional library API which
        // would provide an info if Compiler or DNNL backend was used, since
        // fall back to DNNL from Compiler backend fails.
        if (unfusable_transcendental_op && has_parent_op(op)) {
            for (size_t i = 0; i < op.in_lts_.size(); i++) {
                const auto dt = ref_prim->get_lt_dt(op.in_lts_[i].id_);
                // There's no need to reorder data for f32 tensors.
                if (dt == dnnl_f32 || dt == dnnl_data_type_undef) continue;

                int arg = get_prim_arg_name_from_graph_op_input_offset(
                        ref_prim->get_kind(), i, use_dst);
                dnn_mem_t &src_i
                        = const_cast<dnn_mem_t &>(ref_prim->get_arg(arg));
                dnn_mem_t src_low_dt(src_i, dt, tag::abx, src_i.engine());
                SAFE_V(src_i.reorder(src_low_dt));
            }
        }

        ref_prim->execute_prim(res);

        // For an output, because of various graph compositions, there's a more
        // detailed guide when data adjustment should happen. It's covered by
        // `need_unfusable_output_crop` function.
        //
        // A data type to where transform the data will also be provided by the
        // same function since there are corner cases.
        dnnl_data_type_t dt = dnnl_data_type_undef;
        if (unfusable_transcendental_op && need_unfusable_output_crop(op, dt)) {
            for (size_t i = 0; i < op.out_lts_.size(); i++) {
                // There's no need to reorder data for undefined or f32 tensors.
                if (dt == dnnl_data_type_undef || dt == dnnl_f32) continue;

                int arg = get_prim_arg_name_from_graph_op_output_offset(
                        ref_prim->get_kind(), i);
                dnn_mem_t &dst_i
                        = const_cast<dnn_mem_t &>(ref_prim->get_arg(arg));
                dnn_mem_t dst_low_dt(dst_i, dt, tag::abx, dst_i.engine());
                SAFE_V(dst_i.reorder(dst_low_dt));
            }
        }
    }
}

int ref_partition_t::check_partition_correctness(
        partition_mem_map_t &partition_mem_map, res_t *res) {

    bool mistrusted = false, has_eltwise = false, output_has_nans = false;
    const auto &map_kind_to_alg = eltwise::get_eltwise_kind_map();

    for (const auto &op : partition_ops_ref_) {
        size_t op_id = op.get().id_;
        const auto op_kind = op.get().kind_;
        const auto ref_prim = ref_prims_.at(op_id);

        // if there is eltwise post-ops or binary div post-ops (GPU test), need
        // to relax compare critria.
        // Currently, both cases use set_has_eltwise_post_op flag in benchdnn
        // compare function.
        // The flag name is not very accurate, add this note to avoid confusion
        const auto op_driver = opkind2driver(ref_prim->get_kind());
        has_eltwise = has_eltwise
                || ((op_driver == dnnl_driver_t::eltwise
                        || (opstr2kind(op_kind) == dnnl::graph::op::kind::Divide
                                && engine_tgt_kind == dnnl_gpu)));
        output_has_nans = output_has_nans
                || ((map_kind_to_alg.find(op_kind) != map_kind_to_alg.end())
                        && ::eltwise::eltwise_alg_returns_nan_or_inf(
                                map_kind_to_alg.at(op_kind)))
                // `f8_e4m3` range is very short which makes inputs convert
                // into NaNs.
                || (op_driver == dnnl_driver_t::reorder
                        && op.get().in_lts_.front().get_data_type()
                                == logical_tensor::data_type::f8_e4m3);

        // get the args that need comparing
        args_t output_args;
        for (size_t out_idx = 0; out_idx < op.get().out_lts_.size();
                ++out_idx) {
            int out_arg = get_prim_arg_name_from_graph_op_output_offset(
                    opstr2kind(op_kind), out_idx);
            if (out_arg == 0) continue; // unsupported case

            size_t out_lt_id = op.get().out_lts_[out_idx].id_;
            for (size_t i = 0; i < partition_out_ids_.size(); i++) {
                if (out_lt_id != partition_out_ids_[i]) continue;

                auto &graph_mem = partition_mem_map.at(out_lt_id);
                const auto &par_out_mem = graph_mem.get_mem();
                output_args.set(out_arg, par_out_mem);
                break;
            }
        }
        if (output_args.size() == 0) continue;

        // reset the state
        res->state = EXECUTED;

        ref_prim->check_correctness(
                output_args, has_eltwise, output_has_nans, res);
        if (res->state == FAILED) {
            BENCHDNN_PRINT(
                    2, "Op failed: {(%zu) %s}\n", op_id, op_kind.c_str());
            return FAIL;
        }

        mistrusted = mistrusted || (res->state == MISTRUSTED);
    }
    if (res->errors > 0) {
        res->state = FAILED;
    } else if (mistrusted) {
        res->state = MISTRUSTED;
    } else {
        res->state = PASSED;
    }

    return OK;
}

bool ref_partition_t::has_parent_op(const deserialized_op &op) const {
    if (partition_ops_ref_.size() < 2) return false;

    for (const auto &in_lt : op.in_lts_) {
        // Check if parent op exist for an `op`.
        const auto &parent_op = dg_->get_op_by_out_lt(in_lt.id_);
        if (parent_op.empty()) continue;

        // If it does, check its ID presents in a partition.
        for (const auto &op_ref : partition_ops_ref_) {
            const auto &cur_op = op_ref.get();
            if (parent_op.id_ == cur_op.id_) return true;
        }
    }

    return false;
}

bool ref_partition_t::has_child_op(
        const deserialized_op &op, const deserialized_op **child_op_ptr) const {
    if (partition_ops_ref_.size() < 2) return false;

    for (const auto &out_lt : op.out_lts_) {
        // Check if child op exist for an `op`.
        const auto &child_op = dg_->get_op_by_in_lt(out_lt.id_);
        if (child_op.empty()) continue;

        // If it does, check its ID presents in a partition.
        for (const auto &op_ref : partition_ops_ref_) {
            const auto &cur_op = op_ref.get();
            if (child_op.id_ == cur_op.id_) {
                if (child_op_ptr) *child_op_ptr = &child_op;
                return true;
            }
        }
    }

    return false;
}

// This function decides when unfusable transcendental op output should be
// reordered to lower data type and back to f32 for a reference path.
bool ref_partition_t::need_unfusable_output_crop(
        const deserialized_op &op, dnnl_data_type_t &dt) const {
    const deserialized_op *child_op = nullptr;
    // First of all, the output should have a child op...
    if (!has_child_op(op, &child_op)) return false;
    // If the child op is not a TypeCast, it's safe to crop.
    if (child_op->kind_ != "TypeCast") {
        // Target dt in this case is the output dt of input `op`.
        dt = convert_dt(op.out_lts_[0].get_data_type());
        return true;
    }
    // When it is a TypeCast (it always changes `cur_dt` <-> f32, both ways are
    // possible), there are options:
    // * If it's the last one, no crop, as f32 will happen on the other end.
    const deserialized_op *next_child_op = nullptr;
    if (!has_child_op(*child_op, &next_child_op)) return false;
    // * If there's a child Quantize, no crop either, since output would
    //   perform a reorder with a proper scale value to match the other end.
    if (next_child_op->kind_ == "Quantize") return false;
    // * However, a second TypeCast would negate an effect of the previous...
    if (next_child_op->kind_ == "TypeCast") {
        // Target dt in this case is the output dt of the last TypeCast.
        dt = convert_dt(next_child_op->out_lts_[0].get_data_type());
        return true;
    }

    // Rest potential outcomes are default to make a crop. The target dt in
    // this case is the output dt of the child op.
    dt = convert_dt(child_op->out_lts_[0].get_data_type());
    return true;
}

} // namespace graph
