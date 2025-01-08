#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <string>
#include <vector>
using namespace std;

// RAC begin

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
typedef ac_int<128, true> si128;

ui128 rv_vredsum_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr + vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr + vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr + vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr + vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

ui128 rv_vredand_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr & vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr & vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr & vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr & vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}
ui128 rv_vredor_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr | vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr | vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr | vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr | vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

ui128 rv_vredxor_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr ^ vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr ^ vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr ^ vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              vd_arr  = vd_arr ^ vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

ui128 rv_vredminu_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((vs2_arr < vd_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((vs2_arr < vd_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((vs2_arr < vd_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((vs2_arr < vd_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

ui128 rv_vredmin_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((si8(vs2_arr) < si8(vd_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((si16(vs2_arr) < si16(vd_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((si32(vs2_arr) < si32(vd_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((si64(vs2_arr) < si64(vd_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

ui128 rv_vredmaxu_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((vd_arr < vs2_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((vd_arr < vs2_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((vd_arr < vs2_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((vd_arr < vs2_arr) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

ui128 rv_vredmax_vs(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<8>(0);
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        if (i < vl) {
           if ((si8(vd_arr) < si8(vs2_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 16; i++) {
           oldvd_arr = oldvd.slc<8>(i*8);
           if (vta == 0) {
              vd.set_slc(i*8,oldvd_arr);
           } else {
              vd.set_slc(i*8,ui8(0xFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x1) {
     ui16  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<16>(0);
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        if (i < vl) {
           if ((si16(vd_arr) < si16(vs2_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 8; i++) {
           oldvd_arr = oldvd.slc<16>(i*16);
           if (vta == 0) {
              vd.set_slc(i*16,oldvd_arr);
           } else {
              vd.set_slc(i*16,ui16(0xFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x2) {
     ui32  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<32>(0);
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((si32(vd_arr) < si32(vs2_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 4; i++) {
           oldvd_arr = oldvd.slc<32>(i*32);
           if (vta == 0) {
              vd.set_slc(i*32,oldvd_arr);
           } else {
              vd.set_slc(i*32,ui32(0xFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, vd_arr, oldvd_arr;
     vd_arr  = vs1.slc<64>(0);
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((si64(vd_arr) < si64(vs2_arr)) && !((Mask == 1) && (vm[i] == 0))) {
              vd_arr  = vs2_arr;
           }
        }
     }
     if (vl != 0) {
        vd.set_slc(0,vd_arr);
        for (uint i = 1; i < 2; i++) {
           oldvd_arr = oldvd.slc<64>(i*64);
           if (vta == 0) {
              vd.set_slc(i*64,oldvd_arr);
           } else {
              vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
           }
        }
     } else {
       vd = oldvd;
     }
  }
  return vd;
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
    ui5 imm;
    ui128 vd;
   
    bool Widden, Mask, vma, vta, vxsat; 
    ui2 vxrm;
    ui6 funct6;
    ui3 funct3, vsew;
    ui8 vl;
    ui7 vstart;

    Hector::registerInput("vs1", vs1);
    Hector::registerInput("vs2", vs2);
    Hector::registerInput("oldvd", oldvd);
    Hector::registerInput("vm", vm);
    Hector::registerInput("imm", imm);
    Hector::registerInput("Widden", Widden);
    Hector::registerInput("Mask", Mask);
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vxrm", vxrm);
    
    Hector::registerOutput("vd", vd);
    Hector::registerOutput("vxsat", vxsat);
    
    Hector::beginCapture();

    vd = 0;
    if((funct6 == 0x00) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredsum_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x01) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredand_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x02) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredor_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x03) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredxor_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x04) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredminu_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x05) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredmin_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x06) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredmaxu_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x07) && (funct3 == 0x2) && (Widden == 0x0) && (vstart == 0x0)) { vd = rv_vredmax_vs(vs1, vs2, vm, oldvd, vl, vsew, vma, vta, Mask); }
    
    Hector::endCapture();
}

