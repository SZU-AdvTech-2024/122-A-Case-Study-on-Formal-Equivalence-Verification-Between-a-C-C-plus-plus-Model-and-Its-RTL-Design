#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <string>
#include <vector>
using namespace std;

// RAC begin

typedef ac_int<1, false> ui1;
typedef ac_int<2, false> ui2;
typedef ac_int<3, false> ui3;
typedef ac_int<4, false> ui4;
typedef ac_int<5, false> ui5;
typedef ac_int<6, false> ui6;
typedef ac_int<7, false> ui7;
typedef ac_int<8, false> ui8;
typedef ac_int<16, false> ui16;
typedef ac_int<32, false> ui32;
typedef ac_int<64, false> ui64;
typedef ac_int<128, false> ui128;
typedef ac_int<5, true> si5;
typedef ac_int<8, true> si8;
typedef ac_int<16, true> si16;
typedef ac_int<32, true> si32;
typedef ac_int<64, true> si64;

//fun_signExt
ui128 rv_fun_signExt(ui128 imm)
{
        return imm.slc<1>(4) ? 0xFFFFFFFFFFFFFFE0|imm : imm;
}

ui128 rv_adc_signExt(ui128 imm, ui3 vsew)
{
	if(vsew == 0x0){
		return imm.slc<1>(4) ? ui128(0xE0)|imm : imm;
	}else if(vsew == 0x1){
		return imm.slc<1>(4) ? ui128(0xFFE0)|imm : imm;
	}else if(vsew == 0x2){
                return imm.slc<1>(4) ? ui128(0xFFFFFFE0)|imm : imm;
        }else if(vsew == 0x3){
                return imm.slc<1>(4) ? ui128(0xFFFFFFFFFFFFFFE0)|imm : imm;
        }else{
		return 0x0;
	}
}


//widden begin
ui128 rv_vwadd_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        if(vs1.slc<1>((i+1)*8-1)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<8>(i*8);
        if(vs2.slc<1>((i+1)*8-1)){
          vs2_arrext = 0xFF00|vs2_arr;
        }else{
          vs2_arrext = 0x0000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) { 
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        if(vs1.slc<1>((i+1)*16-1)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        if(vs2.slc<1>((i+1)*16-1)){
          vs2_arrext = 0xFFFF0000|vs2_arr;
        }else{
          vs2_arrext = 0x00000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        if(vs1.slc<1>((i+1)*32-1)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        if(vs2.slc<1>((i+1)*32-1)){
          vs2_arrext = 0xFFFFFFFF00000000|vs2_arr;
        }else{
          vs2_arrext = 0x0000000000000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwadd_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
        if(vs1.slc<1>(7)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<8>(i*8);
        if(vs2.slc<1>((i+1)*8-1)){
          vs2_arrext = 0xFF00|vs2_arr;
        }else{
          vs2_arrext = 0x0000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
        if(vs1.slc<1>(15)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        if(vs2.slc<1>((i+1)*16-1)){
          vs2_arrext = 0xFFFF0000|vs2_arr;
        }else{
          vs2_arrext = 0x00000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
        if(vs1.slc<1>(31)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        if(vs2.slc<1>((i+1)*32-1)){
          vs2_arrext = 0xFFFFFFFF00000000|vs2_arr;
        }else{
          vs2_arrext = 0x0000000000000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwadd_wv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        if(vs1.slc<1>((i+1)*8-1)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        if(vs1.slc<1>((i+1)*16-1)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        if(vs1.slc<1>((i+1)*32-1)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwadd_wx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
        if(vs1.slc<1>(7)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
        if(vs1.slc<1>(15)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
        if(vs1.slc<1>(31)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwaddu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<8>(i*8);
        vs2_arrext = 0x0000|vs2_arr;
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
          vs2_arrext = 0x00000000|vs2_arr;
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
          vs2_arrext = 0x0000000000000000|vs2_arr;
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwaddu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
          vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<8>(i*8);
          vs2_arrext = 0x0000|vs2_arr;
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
          vs2_arrext = 0x00000000|vs2_arr;
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
          vs2_arrext = 0x0000000000000000|vs2_arr;
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwaddu_wv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
          vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwaddu_wx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
          vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arrext + vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwsub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        if(vs1.slc<1>((i+1)*8-1)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<8>(i*8);
        if(vs2.slc<1>((i+1)*8-1)){
          vs2_arrext = 0xFF00|vs2_arr;
        }else{
          vs2_arrext = 0x0000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        if(vs1.slc<1>((i+1)*16-1)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        if(vs2.slc<1>((i+1)*16-1)){
          vs2_arrext = 0xFFFF0000|vs2_arr;
        }else{
          vs2_arrext = 0x00000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        if(vs1.slc<1>((i+1)*32-1)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        if(vs2.slc<1>((i+1)*32-1)){
          vs2_arrext = 0xFFFFFFFF00000000|vs2_arr;
        }else{
          vs2_arrext = 0x0000000000000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwsub_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
        if(vs1.slc<1>(7)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<8>(i*8);
        if(vs2.slc<1>((i+1)*8-1)){
          vs2_arrext = 0xFF00|vs2_arr;
        }else{
          vs2_arrext = 0x0000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
        if(vs1.slc<1>(15)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        if(vs2.slc<1>((i+1)*16-1)){
          vs2_arrext = 0xFFFF0000|vs2_arr;
        }else{
          vs2_arrext = 0x00000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
        if(vs1.slc<1>(31)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        if(vs2.slc<1>((i+1)*32-1)){
          vs2_arrext = 0xFFFFFFFF00000000|vs2_arr;
        }else{
          vs2_arrext = 0x0000000000000000|vs2_arr;
        }
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
}
  return vd;
}

ui128 rv_vwsub_wv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        if(vs1.slc<1>((i+1)*8-1)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        if(vs1.slc<1>((i+1)*16-1)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        if(vs1.slc<1>((i+1)*32-1)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}
ui128 rv_vwsub_wx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
        if(vs1.slc<1>(7)){
          vs1_arrext = 0xFF00|vs1_arr;
        }else{
          vs1_arrext = 0x0000|vs1_arr;
        }
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
        if(vs1.slc<1>(15)){
          vs1_arrext = 0xFFFF0000|vs1_arr;
        }else{
          vs1_arrext = 0x00000000|vs1_arr;
        }
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
        if(vs1.slc<1>(31)){
          vs1_arrext = 0xFFFFFFFF00000000|vs1_arr;
        }else{
          vs1_arrext = 0x0000000000000000|vs1_arr;
        }
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwsubu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<8>(i*8);
        vs2_arrext = 0x0000|vs2_arr;
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
          vs2_arrext = 0x00000000|vs2_arr;
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
          vs2_arrext = 0x0000000000000000|vs2_arr;
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwsubu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr;
     ui16 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
          vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<8>(i*8);
          vs2_arrext = 0x0000|vs2_arr;
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr;
     ui32 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
          vs2_arrext = 0x00000000|vs2_arr;
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr;
     ui64 vs1_arrext, vs2_arrext, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
          vs2_arrext = 0x0000000000000000|vs2_arr;
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arrext - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwsubu_wv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(i*8);
          vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(i*16);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(i*32);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vwsubu_wx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr;
     ui16 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<8>(0);
          vs1_arrext = 0x0000|vs1_arr;
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr;
     ui32 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<16>(0);
          vs1_arrext = 0x00000000|vs1_arr;
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr;
     ui64 vs1_arrext, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<32>(0);
          vs1_arrext = 0x0000000000000000|vs1_arr;
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arrext;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

//widden end

//Narrow begin
//vnsra_wi
ui128 rv_vnsra_wi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = ui8(imm);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = ui16(si16(vs2_arr) >> shamt).slc<8>(0);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = ui16(imm);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = ui32(si32(vs2_arr) >> shamt).slc<16>(0);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = ui32(imm);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0); 
              vd_arr = ui64(si64(vs2_arr) >> shamt).slc<32>(0);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  return vd;
}

//vnsra_wv
ui128 rv_vnsra_wv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = ui16(si16(vs2_arr) >> shamt).slc<8>(0);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = ui32(si32(vs2_arr) >> shamt).slc<16>(0);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = ui64(si64(vs2_arr) >> shamt).slc<32>(0);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  return vd;
}

//vnsra_vx
ui128 rv_vnsra_wx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = ui16(si16(vs2_arr) >> shamt).slc<8>(0);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = ui32(si32(vs2_arr) >> shamt).slc<16>(0);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = ui64(si64(vs2_arr) >> shamt).slc<32>(0);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  return vd;
}

//vnsrl_wi
ui128 rv_vnsrl_wi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = ui8(imm);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = (vs2_arr >> shamt).slc<8>(0);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = ui16(imm);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = (vs2_arr >> shamt).slc<16>(0);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = ui32(imm);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = (vs2_arr >> shamt).slc<32>(0);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  return vd;
}

//vnsrl_wv
ui128 rv_vnsrl_wv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = (vs2_arr >> shamt).slc<8>(0);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = (vs2_arr >> shamt).slc<16>(0);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = (vs2_arr >> shamt).slc<32>(0);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  return vd;
}

//vnsrl_wx
ui128 rv_vnsrl_wx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = (vs2_arr >> shamt).slc<8>(0);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = (vs2_arr >> shamt).slc<16>(0);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = (vs2_arr >> shamt).slc<32>(0);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  return vd;
}

//rv_vmadc_vvm
ui128 rv_vmadc_vvm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<16){
            vs1_arr = ui128(vs1.slc<8>(i*8));
            vs2_arr = ui128(vs2.slc<8>(i*8));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(8);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }   
  }
  if (vsew == 0x1) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<8){
            vs1_arr = ui128(vs1.slc<16>(i*16));
            vs2_arr = ui128(vs2.slc<16>(i*16));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(16);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x2) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<4){
            vs1_arr = ui128(vs1.slc<32>(i*32));
            vs2_arr = ui128(vs2.slc<32>(i*32));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(32);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x3) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<2){
            vs1_arr = ui128(vs1.slc<64>(i*64));
            vs2_arr = ui128(vs2.slc<64>(i*64));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(64);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  return vd;
}

//rv_vmadc_vxm
ui128 rv_vmadc_vxm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<16){
            vs1_arr = ui128(vs1.slc<8>(0));
            vs2_arr = ui128(vs2.slc<8>(i*8));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(8);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x1) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<8){
            vs1_arr = ui128(vs1.slc<16>(0));
            vs2_arr = ui128(vs2.slc<16>(i*16));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(16);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x2) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<4){
            vs1_arr = ui128(vs1.slc<32>(0));
            vs2_arr = ui128(vs2.slc<32>(i*32));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(32);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x3) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<2){
            vs1_arr = ui128(vs1.slc<64>(0));
            vs2_arr = ui128(vs2.slc<64>(i*64));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(64);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  return vd;
}

//rv_vmadc_vim
ui128 rv_vmadc_vim(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<16){
            vs1_arr = rv_adc_signExt(imm, vsew);
            vs2_arr = ui128(vs2.slc<8>(i*8));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(8);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x1) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<8){
            vs1_arr = rv_adc_signExt(imm, vsew);
            vs2_arr = ui128(vs2.slc<16>(i*16));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(16);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x2) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<4){
            vs1_arr = rv_adc_signExt(imm, vsew);
            vs2_arr = ui128(vs2.slc<32>(i*32));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(32);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x3) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<2){
            vs1_arr = rv_adc_signExt(imm, vsew);
            vs2_arr = ui128(vs2.slc<64>(i*64));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs1_arr + vs2_arr + vm[i];
                } else {
                   result = vs1_arr + vs2_arr;
                }
               vd_arr = result.slc<1>(64);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  return vd;
}

//rv_vmsbc_vvm
ui128 rv_vmsbc_vvm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<16){
            vs1_arr = ui128(vs1.slc<8>(i*8));
            vs2_arr = ui128(vs2.slc<8>(i*8));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(8);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x1) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<8){
            vs1_arr = ui128(vs1.slc<16>(i*16));
            vs2_arr = ui128(vs2.slc<16>(i*16));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(16);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x2) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<4){
            vs1_arr = ui128(vs1.slc<32>(i*32));
            vs2_arr = ui128(vs2.slc<32>(i*32));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(32);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x3) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<2){
            vs1_arr = ui128(vs1.slc<64>(i*64));
            vs2_arr = ui128(vs2.slc<64>(i*64));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(64);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  return vd;
}

//rv_vmsbc_vxm
ui128 rv_vmsbc_vxm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<16){
            vs1_arr = ui128(vs1.slc<8>(0));
            vs2_arr = ui128(vs2.slc<8>(i*8));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(8);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x1) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<8){
            vs1_arr = ui128(vs1.slc<16>(0));
            vs2_arr = ui128(vs2.slc<16>(i*16));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(16);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x2) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<4){
            vs1_arr = ui128(vs1.slc<32>(0));
            vs2_arr = ui128(vs2.slc<32>(i*32));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(32);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  if (vsew == 0x3) {
     ui128 vs1_arr, vs2_arr;
     ui1 oldvd_arr, vd_arr;
     ui128 result;
     for (uint i = 0; i < 128; i++) {
        if(i<2){
            vs1_arr = ui128(vs1.slc<64>(0));
            vs2_arr = ui128(vs2.slc<64>(i*64));
            oldvd_arr = oldvd.slc<1>(i);
            if ((i >= vstart) && (i < vl)) {
                if (Mask == 1) {
                   result = vs2_arr - vs1_arr - vm[i];
                } else {
                   result = vs2_arr - vs1_arr;
                }
               vd_arr = result.slc<1>(64);
               vd.set_slc(i,vd_arr);
            } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
        }
     }
  }
  return vd;
}

//rv_vmseq_vi
ui128 rv_vmseq_vi(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs2_arr; 
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<8>(0) == vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<16>(0) == vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<32>(0) == vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<64>(0) == vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmseq_vv
ui128 rv_vmseq_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(i*8);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) == si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(i*16);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) == si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else { 
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(i*32);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) == si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else { 
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(i*64);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) == si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else { 
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmseq_vx
ui128 rv_vmseq_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) == si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) == si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) == si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) == si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsgt_vi
ui128 rv_vmsgt_vi(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs2_arr;  
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(rv_fun_signExt(imm).slc<8>(0)) < si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else { 
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs2_arr; 
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(rv_fun_signExt(imm).slc<16>(0)) < si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs2_arr; 
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(rv_fun_signExt(imm).slc<32>(0)) < si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else { 
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(rv_fun_signExt(imm).slc<64>(0)) < si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsgt_vx
ui128 rv_vmsgt_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) < si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else { 
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) < si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) < si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) < si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsgtu_vi
ui128 rv_vmsgtu_vi(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<8>(0) < vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<16>(0) < vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<32>(0) < vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<64>(0) < vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsgtu_vx
ui128 rv_vmsgtu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr < vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr < vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr < vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr < vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsle_vi
ui128 rv_vmsle_vi(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(rv_fun_signExt(imm).slc<8>(0)) >= si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(rv_fun_signExt(imm).slc<16>(0)) >= si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(rv_fun_signExt(imm).slc<32>(0)) >= si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(rv_fun_signExt(imm).slc<64>(0)) >= si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsle_vv
ui128 rv_vmsle_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(i*8);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) >= si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(i*16);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) >= si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(i*32);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) >= si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(i*64);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) >= si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsle_vx
ui128 rv_vmsle_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) >= si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) >= si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) >= si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) >= si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsleu_vi
ui128 rv_vmsleu_vi(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<8>(0) >= vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<16>(0) >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<32>(0) >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<64>(0) >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsleu_vv
ui128 rv_vmsleu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(i*8);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else { 
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(i*16);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(i*32);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(i*64);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsleu_vx
ui128 rv_vmsleu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else { 
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr >= vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmslt_vv
ui128 rv_vmslt_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(i*8);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) > si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(i*16);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) > si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(i*32);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) > si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(i*64);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) > si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmslt_vx
ui128 rv_vmslt_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) > si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) > si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) > si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) > si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsltu_vv
ui128 rv_vmsltu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(i*8);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(i*16);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) { 
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = 0x1;
                   } else { 
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(i*32);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(i*64);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsltu_vx
ui128 rv_vmsltu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(vs1_arr > vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsne_vi
ui128 rv_vmsne_vi(ui128 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<8>(0) != vs2_arr) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<16>(0) != vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<32>(0) != vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(rv_fun_signExt(imm).slc<64>(0) != vs2_arr) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsne_vv
ui128 rv_vmsne_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(i*8);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) != si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(i*16);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) != si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(i*32);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) != si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(i*64);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) != si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//rv_vmsne_vx
ui128 rv_vmsne_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui1 oldvd_arr, vd_arr;
     ui8 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<16){
             vs1_arr = vs1.slc<8>(0);
             vs2_arr = vs2.slc<8>(i*8);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si8(vs1_arr) != si8(vs2_arr)) {
                     vd_arr = ui1(0x1);
                   } else {
                     vd_arr = ui1(0x0);
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x1) {
     ui1 oldvd_arr, vd_arr;
     ui16 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<8){
             vs1_arr = vs1.slc<16>(0);
             vs2_arr = vs2.slc<16>(i*16);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si16(vs1_arr) != si16(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x2) {
     ui1 oldvd_arr, vd_arr;
     ui32 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<4){
             vs1_arr = vs1.slc<32>(0);
             vs2_arr = vs2.slc<32>(i*32);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si32(vs1_arr) != si32(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  if (vsew == 0x3) {
     ui1 oldvd_arr, vd_arr;
     ui64 vs1_arr, vs2_arr;
     for (uint i = 0; i < 128; i++) {
         if(i<2){
             vs1_arr = vs1.slc<64>(0);
             vs2_arr = vs2.slc<64>(i*64);
             oldvd_arr = oldvd.slc<1>(i);
             if ((i >= vstart) && (i < vl)) {
                if ((Mask == 1) && (vm[i] == 0)) {
                   if (vma) {
                      vd_arr  = ui1(0x1);
                   } else {
                      vd_arr  = oldvd_arr;
                   }
                } else {
                   if(si64(vs1_arr) != si64(vs2_arr)) {
                     vd_arr = 0x1;
                   } else {
                     vd_arr = 0x0;
                   }
                }
                vd.set_slc(i,vd_arr);
             } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
                vd.set_slc(i,oldvd_arr);
             } else {
                vd.set_slc(i,ui1(0x1));
             }
         }else{
            oldvd_arr = oldvd.slc<1>(i);
            if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
               vd.set_slc(i,oldvd_arr);
            } else {
               vd.set_slc(i,ui1(0x1));
            }
         }
     }
  }
  return vd;
}

//Narrow end 

ui128 rv_vsub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsub_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs2_arr - vs1_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vrsub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vrsub_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr - vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vminu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vminu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmin_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si8(vs1_arr) < si8(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si16(vs1_arr) < si16(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si32(vs1_arr) < si32(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si64(vs1_arr) < si64(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmin_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si8(vs1_arr) < si8(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si16(vs1_arr) < si16(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si32(vs1_arr) < si32(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si64(vs1_arr) < si64(vs2_arr)) {
                vd_arr = vs1_arr;
              } else {
                vd_arr = vs2_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmaxu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmaxu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(vs1_arr < vs2_arr) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmax_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si8(vs1_arr) < si8(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si16(vs1_arr) < si16(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si32(vs1_arr) < si32(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si64(vs1_arr) < si64(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmax_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si8(vs1_arr) < si8(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si16(vs1_arr) < si16(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si32(vs1_arr) < si32(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              if(si64(vs1_arr) < si64(vs2_arr)) {
                vd_arr = vs2_arr;
              } else {
                vd_arr = vs1_arr;
              }
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vand_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = ui8(si5(imm));
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = ui16(si5(imm));
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = ui32(si5(imm));
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = ui64(si5(imm));
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vand_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vand_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr & vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vor_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = ui8(si5(imm));
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = ui16(si5(imm));
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = ui32(si5(imm));
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = ui64(si5(imm));
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vor_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vor_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr | vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vxor_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = ui8(si5(imm));
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = ui16(si5(imm));
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = ui32(si5(imm));
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = ui64(si5(imm));
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vxor_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vxor_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(0);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(0);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(0);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(0);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = vs1_arr ^ vs2_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vadc_vim(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = ui8(si5(imm)) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = ui16(si5(imm)) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = ui32(si5(imm)) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = ui64(si5(imm)) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vadc_vvm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs1_arr;
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr[i] = vs1.slc<8>(i*8);
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vs1_arr[i] + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs1_arr;
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr[i] = vs1.slc<16>(i*16);
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vs1_arr[i] + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs1_arr;
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr[i] = vs1.slc<32>(i*32);
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vs1_arr[i] + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs1_arr;
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr[i] = vs1.slc<64>(i*64);
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vs1_arr[i] + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vadc_vxm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vs1.slc<8>(0) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vs1.slc<16>(0) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vs1.slc<32>(0) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vs1.slc<64>(0) + vs2_arr[i] + vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsdc_vvm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs1_arr;
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr[i] = vs1.slc<8>(i*8);
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vs2_arr[i] - vs1_arr[i] - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs1_arr;
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr[i] = vs1.slc<16>(i*16);
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vs2_arr[i] - vs1_arr[i] - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs1_arr;
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr[i] = vs1.slc<32>(i*32);
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vs2_arr[i] - vs1_arr[i] - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs1_arr;
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr[i] = vs1.slc<64>(i*64);
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vs2_arr[i] - vs1_arr[i] - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsdc_vxm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vs2_arr[i] - vs1.slc<8>(0) - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vs2_arr[i] - vs1.slc<16>(0) - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vs2_arr[i] - vs1.slc<32>(0) - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vs2_arr[i] - vs1.slc<64>(0) - vm[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vzext_vf8(ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x3) {
     ui8  vs2_arr;
     ui64 vd_arr, oldvd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui64(ui8(vs2_arr));
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsext_vf8(ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x3) {
     ui8  vs2_arr;
     ui64 vd_arr, oldvd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui64(si8(vs2_arr));
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vzext_vf4(ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x2) {
     ui8  vs2_arr;
     ui32 vd_arr, oldvd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui32(ui8(vs2_arr));
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui16 vs2_arr;
     ui64 vd_arr, oldvd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui64(ui16(vs2_arr));
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsext_vf4(ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x2) {
     ui8  vs2_arr;
     ui32 vd_arr, oldvd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui32(si8(vs2_arr));
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui16 vs2_arr;
     ui64 vd_arr, oldvd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui64(si16(vs2_arr));
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vzext_vf2(ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x1) {
     ui8  vs2_arr;
     ui16 vd_arr, oldvd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui16(ui8(vs2_arr));
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui16  vs2_arr;
     ui32 vd_arr, oldvd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui32(ui16(vs2_arr));
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui32 vs2_arr;
     ui64 vd_arr, oldvd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui64(ui32(vs2_arr));
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsext_vf2(ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x1) {
     ui8  vs2_arr;
     ui16 vd_arr, oldvd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui16(si8(vs2_arr));
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui16  vs2_arr;
     ui32 vd_arr, oldvd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui32(si16(vs2_arr));
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui32 vs2_arr;
     ui64 vd_arr, oldvd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = ui64(si32(vs2_arr));
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmv_v_i(ui5 imm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = ui8(si5(imm));
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = ui16(si5(imm));
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = ui32(si5(imm));
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = ui64(si5(imm));
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmv_v_v(ui128 vs1, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs1_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr[i] = vs1.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vs1_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs1_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr[i] = vs1.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vs1_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs1_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr[i] = vs1.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vs1_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs1_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr[i] = vs1.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vs1_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmv_v_x(ui128 vs1, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vs1.slc<8>(0);
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vs1.slc<16>(0);
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vs1.slc<32>(0);
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vs1.slc<64>(0);
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmerge_vim(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vm[i] ? ui8(si5(imm)) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vm[i] ? ui16(si5(imm)) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vm[i] ? ui32(si5(imm)) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vm[i] ? ui64(si5(imm)) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmerge_vvm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs1_arr;
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr[i] = vs1.slc<8>(i*8);
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vm[i] ? vs1_arr[i] : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs1_arr;
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr[i] = vs1.slc<16>(i*16);
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vm[i] ? vs1_arr[i] : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs1_arr;
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr[i] = vs1.slc<32>(i*32);
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vm[i] ? vs1_arr[i] : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs1_arr;
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr[i] = vs1.slc<64>(i*64);
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vm[i] ? vs1_arr[i] : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmerge_vxm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     array<ui8,16> vs2_arr;
     array<ui8,16> oldvd_arr;
     array<ui8,16> vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr[i] = vs2.slc<8>(i*8);
        oldvd_arr[i] = oldvd.slc<8>(i*8);
        vd_arr[i]  = vm[i] ? vs1.slc<8>(0) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*8,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr[i]);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     array<ui16,8> vs2_arr;
     array<ui16,8> oldvd_arr;
     array<ui16,8> vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr[i] = vs2.slc<16>(i*16);
        oldvd_arr[i] = oldvd.slc<16>(i*16);
        vd_arr[i]  = vm[i] ? vs1.slc<16>(0) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*16,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr[i]);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     array<ui32,4> vs2_arr;
     array<ui32,4> oldvd_arr;
     array<ui32,4> vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr[i] = vs2.slc<32>(i*32);
        oldvd_arr[i] = oldvd.slc<32>(i*32);
        vd_arr[i]  = vm[i] ? vs1.slc<32>(0) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*32,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr[i]);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     array<ui64,2> vs2_arr;
     array<ui64,2> oldvd_arr;
     array<ui64,2> vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr[i] = vs2.slc<64>(i*64);
        oldvd_arr[i] = oldvd.slc<64>(i*64);
        vd_arr[i]  = vm[i] ? vs1.slc<64>(0) : vs2_arr[i];
        if ((i >= vstart) && (i < vl)) {
           vd.set_slc(i*64,vd_arr[i]);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr[i]);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsll_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = imm.slc<3>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = imm.slc<4>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = imm.slc<5>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = ui6(imm);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsll_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = vs1_arr.slc<3>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsll_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = vs1.slc<3>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1.slc<4>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1.slc<5>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1.slc<6>(0);
              vd_arr = vs2_arr << shamt;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsrl_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = imm.slc<3>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = imm.slc<4>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = imm.slc<5>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = ui6(imm);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsrl_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = vs1_arr.slc<3>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsrl_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = vs1.slc<3>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1.slc<4>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1.slc<5>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1.slc<6>(0);
              vd_arr = vs2_arr >> shamt;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsra_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = imm.slc<3>(0);
              vd_arr = ui8(si8(vs2_arr) >> shamt);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = imm.slc<4>(0);
              vd_arr = ui16(si16(vs2_arr) >> shamt);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = imm.slc<5>(0);
              vd_arr = ui32(si32(vs2_arr) >> shamt);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = ui6(imm);
              vd_arr = ui64(si64(vs2_arr) >> shamt);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsra_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr = vs1.slc<8>(i*8);
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = vs1_arr.slc<3>(0);
              vd_arr = ui8(si8(vs2_arr) >> shamt);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr = vs1.slc<16>(i*16);
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1_arr.slc<4>(0);
              vd_arr = ui16(si16(vs2_arr) >> shamt);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr = vs1.slc<32>(i*32);
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1_arr.slc<5>(0);
              vd_arr = ui32(si32(vs2_arr) >> shamt);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr = vs1.slc<64>(i*64);
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1_arr.slc<6>(0);
              vd_arr = ui64(si64(vs2_arr) >> shamt);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vsra_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui3 shamt = vs1.slc<3>(0);
              vd_arr = ui8(si8(vs2_arr) >> shamt);
           }
           vd.set_slc(i*8,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui4 shamt = vs1.slc<4>(0);
              vd_arr = ui16(si16(vs2_arr) >> shamt);
           }
           vd.set_slc(i*16,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui5 shamt = vs1.slc<5>(0);
              vd_arr = ui32(si32(vs2_arr) >> shamt);
           }
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              ui6 shamt = vs1.slc<6>(0);
              vd_arr = ui64(si64(vs2_arr) >> shamt);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
//    ui5 imm;
    ui128 imm;
    ui128 vd;
   
    bool Narrow , Widden, Mask, MaskOp, vma, vta; 
    ui6 funct6;
    ui3 funct3, vsew, eewType_1, eewType_2;
    ui8 vl;
    ui7 vstart;

    Hector::registerInput("vs1", vs1);
    Hector::registerInput("vs2", vs2);
    Hector::registerInput("oldvd", oldvd);
    Hector::registerInput("vm", vm);
    Hector::registerInput("imm", imm);
    Hector::registerInput("Narrow", Narrow);
    Hector::registerInput("Widden", Widden);
    Hector::registerInput("Mask", Mask);
    Hector::registerInput("MaskOp", MaskOp);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vma", vma);
    Hector::registerInput("vta", vta);
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("eewType_1", eewType_1);
    Hector::registerInput("eewType_2", eewType_2);
    
    Hector::registerOutput("vd", vd);
    
    Hector::beginCapture();

    vd = 0x888;
    if((funct6 == 0x31) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwadd_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x31) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwadd_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x35) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwadd_wv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x35) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwadd_wx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x30) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwaddu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x30) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwaddu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x34) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwaddu_wv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x34) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwaddu_wx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x33) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwsub_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x33) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwsub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x37) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwsub_wv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x37) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwsub_wx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x32) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwsubu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x32) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x3)) { vd = rv_vwsubu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x36) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwsubu_wv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x36) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x1) && (eewType_1 == 0x4)) { vd = rv_vwsubu_wx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    if((funct6 == 0x2D) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (eewType_2 == 0x3)) { vd = rv_vnsra_wi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2D) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (eewType_2 == 0x3)) { vd = rv_vnsra_wv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2D) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (eewType_2 == 0x3)) { vd = rv_vnsra_wx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2C) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (eewType_2 == 0x3)) { vd = rv_vnsrl_wi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2C) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (eewType_2 == 0x3)) { vd = rv_vnsrl_wv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2C) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (eewType_2 == 0x3)) { vd = rv_vnsrl_wx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    if((funct6 == 0x11) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x1) && (vta == 0x1)) { vd = rv_vmadc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x11) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x0) && (vta == 0x1)) { vd = rv_vmadc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x11) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x1) && (vta == 0x1)) { vd = rv_vmadc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x11) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x0) && (vta == 0x1)) { vd = rv_vmadc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x11) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x1) && (vta == 0x1)) { vd = rv_vmadc_vim(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x11) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x0) && (vta == 0x1)) { vd = rv_vmadc_vim(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x1) && (vta == 0x1)) { vd = rv_vmsbc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x0) && (vta == 0x1)) { vd = rv_vmsbc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x1) && (vta == 0x1)) { vd = rv_vmsbc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (Mask == 0x0) && (vta == 0x1)) { vd = rv_vmsbc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    if((funct6 == 0x18) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmseq_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x18) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmseq_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x18) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmseq_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1F) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsgt_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1F) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsgt_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1E) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsgtu_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1E) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsgtu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1D) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsle_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1D) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsle_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1D) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsle_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1C) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsleu_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1C) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsleu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1C) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsleu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1B) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmslt_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1B) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmslt_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1A) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsltu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1A) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsltu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x19) && (funct3 == 0x3) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsne_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x19) && (funct3 == 0x0) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsne_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x19) && (funct3 == 0x4) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 0x1)) { vd = rv_vmsne_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }


//    if((funct6 == 0x02) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x03) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vrsub_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x03) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vrsub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x04) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vminu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x04) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vminu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x05) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmin_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x05) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmin_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x06) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmaxu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x06) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmaxu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x07) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmax_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x07) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmax_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x09) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vand_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x09) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vand_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x09) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vand_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x0A) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vor_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x0A) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vor_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x0A) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vor_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x0B) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vxor_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x0B) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vxor_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x0B) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vxor_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x10) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vadc_vim(imm, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x10) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vadc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x10) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vadc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x12) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vsdc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x12) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vsdc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x02)) { vd = rv_vzext_vf8(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x03)) { vd = rv_vsext_vf8(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x04)) { vd = rv_vzext_vf4(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x05)) { vd = rv_vsext_vf4(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x06)) { vd = rv_vzext_vf2(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x07)) { vd = rv_vsext_vf2(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x17) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { vd = rv_vmv_v_i(imm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x17) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { vd = rv_vmv_v_v(vs1, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x17) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { vd = rv_vmv_v_x(vs1, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x17) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vmerge_vim(imm, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x17) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vmerge_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x17) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vmerge_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x25) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsll_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x25) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsll_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x25) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsll_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x28) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsrl_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x28) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsrl_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x28) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsrl_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x29) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsra_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x29) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsra_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
//    if((funct6 == 0x29) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsra_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    
    Hector::endCapture();
}

