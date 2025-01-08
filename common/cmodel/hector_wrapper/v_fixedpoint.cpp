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

