/*******************************************************************************
* Copyright 2024 Intel Corporation
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

namespace dnnl {
namespace impl {
namespace gpu {
namespace intel {
namespace jit {
namespace v2 {
namespace conv {

// clang-format off
const char** get_plan_registry_entries() {
    static const char *entries[] = {
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic8oc16ow2 tg=oc16 loop_desc=mb,ow,oh,od model=010000000400000000000000E594413978B0D0370A9B50377F3BCE3D",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64mb4 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=01000000040000000000000021AB13371D2CD739262C573BA91A0738",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow2 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000C2148B36ADBF1039B2BF903AC16DBC39",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic16iw4oc2 tg=ic8iw4 loop_desc=kw,kh,kd,oc load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sd==1:sh==1:sw==1 model=0100000004000000000000001E3C903C1F503B3DF526F53CA12B83BE",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic32oc16ow8 tg=oc16ow2 loop_desc=kw,kh,kd,ic model=010000000400000000000000D579783A4333073B524B843AE5C0ECBA",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32mb8oc2 tg=ic2 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=010000000400000000000000A18613B9754F023EA426073D842CAFBE",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32iw2oc8 tg=ic8iw2 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=010000000400000000000000022CB63C2DD96A3C8BC71F3DF5374EBD",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic16mb2oc32 tg=mb2 loop_desc=kw,kh,kd,oc load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304:sd==1:sh==1:sw==1 model=0100000004000000000000003AACD13CD5BDDB3D45EB003C56AD8FBD",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb8 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000BF769B3736CFDB3931CF5B3BD86B1039",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:u8 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=ow2 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=010000000400000000000000D689C3C0B916353C790ADB3A368AC340",
        "prop=fwd dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64mb2 tg=mb4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000FD783EC3A901AB3BF0F1173AFD783E43",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32oc16ow4 tg=oc4ow16 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sw==1 model=0100000004000000000000008E74923D13868B3ED749FC3D611C9CBE",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic2mb4oc64 tg=oc16 loop_desc=kw,kh,kd,ic model=0100000004000000000000000A0DC33CFE1B773D82E9AE3D3C9998BE",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic64mb8oc16 tg=oc8ow2 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d prefetch=x3 reqs=ic%64==0:ic>=64:id*ih*iw*g*ic<=16777216:mb<=16777216:oc%64==0:oc>=64:od*oh*ow*g*oc<=16777216 model=010000000400000000000000C268293C45FB303C89CF813C433EDCBC",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic64oc32ow16 tg=oc4ow4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d prefetch=x3 reqs=g*ic<=16777216:g*oc<=16777216:ic%64==0:ic>=64:iw<=16777216:oc%64==0:oc>=64:ow<=16777216:sw==1 model=010000000400000000000000458DE03CEFBFBB3DAAD446BCDED182BD",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic8mb2oc64 tg=oc4 loop_desc=mb,ow,oh,od load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=0100000004000000000000003AFC9C3A4B0CF8389D077839D54CF53C",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic32oc16ow8 tg=oc8ow2 loop_desc=kw,kh,kd,ic model=01000000040000000000000033CC3D3A508E123B5C4A573AB54EC2BA",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic32mb8oc16 tg=mb2 loop_desc=kw,kh,kd,ic model=010000000400000000000000DD8B033AE297473B010D4F395C83B0BA",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic16mb4oc16 tg=oc4ow16 loop_desc=kw,kh,kd,ic model=0100000004000000000000003E96A83CEBBC823D76EA3A3D370878BD",
        "prop=bwd_d src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic16mb8oc32 tg=iw16 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=0100000004000000000000009AF4B93A0288273B3551023BBF3A5CBB",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic4mb8oc64 tg=mb8oc8 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=010000000400000000000000B2E73B3DD637A33E6030943E602EBDBE",
        "prop=bwd_d src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic16iw8oc32 tg=ic8iw4 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=010000000400000000000000E4EBA43AD399393B5525CF3A076E2EBB",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic32mb32oc16 tg=mb4oc4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d prefetch=x3 reqs=ic%64==0:ic>=64:id*ih*iw*g*ic<=16777216:mb<=16777216:oc%64==0:oc>=64:od*oh*ow*g*oc<=16777216 model=0100000004000000000000002ED3ED3C45610F3D8F8F1B3DFD7003BE",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g16mb16 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=0100000004000000000000004145F737D22D5F39DA2DDF3AA90F063A",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic4oc64ow16 tg=ic8 loop_desc=mb,ow,oh,od load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sw==1 model=010000000400000000000000D29F063B866EBD3C816EBD3C0C63903E",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow2 tg=ow4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000AD139A438AAA853BC24352B8A8139AC3",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g16ow16 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000E5B08B372469E0392F69603BF40B4138",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:s32 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=ow4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=010000000400000000000000178D684370B9053C78AC7D3B068D68C3",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic8mb16oc16 tg=mb2oc8 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=010000000400000000000000B8D6053ED75DB03D923FB63E10A527BF",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow8 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000E0E244377F10AD397E102D3BC77E9938",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic4mb8oc64 tg=oc2ow8 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=0100000004000000000000002EE7813D5B07DA3E6F2A35BDF2BFCDBE",
        "prop=fwd dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow4 tg=ow2 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=01000000040000000000000074F08144ADD1E83B3EE0E73871F081C4",
        "prop=fwd dw=1 src=axb:s8 wei=xab:s8 dst=axb:s32 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=ow16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=01000000040000000000000027AA13C45FEC2C3CC7198B3A3DAA1344",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g16ow2 tg=ow16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000786B2AC32B0A5A3AA5EBBE3A8A6B2A43",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32mb2oc8 tg=ic16mb4 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=01000000040000000000000052A20D3CE53C6C3DCD915B3C298065BD",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic8oc32ow4 tg=ic2oc8 loop_desc=mb,ow,oh,od load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sw==1 model=0100000004000000000000004110EA395736023C8035823B1BFD2F3E",
        "prop=bwd_d src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic16mb8oc32 tg=x loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=01000000040000000000000034414039B217973BFFE1853A5C6B46BB",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:u8 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=ow16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=01000000040000000000000056E31D431DD2E63A661D2A3B2CE31DC3",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g16mb4 tg=ow8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=0100000004000000000000009FA2D9C1E0D1A03F7A69A0BFD2A2D941",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic8oc16ow4 tg=oc8 loop_desc=mb,ow,oh,od model=010000000400000000000000B6CD003A6D159C3A08159C3A1523A53D",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=ow16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000B65D26C30E2F0F3C660BCA39E55D2643",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow8 tg=ow8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000E77E7440CDF133BE735E3D3E3A7374C0",
        "prop=fwd dw=1 src=axb:s8 wei=xab:s8 dst=axb:s8 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=x loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=0100000004000000000000008C2B3DC305E1423CAC1A2739992B3D43",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic2mb64oc16 tg=oc4ow8 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=010000000400000000000000F1ED973DE453873E6F152D3EE4D0B6BE",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb4 tg=x loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=0100000004000000000000004AD3DCC1EE63463B5C9A2A394BD3DC41",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32mb4oc2 tg=ic8iw4 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=010000000400000000000000ADE3693CD4CB473D34491F3D8C0B3ABE",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic2mb8oc16 tg=oc16 loop_desc=kw,kh,kd,ic model=010000000400000000000000B665513C720D4E3DD8CB373D61C5D5BD",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic64oc16ow64 tg=oc4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d prefetch=x3 reqs=g*ic<=16777216:g*oc<=16777216:ic%64==0:ic>=64:iw<=16777216:oc%64==0:oc>=64:ow<=16777216:sw==1 model=0100000004000000000000004E32C03C6EB38A3D0B5B433DED7560BD",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32mb4oc8 tg=ic2iw16 loop_desc=kw,kh,kd,oc load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304:sd==1:sh==1:sw==1 model=0100000004000000000000001C3D333DA685033EFF337E3D390E17BE",
        "prop=fwd dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=ow16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=0100000004000000000000001FEE57431B1F003B864AF63A04EE57C3",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=ow8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000F5D118C2F038DB3EE69BD9BE1ED21842",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=0100000004000000000000009F5B9E36C65B083AE35B883B75128138",
        "prop=fwd dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow2 tg=ow4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000481DD5C3B4D8BA3BDB2B6E3A4C1DD543",
        "prop=fwd dw=1 src=axb:s8 wei=xab:s8 dst=axb:s8 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=mb16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=010000000400000000000000B0562FC3B599F23A03FD2A3BD7562F43",
        "prop=fwd dw=1 src=axb:s8 wei=xab:s8 dst=axb:s32 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=mb16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=01000000040000000000000015C889C2E5C7493A2DD26F3B6AC88942",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb4 tg=mb4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=0100000004000000000000007D8D0FC32DFE963B591A94397E8D0F43",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb2 tg=mb8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000BF0648C3BF48C6BF28CAC63FBC064843",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:u8 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=x loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=0100000004000000000000000A2032C453AE3D3CFF411D3A0A203244",
        "prop=fwd dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=mb4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000622486C397987E3BC8B80EB963248643",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow2 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000864FEE3608850039F884803ABA593439",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32 tg=mb8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=01000000040000000000000010FC19C286868B3D14D886BDFEFB1942",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=mb4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=01000000040000000000000089F26A436A5E683BFEB8A9B780F26AC3",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb2 tg=ow4 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000F4B32DC1DFD1643B9350723841B42D41",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=x loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000B62099C331F7573BD2C111BABF209943",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic2mb16oc32 tg=ow4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=010000000400000000000000CE1C5E3AADE0E63D667E423E6C7E53BF",
        "prop=fwd dw=1 src=axb:s8 wei=xab:s8 dst=axb:s32 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=mb2 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=0100000004000000000000007803A4C33A8C923BEF60EDB97F03A443",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic16mb4oc16 tg=oc2ow4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=0100000004000000000000001B6A1D3D1E99063E1799063E82BA68BE",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:u8 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow2 tg=ow8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=010000000400000000000000ACED0EC2261F993E9F1195BE19EE0E42",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic64mb64oc16 tg=oc4ow4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d prefetch=x3 reqs=ic%64==0:ic>=64:id*ih*iw*g*ic<=16777216:mb<=16777216:oc%64==0:oc>=64:od*oh*ow*g*oc<=16777216 model=010000000400000000000000F8F96E3DC1C0E63D5E97053D51930CBE",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic16mb4oc16 tg=oc8 loop_desc=kw,kh,kd,ic model=0100000004000000000000003126BA3CEBCF6B3DE4CF6B3DF798BFBD",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow2 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=0100000004000000000000008B1F8E36BF458139C345013B9A3F9839",
        "prop=fwd dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32 tg=ow2 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=0100000004000000000000003AA3DD43B948193B110BADB92FA3DDC3",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic64mb8oc64 tg=oc2ow4 loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d prefetch=x3 reqs=ic%64==0:ic>=64:id*ih*iw*g*ic<=16777216:mb<=16777216:oc%64==0:oc>=64:od*oh*ow*g*oc<=16777216 model=0100000004000000000000004ED6D43C5CCF2A3D5DCF2A3DDD6E6CBD",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic16iw8oc2 tg=ic4iw16 loop_desc=kw,kh,kd,oc load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sd==1:sh==1:sw==1 model=010000000400000000000000E73C893CA908FB3D80C42E3DDBA23BBE",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:s32 hw=xehpc fma=mad simd=16 regs=128 iter=g64mb2 tg=ow8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=010000000400000000000000A23999C279935F40AC245FC0D4399942",
        "prop=bwd_d src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic16iw8oc32 tg=ic4iw4 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=010000000400000000000000B60C8F3A733F213B828A043BA67D13BB",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000D83FB636729FC4386C9F443A7C7D0839",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic8mb4oc32 tg=ic8oc4 loop_desc=mb,ow,oh,od model=010000000400000000000000943F3E3A982C8738DA3D87374704223E",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g16 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000717151368CB9853764BA0539C503EE3A",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic4oc32ow8 tg=ic2oc16 loop_desc=mb,ow,oh,od model=01000000040000000000000036FB1F3B6B990EBB19990EBA87E66E3D",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g16mb8 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000F612E037061DB138E01C313AC235A03A",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g64mb2 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000FB86473677E387396FE3073BF2A4A03A",
        "prop=bwd_w dw=1 src=axb:f32 wei=xab:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb8 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%16==0:ic==1:oc==1 model=010000000400000000000000F5B0283796D19F3996D11F3B74A90538",
        "prop=fwd dw=1 src=axb:s8 wei=xab:s8 dst=axb:s8 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow2 tg=ow16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=010000000400000000000000AF99ECC2FC85993BB4245E3B279AEC42",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic2mb16oc16 tg=ic8 loop_desc=mb,ow,oh,od model=010000000400000000000000F17DE3393BDB2C39E9D92C3904FA403D",
        "prop=fwd dw=1 src=axb:u8 wei=xab:s8 dst=axb:s32 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow4 tg=ow2 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%64==0:ic==1:oc==1 model=0100000004000000000000009733C5C26FCD143CB24F233B8433C542",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic2oc16ow16 tg=ic16 loop_desc=mb,ow,oh,od load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sw==1 model=010000000400000000000000F75304B953C926B866D3C23A92032A3E",
        "prop=fwd src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic32mb8oc16 tg=mb2oc8 loop_desc=kw,kh,kd,ic model=010000000400000000000000A9595A3A3F43C93A90DDCD3A0D0CE9BA",
        "prop=bwd_d src=axb:s8 wei=axcb:s8 dst=axb:s8 hw=xehpc fma=dpas simd=16 regs=256 iter=ic16mb8oc32 tg=ic16 loop_desc=kw,kh,kd,oc reqs=sd==1:sh==1:sw==1 model=01000000040000000000000053FCFE3A1ED7173B0CDEF73AF2455ABB",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic4mb16oc32 tg=ic2oc2 loop_desc=mb,ow,oh,od load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304 model=0100000004000000000000004087E73ADEEDA339E7E2233A9B7CB73E",
        "prop=fwd dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g32 tg=mb16 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=0100000004000000000000008F9809C1817756398E670D3BFD990941",
        "prop=fwd dw=1 src=axb:f16 wei=xab:f16 dst=axb:f16 hw=xehpc fma=mad simd=16 regs=128 iter=g64ow2 tg=ow8 loop_desc=kw,kh,kd,ic load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000EFBF53C36815D74030E6D6C007C05343",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g32ow8 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000A59641379382F8399682783B66221D3A",
        "prop=fwd src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic4oc32ow2 tg=x loop_desc=kw,kh,kd,ic load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sw==1 model=010000000400000000000000B10C563BB586D23DA531DD3B434C85BE",
        "prop=bwd_w dw=1 src=axb:bf16 wei=xab:bf16 dst=axb:bf16 hw=xehpc fma=mad simd=16 regs=128 iter=g32mb2 tg=x loop_desc=mb,ow,oh,od load=a:block,b:block store=c:block reqs=g%32==0:ic==1:oc==1 model=010000000400000000000000A49C17379566B5389166353A0AFF6E39",
        "prop=bwd_d src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32mb8oc4 tg=ic16 loop_desc=kw,kh,kd,oc load=a:2d,b:2d store=c:2d reqs=ic%16==0:ic>=16:id*ih*iw*g*ic<=4194304:mb<=16777216:oc%16==0:oc>=16:od*oh*ow*g*oc<=4194304:sd==1:sh==1:sw==1 model=0100000004000000000000008F17A73D713A353E0332123EF0FE17BF",
        "prop=bwd_w src=axb:f32 wei=axcb:f32 dst=axb:f32 hw=xehpc fma=mad simd=16 regs=128 iter=ic32oc16ow4 tg=ic8oc2 loop_desc=mb,ow,oh,od load=a:2d,b:2d store=c:2d reqs=g*ic<=4194304:g*oc<=4194304:ic%16==0:ic>=16:iw<=16777216:oc%16==0:oc>=16:ow<=16777216:sw==1 model=01000000040000000000000004789CB97C068C3CA0060C3CB0B3EE3E",
        nullptr,
    };
    return entries;
}
// clang-format on

} // namespace conv
} // namespace v2
} // namespace jit
} // namespace intel
} // namespace gpu
} // namespace impl
} // namespace dnnl
