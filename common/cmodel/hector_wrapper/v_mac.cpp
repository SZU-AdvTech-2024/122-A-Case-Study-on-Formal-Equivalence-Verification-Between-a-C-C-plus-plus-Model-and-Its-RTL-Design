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
                 vd_arr  = ui32(0xFFFFFFFF);
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
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
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
                 vd_arr  = ui32(0xFFFFFFFF);
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
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
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

