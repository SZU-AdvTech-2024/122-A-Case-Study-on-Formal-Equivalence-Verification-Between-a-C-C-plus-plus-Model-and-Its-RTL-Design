#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <arith.h>
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

// Rounding modes:

enum Rmode {RNU, RNE, RDN, ROD};

template<typename UT>
static inline UT roundoff(UT v, UT vavg, ui2 Rmode, int d)
{
  UT res;
  bool r = false;
  int  dm1 = d - 1;
  UT lsb = UT(1) << d;
  UT lsb_half = lsb >> 1;
  switch (Rmode) {
    case RNU:
      r = (((v >> dm1) & 0x1) != 0);
      break;
    case RNE:
      if (d == 1) {
         r = (((v >> dm1) & 0x1) != 0) & (((v >> d) & 0x1) != 0);
      } else {
         r = (((v >> dm1) & 0x1) != 0) & (((v & (lsb_half - 1)) != 0) | (((v >> d) & 0x1) != 0));
      }
      break;
    case RDN:
      r = false;
      break;
    case ROD:
      r = (((v >> d) & 0x1) == 0) & ((v & (lsb - 1)) != 0);
      break;
  }
  res = vavg + r;
  return res;
}

ui128 rv_vaaddu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vaaddu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr + vs1_arr;
              vavg_arr = (vs2_arr + vs1_arr) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vaadd_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si8(vs2_arr) + si8(vs1_arr);
              vavg_arr = (si8(vs2_arr) + si8(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si16(vs2_arr) + si16(vs1_arr);
              vavg_arr = (si16(vs2_arr) + si16(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si32(vs2_arr) + si32(vs1_arr);
              vavg_arr = (si32(vs2_arr) + si32(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si64(vs2_arr) + si64(vs1_arr);
              vavg_arr = (si64(vs2_arr) + si64(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vaadd_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si8(vs2_arr) + si8(vs1_arr);
              vavg_arr = (si8(vs2_arr) + si8(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si16(vs2_arr) + si16(vs1_arr);
              vavg_arr = (si16(vs2_arr) + si16(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si32(vs2_arr) + si32(vs1_arr);
              vavg_arr = (si32(vs2_arr) + si32(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si64(vs2_arr) + si64(vs1_arr);
              vavg_arr = (si64(vs2_arr) + si64(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vasubu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vasubu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr - vs1_arr;
              vavg_arr = (vs2_arr - vs1_arr) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vasub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si8(vs2_arr) - si8(vs1_arr);
              vavg_arr = (si8(vs2_arr) - si8(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si16(vs2_arr) - si16(vs1_arr);
              vavg_arr = (si16(vs2_arr) - si16(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si32(vs2_arr) - si32(vs1_arr);
              vavg_arr = (si32(vs2_arr) - si32(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si64(vs2_arr) - si64(vs1_arr);
              vavg_arr = (si64(vs2_arr) - si64(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

ui128 rv_vasub_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si8(vs2_arr) - si8(vs1_arr);
              vavg_arr = (si8(vs2_arr) - si8(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui8>(vd_arr, vavg_arr, vxrm, 1);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si16(vs2_arr) - si16(vs1_arr);
              vavg_arr = (si16(vs2_arr) - si16(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui16>(vd_arr, vavg_arr, vxrm, 1);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si32(vs2_arr) - si32(vs1_arr);
              vavg_arr = (si32(vs2_arr) - si32(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui32>(vd_arr, vavg_arr, vxrm, 1);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = si64(vs2_arr) - si64(vs1_arr);
              vavg_arr = (si64(vs2_arr) - si64(vs1_arr)) >> 1;
              vd_arr   = roundoff<ui64>(vd_arr, vavg_arr, vxrm, 1);
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

tuple<ui128, bool> rv_vsaddu_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = ui8(si5(imm));
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = ui16(si5(imm));
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = ui32(si5(imm));
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = ui64(si5(imm));
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vsaddu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vsaddu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr    = vs2_arr + vs1_arr;
              sat_arr   = vd_arr < vs2_arr;
              vd_arr   |= -(vd_arr < vs2_arr);
              sat      |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vsadd_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = ui8(si5(imm));
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int8_t,uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = ui16(si5(imm));
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int16_t,uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = ui32(si5(imm));
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int32_t,uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = ui64(si5(imm));
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int64_t,uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vsadd_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int8_t,uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int16_t,uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int32_t,uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int64_t,uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vsadd_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int8_t,uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int16_t,uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int32_t,uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_add<int64_t,uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vssubu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vssubu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_subu<uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vssub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int8_t,uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int16_t,uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int32_t,uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int64_t,uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

tuple<ui128, bool> rv_vssub_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int8_t,uint8_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int16_t,uint16_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int32_t,uint32_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = sat_sub<int64_t,uint64_t>(vs2_arr, vs1_arr, sat_arr);
              sat    |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, bool>(vd, sat);
}

ui128 rv_vssrl_vi(ui5 imm, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm.slc<3>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui8>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui16 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm.slc<4>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui16>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui32 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm;
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui32>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui64 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm;
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui64>(vs2_arr, vavg_arr, vxrm, shamt);
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

ui128 rv_vssrl_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<3>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui8>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<4>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui16>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<5>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui32>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<6>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui64>(vs2_arr, vavg_arr, vxrm, shamt);
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

ui128 rv_vssrl_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<3>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui8>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui16 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<4>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui16>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui32 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<5>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui32>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui64 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<6>(0);
              vavg_arr  = vs2_arr >> shamt;
              vd_arr    = roundoff<ui64>(vs2_arr, vavg_arr, vxrm, shamt);
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

ui128 rv_vssra_vi(ui5 imm, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm.slc<3>(0);
              vavg_arr  = si8(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui8>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui16 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm.slc<4>(0);
              vavg_arr  = si16(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui16>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui32 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm;
              vavg_arr  = si32(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui32>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui64 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = imm;
              vavg_arr  = si64(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui64>(vs2_arr, vavg_arr, vxrm, shamt);
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

ui128 rv_vssra_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<3>(0);
              vavg_arr  = si8(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui8>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui16 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<4>(0);
              vavg_arr  = si16(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui16>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui32 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<5>(0);
              vavg_arr  = si32(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui32>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui64 vs1_arr, vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1_arr.slc<6>(0);
              vavg_arr  = si64(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui64>(vs2_arr, vavg_arr, vxrm, shamt);
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

ui128 rv_vssra_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<3>(0);
              vavg_arr  = si8(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui8>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui16 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<4>(0);
              vavg_arr  = si16(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui16>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui32 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<5>(0);
              vavg_arr  = si32(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui32>(vs2_arr, vavg_arr, vxrm, shamt);
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
     ui64 vs2_arr, oldvd_arr, vavg_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              int shamt = vs1.slc<6>(0);
              vavg_arr  = si64(vs2_arr) >> shamt;
              vd_arr    = roundoff<ui64>(vs2_arr, vavg_arr, vxrm, shamt);
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
    ui5 imm;
    ui128 vd;
   
    bool Narrow , Widden, Mask, vma, vta, vxsat; 
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
    Hector::registerInput("Narrow", Narrow);
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
    vxsat = false;
    if((funct6 == 0x08) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vaaddu_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x08) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vaaddu_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vaadd_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vaadd_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vasubu_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vasubu_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0B) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vasub_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0B) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vasub_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x20) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsaddu_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x20) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsaddu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x20) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsaddu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x21) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsadd_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x21) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsadd_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x21) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsadd_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x22) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vssubu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x22) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vssubu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x23) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vssub_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x23) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vssub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2A) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vssrl_vi(imm, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2A) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vssrl_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2A) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vssrl_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2B) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vssra_vi(imm, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2B) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vssra_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2B) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vssra_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    
    Hector::endCapture();
}

#include "Hector.h"
#include "softfloat.h"
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

float32_t f32_min( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  bool islt = f32_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFF) == 0x00000000) && ((b.v & 0x7FFFFFFF) == 0x00000000));
  bool hasNZero = ((a.v == 0x80000000) || (b.v == 0x80000000));
  bool a_isNaN  = (((a.v & 0x7F800000) == 0x7F800000) && (a.v & 0x007FFFFF));
  bool b_isNaN  = (((b.v & 0x7F800000) == 0x7F800000) && (b.v & 0x007FFFFF));
  bool a_isSNaN = (((a.v & 0x7FC00000) == 0x7F800000) && (a.v & 0x003FFFFF));
  bool b_isSNaN = (((b.v & 0x7FC00000) == 0x7F800000) && (b.v & 0x003FFFFF));
  if(islt) {
      res = a.v;
  } else {
      res = b.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FC00000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasNZero) {
          res = 0x80000000;
      } else {
          res = 0x00000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float32_t f32_max( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  bool islt = f32_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFF) == 0x00000000) && ((b.v & 0x7FFFFFFF) == 0x00000000));
  bool hasPZero = ((a.v == 0x00000000) || (b.v == 0x00000000));
  bool a_isNaN = (((a.v & 0x7F800000) == 0x7F800000) && (a.v & 0x007FFFFF));
  bool b_isNaN = (((b.v & 0x7F800000) == 0x7F800000) && (b.v & 0x007FFFFF));
  bool a_isSNaN = (((a.v & 0x7FC00000) == 0x7F800000) && (a.v & 0x003FFFFF));
  bool b_isSNaN = (((b.v & 0x7FC00000) == 0x7F800000) && (b.v & 0x003FFFFF));
  if(islt) {
      res = b.v;
  } else {
      res = a.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FC00000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasPZero) {
          res = 0x00000000;
      } else {
          res = 0x80000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float32_t f32_sgnj( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  res = (a.v & 0x7FFFFFFF) | (b.v & 0x80000000);
  fd.v = res;
  return fd;
}

float32_t f32_sgnjn( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  res = (a.v & 0x7FFFFFFF) | (~b.v & 0x80000000);
  fd.v = res;
  return fd;
}

float32_t f32_sgnjx( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  res = (a.v & 0x7FFFFFFF) | ((a.v ^ b.v) & 0x80000000);
  fd.v = res;
  return fd;
}

float64_t f64_min( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  bool islt = f64_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000) && ((b.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000));
  bool hasNZero = ((a.v == 0x8000000000000000) || (b.v == 0x8000000000000000));
  bool a_isNaN  = (((a.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (a.v & 0x000FFFFFFFFFFFFF));
  bool b_isNaN  = (((b.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (b.v & 0x000FFFFFFFFFFFFF));
  bool a_isSNaN = (((a.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (a.v & 0x0007FFFFFFFFFFFF));
  bool b_isSNaN = (((b.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (b.v & 0x0007FFFFFFFFFFFF));
  if(islt) {
      res = a.v;
  } else {
      res = b.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FF8000000000000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasNZero) {
          res = 0x8000000000000000;
      } else {
          res = 0x0000000000000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float64_t f64_max( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  bool islt = f64_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000) && ((b.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000));
  bool hasPZero = ((a.v == 0x0000000000000000) || (b.v == 0x0000000000000000));
  bool a_isNaN  = (((a.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (a.v & 0x000FFFFFFFFFFFFF));
  bool b_isNaN  = (((b.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (b.v & 0x000FFFFFFFFFFFFF));
  bool a_isSNaN = (((a.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (a.v & 0x0007FFFFFFFFFFFF));
  bool b_isSNaN = (((b.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (b.v & 0x0007FFFFFFFFFFFF));
  if(islt) {
      res = b.v;
  } else {
      res = a.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FF8000000000000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasPZero) {
          res = 0x0000000000000000;
      } else {
          res = 0x8000000000000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float64_t f64_sgnj( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  res = (a.v & 0x7FFFFFFFFFFFFFFF) | (b.v & 0x8000000000000000);
  fd.v = res;
  return fd;
}

float64_t f64_sgnjn( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  res = (a.v & 0x7FFFFFFFFFFFFFFF) | (~b.v & 0x8000000000000000);
  fd.v = res;
  return fd;
}

float64_t f64_sgnjx( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  res = (a.v & 0x7FFFFFFFFFFFFFFF) | ((a.v ^ b.v) & 0x8000000000000000);
  fd.v = res;
  return fd;
}

tuple<ui128, ui5> rv_vfmin_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_min(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_min(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmin_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_min(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_min(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmax_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_max(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_max(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmax_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_max(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_max(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsgnj_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_sgnj(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sgnj(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsgnj_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_sgnj(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sgnj(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsgnjn_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_sgnjn(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sgnjn(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsgnjn_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_sgnjn(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sgnjn(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsgnjx_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_sgnjx(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sgnjx(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsgnjx_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_sgnjx(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sgnjx(f_vs2, f_vs1);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfcvt_xu_f_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = f32_to_ui32(f_vs2, frm, 1);
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = f64_to_ui64(f_vs2, frm, 1);
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfcvt_x_f_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = f32_to_i32(f_vs2, frm, 1);
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = f64_to_i64(f_vs2, frm, 1);
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfcvt_f_xu_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vd;
     for (uint i = 0; i < 4; i++) {
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = ui32_to_f32(vs2.slc<32>(i*32));
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = ui64_to_f64(vs2.slc<64>(i*64));
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfcvt_f_x_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vd;
     for (uint i = 0; i < 4; i++) {
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = i32_to_f32(si32(vs2.slc<32>(i*32)));
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = i64_to_f64(si64(vs2.slc<64>(i*64)));
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfrsqrt7_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_rsqrte7(f_vs2);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_rsqrte7(f_vs2);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfrec7_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f32_recip7(f_vs2);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_recip7(f_vs2);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfclass_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = f32_classify(f_vs2);
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr  = f64_classify(f_vs2);
              fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmv_v_f(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     ui32 f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1   = vs1.slc<32>(0);
        } else {
           f_vs1   = 0x7FC00000;
        }
        f_vs2   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f_vs1;
           vd_arr  = f_vd;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     ui64 f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1   = vs1.slc<64>(0);
        f_vs2   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f_vs1;
           vd_arr  = f_vd;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmerge_vfm(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     ui32 f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1   = vs1.slc<32>(0);
        } else {
           f_vs1   = 0x7FC00000;
        }
        f_vs2   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = vm[i] ? f_vs1 : f_vs2;
           vd_arr  = f_vd;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     ui64 f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1   = vs1.slc<64>(0);
        f_vs2   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = vm[i] ? f_vs1 : f_vs2;
           vd_arr  = f_vd;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
    ui5 imm, fflags;
    ui128 vd;
   
    bool Narrow , Widden, MaskOp, Mask, vma, vta; 
    ui2 vxrm;
    ui6 funct6;
    ui3 funct3, vsew, frm;
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
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vta", vta);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vxrm", vxrm);
    Hector::registerInput("frm", frm);
    
    Hector::registerOutput("vd", vd);
    Hector::registerOutput("fflags", fflags);
    
    Hector::beginCapture();

    vd = 0;
    fflags = 0;
    if((funct6 == 0x04) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfmin_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x04) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfmin_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x06) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfmax_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x06) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfmax_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x08) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfsgnj_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x08) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfsgnj_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfsgnjn_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfsgnjn_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfsgnjx_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0)) { tie(vd,fflags) = rv_vfsgnjx_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x00)) { tie(vd,fflags) = rv_vfcvt_xu_f_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x01)) { tie(vd,fflags) = rv_vfcvt_x_f_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x02)) { tie(vd,fflags) = rv_vfcvt_f_xu_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x03)) { tie(vd,fflags) = rv_vfcvt_f_x_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x06) && (frm == 0x1)) { tie(vd,fflags) = rv_vfcvt_xu_f_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x07) && (frm == 0x1)) { tie(vd,fflags) = rv_vfcvt_x_f_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x04)) { tie(vd,fflags) = rv_vfrsqrt7_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x05)) { tie(vd,fflags) = rv_vfrec7_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x13) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x10)) { tie(vd,fflags) = rv_vfclass_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x17) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmv_v_f(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x1)) { tie(vd,fflags) = rv_vfmerge_vfm(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vta); }
    
    Hector::endCapture();
}

#include "Hector.h"
#include "softfloat.h"
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

tuple<ui128, ui5> rv_vfadd_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_add(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_add(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfadd_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_add(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_add(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsub_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_sub(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_sub(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfsub_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_sub(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_sub(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmul_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mul(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mul(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmul_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mul(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mul(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfrsub_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_sub(f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_sub(f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
    ui5 imm, fflags;
    ui128 vd;
   
    bool Narrow , Widden, MaskOp, Mask, vma, vta; 
    ui2 vxrm;
    ui6 funct6;
    ui3 funct3, vsew, frm;
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
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vta", vta);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vxrm", vxrm);
    Hector::registerInput("frm", frm);
    
    Hector::registerOutput("vd", vd);
    Hector::registerOutput("fflags", fflags);
    
    Hector::beginCapture();

    vd = 0;
    fflags = 0;
    if((funct6 == 0x00) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfadd_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x00) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfadd_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x02) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x02) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsub_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x24) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmul_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x24) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmul_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x27) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfrsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    
    Hector::endCapture();
}

#include "Hector.h"
#include "softfloat.h"
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

tuple<ui128, ui5> rv_vfmadd_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmadd_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmadd_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32) ^ ui32(0x80000000);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64) ^ ui64(0x8000000000000000);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmadd_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32) ^ ui32(0x80000000);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64) ^ ui64(0x8000000000000000);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmsub_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32) ^ ui32(0x80000000);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64) ^ ui64(0x8000000000000000);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmsub_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32) ^ ui32(0x80000000);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64) ^ ui64(0x8000000000000000);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmsub_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmsub_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_oldvd, f_vs1, f_vs2);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmacc_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmacc_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmacc_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32) ^ ui32(0x80000000);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64) ^ ui64(0x8000000000000000);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmacc_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32) ^ ui32(0x80000000);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64) ^ ui64(0x8000000000000000);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmsac_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfmsac_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr ^ ui32(0x80000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr ^ ui64(0x8000000000000000);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmsac_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0) ^ ui32(0x80000000);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0) ^ ui64(0x8000000000000000);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfnmsac_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32) ^ ui32(0x80000000);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*32,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2, f_oldvd, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64) ^ ui64(0x8000000000000000);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        f_oldvd.v = oldvd_arr;
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mulAdd(f_vs2, f_vs1, f_oldvd);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           vd.set_slc(i*64,vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
    ui5 imm, fflags;
    ui128 vd;
   
    bool Narrow , Widden, MaskOp, Mask, vma, vta; 
    ui2 vxrm;
    ui6 funct6;
    ui3 funct3, vsew, frm;
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
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vta", vta);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vxrm", vxrm);
    Hector::registerInput("frm", frm);
    
    Hector::registerOutput("vd", vd);
    Hector::registerOutput("fflags", fflags);
    
    Hector::beginCapture();

    vd = 0;
    fflags = 0;
    if((funct6 == 0x28) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmadd_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x28) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmadd_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x29) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmadd_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x29) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmadd_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2A) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2A) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmsub_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2B) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2B) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmsub_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2C) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmacc_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2C) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmacc_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2D) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmacc_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2D) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmacc_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2E) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmsac_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2E) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmsac_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2F) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmsac_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x2F) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfnmsac_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    
    Hector::endCapture();
}

#include "Hector.h"
#include "softfloat.h"
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

float32_t f32_min( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  bool islt = f32_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFF) == 0x00000000) && ((b.v & 0x7FFFFFFF) == 0x00000000));
  bool hasNZero = ((a.v == 0x80000000) || (b.v == 0x80000000));
  bool a_isNaN  = (((a.v & 0x7F800000) == 0x7F800000) && (a.v & 0x007FFFFF));
  bool b_isNaN  = (((b.v & 0x7F800000) == 0x7F800000) && (b.v & 0x007FFFFF));
  bool a_isSNaN = (((a.v & 0x7FC00000) == 0x7F800000) && (a.v & 0x003FFFFF));
  bool b_isSNaN = (((b.v & 0x7FC00000) == 0x7F800000) && (b.v & 0x003FFFFF));
  if(islt) {
      res = a.v;
  } else {
      res = b.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FC00000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasNZero) {
          res = 0x80000000;
      } else {
          res = 0x00000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float32_t f32_max( float32_t a, float32_t b ) {
  uint32_t res;
  float32_t fd;
  bool islt = f32_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFF) == 0x00000000) && ((b.v & 0x7FFFFFFF) == 0x00000000));
  bool hasPZero = ((a.v == 0x00000000) || (b.v == 0x00000000));
  bool a_isNaN = (((a.v & 0x7F800000) == 0x7F800000) && (a.v & 0x007FFFFF));
  bool b_isNaN = (((b.v & 0x7F800000) == 0x7F800000) && (b.v & 0x007FFFFF));
  bool a_isSNaN = (((a.v & 0x7FC00000) == 0x7F800000) && (a.v & 0x003FFFFF));
  bool b_isSNaN = (((b.v & 0x7FC00000) == 0x7F800000) && (b.v & 0x003FFFFF));
  if(islt) {
      res = b.v;
  } else {
      res = a.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FC00000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasPZero) {
          res = 0x00000000;
      } else {
          res = 0x80000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float64_t f64_min( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  bool islt = f64_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000) && ((b.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000));
  bool hasNZero = ((a.v == 0x8000000000000000) || (b.v == 0x8000000000000000));
  bool a_isNaN  = (((a.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (a.v & 0x000FFFFFFFFFFFFF));
  bool b_isNaN  = (((b.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (b.v & 0x000FFFFFFFFFFFFF));
  bool a_isSNaN = (((a.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (a.v & 0x0007FFFFFFFFFFFF));
  bool b_isSNaN = (((b.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (b.v & 0x0007FFFFFFFFFFFF));
  if(islt) {
      res = a.v;
  } else {
      res = b.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FF8000000000000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasNZero) {
          res = 0x8000000000000000;
      } else {
          res = 0x0000000000000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

float64_t f64_max( float64_t a, float64_t b ) {
  uint64_t res;
  float64_t fd;
  bool islt = f64_lt(a, b);
  bool isBothZero = (((a.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000) && ((b.v & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000));
  bool hasPZero = ((a.v == 0x0000000000000000) || (b.v == 0x0000000000000000));
  bool a_isNaN  = (((a.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (a.v & 0x000FFFFFFFFFFFFF));
  bool b_isNaN  = (((b.v & 0x7FF0000000000000) == 0x7FF0000000000000) && (b.v & 0x000FFFFFFFFFFFFF));
  bool a_isSNaN = (((a.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (a.v & 0x0007FFFFFFFFFFFF));
  bool b_isSNaN = (((b.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (b.v & 0x0007FFFFFFFFFFFF));
  if(islt) {
      res = b.v;
  } else {
      res = a.v;
  }
  if(a_isNaN && b_isNaN) {
      res = 0x7FF8000000000000;
  } else if(!a_isNaN && b_isNaN) {
      res = a.v;
  } else if(a_isNaN && !b_isNaN) {
      res = b.v;
  } else if(isBothZero) {
      if(hasPZero) {
          res = 0x0000000000000000;
      } else {
          res = 0x8000000000000000;
      }
  } else {
  }
  softfloat_exceptionFlags = 0;
  if(a_isSNaN || b_isSNaN) {
      softfloat_raiseFlags( softfloat_flag_invalid );
  }
  fd.v = res;
  return fd;
}

tuple<ui128, ui5> rv_vfredosum_vs(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32  vd_arr, oldvd_arr;
     float32_t  f_vs2, f_vd;
     f_vd.v   = vs1.slc<32>(0);
     vd_arr   = f_vd.v;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v    = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              f_vd    = f32_add(f_vs2, f_vd);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64  vd_arr, oldvd_arr;
     float64_t  f_vs2, f_vd;
     f_vd.v  = vs1.slc<64>(0);
     vd_arr  = f_vd.v;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v    = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              f_vd    = f64_add(f_vs2, f_vd);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfredmax_vs(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32  vd_arr, oldvd_arr;
     float32_t  f_vs2, f_vd;
     f_vd.v   = vs1.slc<32>(0);
     vd_arr   = f_vd.v;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v    = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              f_vd    = f32_max(f_vs2, f_vd);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64  vd_arr, oldvd_arr;
     float64_t  f_vs2, f_vd;
     f_vd.v  = vs1.slc<64>(0);
     vd_arr  = f_vd.v;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v    = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              f_vd    = f64_max(f_vs2, f_vd);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
  return tuple<ui128, ui5>(vd, fflags);
}

tuple<ui128, ui5> rv_vfredmin_vs(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui32  vd_arr, oldvd_arr;
     float32_t  f_vs2, f_vd;
     f_vd.v   = vs1.slc<32>(0);
     vd_arr   = f_vd.v;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v    = vs2.slc<32>(i*32);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              f_vd    = f32_min(f_vs2, f_vd);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
     ui64  vd_arr, oldvd_arr;
     float64_t  f_vs2, f_vd;
     f_vd.v  = vs1.slc<64>(0);
     vd_arr  = f_vd.v;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v    = vs2.slc<64>(i*64);
        if (i < vl) {
           if ((Mask == 1) && (vm[i] == 0)) {
           } else {
              f_vd    = f64_min(f_vs2, f_vd);
              vd_arr  = f_vd.v;
              fflags |= (softfloat_exceptionFlags & 0x1F);
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
  return tuple<ui128, ui5>(vd, fflags);
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
    ui5 imm, fflags;
    ui128 vd;
   
    bool Narrow , Widden, MaskOp, Mask, vma, vta; 
    ui2 vxrm;
    ui6 funct6;
    ui3 funct3, vsew, frm;
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
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vta", vta);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vxrm", vxrm);
    Hector::registerInput("frm", frm);
    
    Hector::registerOutput("vd", vd);
    Hector::registerOutput("fflags", fflags);
    
    Hector::beginCapture();

    vd = 0;
    fflags = 0;
    if((funct6 == 0x01) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (vstart == 0x0)) { tie(vd,fflags) = rv_vfredosum_vs(vs1, vs2, vm, frm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x03) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (vstart == 0x0)) { tie(vd,fflags) = rv_vfredosum_vs(vs1, vs2, vm, frm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x05) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (vstart == 0x0)) { tie(vd,fflags) = rv_vfredmin_vs(vs1, vs2, vm, frm, oldvd, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x07) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (vstart == 0x0)) { tie(vd,fflags) = rv_vfredmax_vs(vs1, vs2, vm, frm, oldvd, vl, vsew, vma, vta, Mask); }
    
    Hector::endCapture();
}

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

ui128 rv_vadd_vi(ui5 imm, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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

ui128 rv_vadd_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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

ui128 rv_vadd_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
              vd_arr  = vs1_arr + vs2_arr;
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
    ui5 imm;
    ui128 vd;
   
    bool Narrow , Widden, Mask, vma, vta; 
    ui6 funct6;
    ui3 funct3, vsew;
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
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vsew", vsew);
    
    Hector::registerOutput("vd", vd);
    
    Hector::beginCapture();

    vd = 0;
    if((funct6 == 0x00) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vadd_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x00) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vadd_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x00) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vadd_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x02) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsub_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x02) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x03) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vrsub_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x03) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vrsub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x04) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vminu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x04) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vminu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x05) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmin_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x05) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmin_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x06) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmaxu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x06) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmaxu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x07) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmax_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x07) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vmax_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vand_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vand_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x09) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vand_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vor_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vor_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0A) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vor_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0B) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vxor_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0B) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vxor_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x0B) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vxor_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x10) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vadc_vim(imm, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x10) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vadc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x10) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vadc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x12) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vsdc_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x12) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vsdc_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x02)) { vd = rv_vzext_vf8(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x03)) { vd = rv_vsext_vf8(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x04)) { vd = rv_vzext_vf4(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x05)) { vd = rv_vsext_vf4(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x06)) { vd = rv_vzext_vf2(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x07)) { vd = rv_vsext_vf2(vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x17) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { vd = rv_vmv_v_i(imm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { vd = rv_vmv_v_v(vs1, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { vd = rv_vmv_v_x(vs1, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vmerge_vim(imm, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vmerge_vvm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { vd = rv_vmerge_vxm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x25) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsll_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x25) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsll_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x25) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsll_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x28) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsrl_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x28) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsrl_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x28) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsrl_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x29) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsra_vi(imm, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x29) && (funct3 == 0x0) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsra_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x29) && (funct3 == 0x4) && (Narrow == 0x0) && (Widden == 0x0)) { vd = rv_vsra_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    
    Hector::endCapture();
}

#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <arith.h>
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

// Rounding modes:

enum Rmode {RNU, RNE, RDN, ROD};

template<typename UT>
static inline UT roundoff(UT v, UT vavg, ui2 Rmode, int d)
{
  UT res;
  bool r = false;
  int  dm1 = d - 1;
  UT lsb = UT(1) << d;
  UT lsb_half = lsb >> 1;
  switch (Rmode) {
    case RNU:
      r = (((v >> dm1) & 0x1) != 0);
      break;
    case RNE:
      if (d == 1) {
         r = (((v >> dm1) & 0x1) != 0) & (((v >> d) & 0x1) != 0);
      } else {
         r = (((v >> dm1) & 0x1) != 0) & (((v & (lsb_half - 1)) != 0) | (((v >> d) & 0x1) != 0));
      }
      break;
    case RDN:
      r = false;
      break;
    case ROD:
      r = (((v >> d) & 0x1) == 0) & ((v & (lsb - 1)) != 0);
      break;
  }
  res = vavg + r;
  return res;
}

ui128 rv_vmulhu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr = 0;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(8,ui8(0xFF));
              } else {
                 vd_arr.set_slc(8,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*8,vd_arr.slc<8>(8));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr = 0;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(16,ui16(0xFFFF));
              } else {
                 vd_arr.set_slc(16,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*16,vd_arr.slc<16>(16));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr = 0;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(32,ui32(0xFFFFFFFF));
              } else {
                 vd_arr.set_slc(32,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*32,vd_arr.slc<32>(32));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr = 0;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(64,ui64(0xFFFFFFFFFFFFFFFF));
              } else {
                 vd_arr.set_slc(64,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*64,vd_arr.slc<64>(64));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmulhu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr = 0;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(8,ui8(0xFF));
              } else {
                 vd_arr.set_slc(8,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*8,vd_arr.slc<8>(8));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr = 0;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(16,ui16(0xFFFF));
              } else {
                 vd_arr.set_slc(16,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*16,vd_arr.slc<16>(16));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr = 0;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(32,ui32(0xFFFFFFFF));
              } else {
                 vd_arr.set_slc(32,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*32,vd_arr.slc<32>(32));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr = 0;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(64,ui64(0xFFFFFFFFFFFFFFFF));
              } else {
                 vd_arr.set_slc(64,oldvd_arr);
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
           }
           vd.set_slc(i*64,vd_arr.slc<64>(64));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmul_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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

ui128 rv_vmul_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs2_arr * vs1_arr;
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

ui128 rv_vmulhsu_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr = 0;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(8,ui8(0xFF));
              } else {
                 vd_arr.set_slc(8,oldvd_arr);
              }
           } else {
              vd_arr   = si8(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*8,vd_arr.slc<8>(8));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr = 0;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(16,ui16(0xFFFF));
              } else {
                 vd_arr.set_slc(16,oldvd_arr);
              }
           } else {
              vd_arr   = si16(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*16,vd_arr.slc<16>(16));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr = 0;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(32,ui32(0xFFFFFFFF));
              } else {
                 vd_arr.set_slc(32,oldvd_arr);
              }
           } else {
              vd_arr   = si32(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*32,vd_arr.slc<32>(32));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr = 0;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(64,ui64(0xFFFFFFFFFFFFFFFF));
              } else {
                 vd_arr.set_slc(64,oldvd_arr);
              }
           } else {
              vd_arr   = si64(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*64,vd_arr.slc<64>(64));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmulhsu_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr = 0;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(8,ui8(0xFF));
              } else {
                 vd_arr.set_slc(8,oldvd_arr);
              }
           } else {
              vd_arr   = si8(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*8,vd_arr.slc<8>(8));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr = 0;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(16,ui16(0xFFFF));
              } else {
                 vd_arr.set_slc(16,oldvd_arr);
              }
           } else {
              vd_arr   = si16(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*16,vd_arr.slc<16>(16));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr = 0;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(32,ui32(0xFFFFFFFF));
              } else {
                 vd_arr.set_slc(32,oldvd_arr);
              }
           } else {
              vd_arr   = si32(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*32,vd_arr.slc<32>(32));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr = 0;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(64,ui64(0xFFFFFFFFFFFFFFFF));
              } else {
                 vd_arr.set_slc(64,oldvd_arr);
              }
           } else {
              vd_arr   = si64(vs2_arr) * vs1_arr;
           }
           vd.set_slc(i*64,vd_arr.slc<64>(64));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmulh_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr = 0;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(8,ui8(0xFF));
              } else {
                 vd_arr.set_slc(8,oldvd_arr);
              }
           } else {
              vd_arr   = si8(vs2_arr) * si8(vs1_arr);
           }
           vd.set_slc(i*8,vd_arr.slc<8>(8));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr = 0;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(16,ui16(0xFFFF));
              } else {
                 vd_arr.set_slc(16,oldvd_arr);
              }
           } else {
              vd_arr   = si16(vs2_arr) * si8(vs1_arr);
           }
           vd.set_slc(i*16,vd_arr.slc<16>(16));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr = 0;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(32,ui32(0xFFFFFFFF));
              } else {
                 vd_arr.set_slc(32,oldvd_arr);
              }
           } else {
              vd_arr   = si32(vs2_arr) * si32(vs1_arr);
           }
           vd.set_slc(i*32,vd_arr.slc<32>(32));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr = 0;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(64,ui64(0xFFFFFFFFFFFFFFFF));
              } else {
                 vd_arr.set_slc(64,oldvd_arr);
              }
           } else {
              vd_arr   = si64(vs2_arr) * si64(vs1_arr);
           }
           vd.set_slc(i*64,vd_arr.slc<64>(64));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

ui128 rv_vmulh_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr = 0;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(8,ui8(0xFF));
              } else {
                 vd_arr.set_slc(8,oldvd_arr);
              }
           } else {
              vd_arr   = si8(vs2_arr) * si8(vs1_arr);
           }
           vd.set_slc(i*8,vd_arr.slc<8>(8));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr = 0;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(16,ui16(0xFFFF));
              } else {
                 vd_arr.set_slc(16,oldvd_arr);
              }
           } else {
              vd_arr   = si16(vs2_arr) * si16(vs1_arr);
           }
           vd.set_slc(i*16,vd_arr.slc<16>(16));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr = 0;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(32,ui32(0xFFFFFFFF));
              } else {
                 vd_arr.set_slc(32,oldvd_arr);
              }
           } else {
              vd_arr   = si32(vs2_arr) * si32(vs1_arr);
           }
           vd.set_slc(i*32,vd_arr.slc<32>(32));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr = 0;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr.set_slc(64,ui64(0xFFFFFFFFFFFFFFFF));
              } else {
                 vd_arr.set_slc(64,oldvd_arr);
              }
           } else {
              vd_arr   = si64(vs2_arr) * si64(vs1_arr);
           }
           vd.set_slc(i*64,vd_arr.slc<64>(64));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return vd;
}

tuple<ui128, bool> rv_vsmul_vv(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr   = 0;
     ui16 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr   = ((si8(vs1_arr) == si8(vs2_arr)) && (si8(vs1_arr) == si8(0x80)));
              vd_arr    = si8(vs2_arr) * si8(vs1_arr);
              vavg_arr  = si16(vd_arr) >> (8-1);
              vd_arr    = roundoff<ui8>(ui8(vd_arr), ui8(vavg_arr), vxrm, (8-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui8(0x7F));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*8,vd_arr.slc<8>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr   = 0;
     ui32 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr  = ((si16(vs1_arr) == si16(vs2_arr)) && (si16(vs1_arr) == si16(0x8000)));
              vd_arr   = si16(vs2_arr) * si16(vs1_arr);
              vavg_arr = si32(vd_arr) >> (16-1);
              vd_arr    = roundoff<ui16>(ui16(vd_arr), ui16(vavg_arr), vxrm, (16-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui16(0x7FFF));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*16,vd_arr.slc<16>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr   = 0;
     ui64 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr  = ((si32(vs1_arr) == si32(vs2_arr)) && (si32(vs1_arr) == si32(0x80000000)));
              vd_arr   = si32(vs2_arr) * si32(vs1_arr);
              vavg_arr = si64(vd_arr) >> (32-1);
              vd_arr    = roundoff<ui32>(ui32(vd_arr), ui32(vavg_arr), vxrm, (32-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui32(0x7FFFFFFF));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*32,vd_arr.slc<32>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr   = 0;
     ui128 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr  = ((si64(vs1_arr) == si64(vs2_arr)) && (si64(vs1_arr) == si64(0x8000000000000000)));
              vd_arr   = si64(vs2_arr) * si64(vs1_arr);
              vavg_arr = si128(vd_arr) >> (64-1);
              vd_arr    = roundoff<ui64>(ui64(vd_arr), ui64(vavg_arr), vxrm, (64-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui64(0x7FFFFFFFFFFFFFFF));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr.slc<64>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128,bool>(vd, sat);
}

tuple<ui128, bool> rv_vsmul_vx(ui128 vs1, ui128 vs2, ui128 vm, ui2 vxrm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  bool  sat = false;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr   = 0;
     ui16 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr   = ((si8(vs1_arr) == si8(vs2_arr)) && (si8(vs1_arr) == si8(0x80)));
              vd_arr    = si8(vs2_arr) * si8(vs1_arr);
              vavg_arr  = si16(vd_arr) >> (8-1);
              vd_arr    = roundoff<ui8>(ui8(vd_arr), ui8(vavg_arr), vxrm, (8-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui8(0x7F));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*8,vd_arr.slc<8>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*8,oldvd_arr);
        } else {
           vd.set_slc(i*8,ui8(0xFF));
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr   = 0;
     ui32 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr  = ((si16(vs1_arr) == si16(vs2_arr)) && (si16(vs1_arr) == si16(0x8000)));
              vd_arr   = si16(vs2_arr) * si16(vs1_arr);
              vavg_arr = si32(vd_arr) >> (16-1);
              vd_arr    = roundoff<ui16>(ui16(vd_arr), ui16(vavg_arr), vxrm, (16-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui16(0x7FFF));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*16,vd_arr.slc<16>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*16,oldvd_arr);
        } else {
           vd.set_slc(i*16,ui16(0xFFFF));
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui64 vd_arr   = 0;
     ui64 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr  = ((si32(vs1_arr) == si32(vs2_arr)) && (si32(vs1_arr) == si32(0x80000000)));
              vd_arr   = si32(vs2_arr) * si32(vs1_arr);
              vavg_arr = si64(vd_arr) >> (32-1);
              vd_arr    = roundoff<ui32>(ui32(vd_arr), ui32(vavg_arr), vxrm, (32-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui32(0x7FFFFFFF));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*32,vd_arr.slc<32>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*32,oldvd_arr);
        } else {
           vd.set_slc(i*32,ui32(0xFFFFFFFF));
        }
     }
  }
  if (vsew == 0x3) {
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui128 vd_arr   = 0;
     ui128 vavg_arr = 0;
     bool sat_arr = false;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              sat_arr  = ((si64(vs1_arr) == si64(vs2_arr)) && (si64(vs1_arr) == si64(0x8000000000000000)));
              vd_arr   = si64(vs2_arr) * si64(vs1_arr);
              vavg_arr = si128(vd_arr) >> (64-1);
              vd_arr    = roundoff<ui64>(ui64(vd_arr), ui64(vavg_arr), vxrm, (64-1));
              if (sat_arr) {
                 vd_arr.set_slc(0,ui64(0x7FFFFFFFFFFFFFFF));
              }
              sat      |= sat_arr;
           }
           vd.set_slc(i*64,vd_arr.slc<64>(0));
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i*64,oldvd_arr);
        } else {
           vd.set_slc(i*64,ui64(0xFFFFFFFFFFFFFFFF));
        }
     }
  }
  return tuple<ui128,bool>(vd, sat);
}

ui128 rv_vmadd_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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

ui128 rv_vmadd_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = oldvd_arr * vs1_arr + vs2_arr;
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

ui128 rv_vnmsub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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

ui128 rv_vnmsub_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(oldvd_arr * vs1_arr) + vs2_arr;
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

ui128 rv_vmacc_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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

ui128 rv_vmacc_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = vs1_arr * vs2_arr + oldvd_arr;
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

ui128 rv_vnmsac_vv(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(i*8);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(i*16);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(i*32);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(i*64);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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

ui128 rv_vnmsac_vx(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd  = 0;
  if (vsew == 0x0) {
     ui8  vs1_arr, vs2_arr, oldvd_arr;
     ui8  vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs1_arr   = vs1.slc<8>(0);
        vs2_arr   = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui8(0xFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
     ui16 vs1_arr, vs2_arr, oldvd_arr;
     ui16 vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs1_arr   = vs1.slc<16>(0);
        vs2_arr   = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui16(0xFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
     ui32 vs1_arr, vs2_arr, oldvd_arr;
     ui32 vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs1_arr   = vs1.slc<32>(0);
        vs2_arr   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui32(0xFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
     ui64  vs1_arr, vs2_arr, oldvd_arr;
     ui64  vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs1_arr   = vs1.slc<64>(0);
        vs2_arr   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              vd_arr   = -(vs1_arr * vs2_arr) + oldvd_arr;
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
    if((funct6 == 0x24) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vmulhu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x24) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vmulhu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x25) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vmul_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x25) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vmul_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x26) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vmulhsu_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x26) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vmulhsu_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x27) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vmulh_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x27) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vmulh_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x27) && (funct3 == 0x0) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsmul_vv(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x27) && (funct3 == 0x4) && (Widden == 0x0)) { tie(vd,vxsat) = rv_vsmul_vx(vs1, vs2, vm, vxrm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x29) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vmadd_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x29) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vmadd_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2B) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vnmsub_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2B) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vnmsub_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2D) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vmacc_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2D) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vmacc_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2F) && (funct3 == 0x2) && (Widden == 0x0)) { vd = rv_vnmsac_vv(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x2F) && (funct3 == 0x6) && (Widden == 0x0)) { vd = rv_vnmsac_vx(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    
    Hector::endCapture();
}

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

//ui128 rv_vcpop_m(ui128 vs2, ui128 vm, ui8 vl, bool Mask) {
//  ui128 vd = 0;
//  for (uint i = 0; i < 128; i++) {
//     if (i < vl) {
//        if (Mask) {
//           vd += (vs2[i] && vm[i]);
//        } else {
//           vd += vs2[i];
//        }
//     }
//  }
//  return vd;
//}

ui128 rv_vcpop_m(ui128 vs2, ui128 vm, ui8 vl, ui3 vsew) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     for (uint i = 0; i < 16; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  if (vsew == 0x1) {
     for (uint i = 0; i < 8; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  if (vsew == 0x2) {
     for (uint i = 0; i < 4; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  if (vsew == 0x3) {
     for (uint i = 0; i < 2; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  return vd;
}

ui128 rv_vfirst_m(ui128 vs2, ui128 vm, ui8 vl, bool Mask) {
  ui128 vd = 0;
  int first = -1;
  bool flag = 1;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] && (vm[i] | !Mask)) && flag) {
           first = i;
           flag  = 0;
        };
     }
  }
  vd = ui64(first);
  return vd;
}

ui128 rv_vmsbf_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask) {
  ui128 vd = 0;
  bool flag = 0;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] & (vm[i] | !Mask)) || flag) {
           vd[i] = 0;
           flag  = 1;
        } else {
           vd[i] = 1;
        }
        if ((Mask == 1) && (vm[i] == 0)) {
           vd[i] = vma | oldvd[i];
        }
     } else if (vl == 0) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmsof_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask) {
  ui128 vd = 0;
  bool flag = 1;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] & (vm[i] | !Mask)) && flag) {
           vd[i] = 1;
           flag  = 0;
        } else {
           vd[i] = 0;
        }
        if ((Mask == 1) && (vm[i] == 0)) {
           vd[i] = vma | oldvd[i];
        }
     } else if (vl == 0) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmsif_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask) {
  ui128 vd = 0;
  bool flag = 1;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] & (vm[i] | !Mask)) && flag) {
           vd[i] = 1;
           flag  = 0;
        } else if (!flag) {
           vd[i] = 0;
        } else {
           vd[i] = 1;
        }
        if ((Mask == 1) && (vm[i] == 0)) {
           vd[i] = vma | oldvd[i];
        }
     } else if (vl == 0) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmandn_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] & !vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmand_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] & vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] | vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmxor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] ^ vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmorn_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] | !vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmnand_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = !(vs2[i] & vs1[i]);
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmnor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = !(vs2[i] | vs1[i]);
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmxnor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = !(vs2[i] ^ vs1[i]);
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm;
    ui5 imm;
    ui128 vd;
   
    bool Mask, vma, vta; 
    ui6 funct6;
    ui3 funct3, vsew;
    ui8 vl;
    ui7 vstart;

    Hector::registerInput("vs1", vs1);
    Hector::registerInput("vs2", vs2);
    Hector::registerInput("oldvd", oldvd);
    Hector::registerInput("vm", vm);
    Hector::registerInput("imm", imm);
    Hector::registerInput("Mask", Mask);
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vsew", vsew);
    
    Hector::registerOutput("vd", vd);
    
    Hector::beginCapture();

    vd = 0;
    if((funct6 == 0x10) && (funct3 == 0x2) && (Mask == 0x1) && (vstart == 0x0) && (imm == 0x10)) { vd = rv_vcpop_m(vs2, vm, vl, vsew); }
    if((funct6 == 0x10) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x11)) { vd = rv_vfirst_m(vs2, vm, vl, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x01)) { vd = rv_vmsbf_m(vs2, vm, oldvd, vl, vma, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x02)) { vd = rv_vmsof_m(vs2, vm, oldvd, vl, vma, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x03)) { vd = rv_vmsif_m(vs2, vm, oldvd, vl, vma, Mask); }
    //if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x10)) { vd = rv_viota_m(vs2, vm, vl, Mask); }
    //if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x11)) { vd = rv_vid_m(vs2, vm, vl, Mask); }
    if((funct6 == 0x18) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmandn_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x19) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmand_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1A) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmor_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1B) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmxor_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1C) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmorn_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1D) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmnand_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1E) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmnor_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1F) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmxnor_mm(vs1, vs2, oldvd, vstart, vl); }
    
    Hector::endCapture();
}

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

