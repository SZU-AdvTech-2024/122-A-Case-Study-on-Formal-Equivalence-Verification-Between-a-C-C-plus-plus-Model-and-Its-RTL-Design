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

