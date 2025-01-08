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

//Widden Begin
//rv_vfwadd_vf
tuple<ui128, ui5> rv_vfwadd_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_add(f32_to_f64(f_vs2), f32_to_f64(f_vs1));
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

//rv_vfwadd_vv
tuple<ui128, ui5> rv_vfwadd_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) { 
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_add(f32_to_f64(f_vs2), f32_to_f64(f_vs1));
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

//rv_vfwadd_wf
tuple<ui128, ui5> rv_vfwadd_wf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1;
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
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
              f_vd    = f64_add(f_vs2, f32_to_f64(f_vs1));
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

//rv_vfwadd_wv
tuple<ui128, ui5> rv_vfwadd_wv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1; 
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
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
              f_vd    = f64_add(f_vs2, f32_to_f64(f_vs1));
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

//rv_vfwsub_vf
tuple<ui128, ui5> rv_vfwsub_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sub(f32_to_f64(f_vs2), f32_to_f64(f_vs1));
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

//rv_vfwsub_vv
tuple<ui128, ui5> rv_vfwsub_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui64(0xFFFFFFFFFFFFFFFF);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
              f_vd    = f64_sub(f32_to_f64(f_vs2), f32_to_f64(f_vs1));
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

//rv_vfwsub_wf
tuple<ui128, ui5> rv_vfwsub_wf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1;
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
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
              f_vd    = f64_sub(f_vs2, f32_to_f64(f_vs1));
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

//rv_vfwsub_wv
tuple<ui128, ui5> rv_vfwsub_wv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1;
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
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
              f_vd    = f64_sub(f_vs2, f32_to_f64(f_vs1));
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

//rv_vfwmul_vf
tuple<ui128, ui5> rv_vfwmul_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mul(f32_to_f64(f_vs2), f32_to_f64(f_vs1));
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

//rv_vfwmul_vv
tuple<ui128, ui5> rv_vfwmul_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui64 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     float64_t f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_mul(f32_to_f64(f_vs2), f32_to_f64(f_vs1));
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

//Widden End


//Narrow Begin
//rv_vmfeq_vf
tuple<ui128, ui5> rv_vmfeq_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_eq(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
	oldvd_arr = oldvd.slc<1>(i);
	if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_eq(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfeq_vv
tuple<ui128, ui5> rv_vmfeq_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_eq(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_eq(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfge_vf
tuple<ui128, ui5> rv_vmfge_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_le(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_le(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfgt_vf
tuple<ui128, ui5> rv_vmfgt_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_lt(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_lt(f_vs1, f_vs2);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfle_vf
tuple<ui128, ui5> rv_vmfle_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_le(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_le(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfle_vv
tuple<ui128, ui5> rv_vmfle_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_le(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_le(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmflt_vf
tuple<ui128, ui5> rv_vmflt_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_lt(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_lt(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmflt_vv
tuple<ui128, ui5> rv_vmflt_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_lt(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_lt(f_vs2, f_vs1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfne_vf
tuple<ui128, ui5> rv_vmfne_vf(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        if (vs1.slc<32>(32) == 0xFFFFFFFF) {
           f_vs1.v   = vs1.slc<32>(0);
        } else {
           f_vs1.v   = 0x7FC00000;
        }
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_eq(f_vs1, f_vs2) ^ ui1(0x1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(0);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_eq(f_vs1, f_vs2) ^ ui1(0x1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//rv_vmfne_vv
tuple<ui128, ui5> rv_vmfne_vv(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  if (vsew == 0x2) {
     ui1 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f32_eq(f_vs1, f_vs2) ^ ui1(0x1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 4; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  if (vsew == 0x3) {
     ui1 vd_arr, oldvd_arr;
     float64_t f_vs1, f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs1.v   = vs1.slc<64>(i*64);
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<1>(i);
        if ((i >= vstart) && (i < vl)) {
           if ((Mask == 1) && (vm[i] == 0)) {
              if (vma) {
                 vd_arr  = ui1(0x1);
              } else {
                 vd_arr  = oldvd_arr;
              }
           } else {
             vd_arr = f64_eq(f_vs1, f_vs2) ^ ui1(0x1);
             fflags |= (softfloat_exceptionFlags & 0x1F);
           }
           vd.set_slc(i, vd_arr);
        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
     for (uint i = 2; i < 128; i++) {
        oldvd_arr = oldvd.slc<1>(i);
        if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
           vd.set_slc(i, oldvd_arr);
        } else {
           vd.set_slc(i, ui1(0x1));
        }
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//Narrow End

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
    ui3 funct3, vsew, frm, eewType_1, eewType_2;
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
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vta", vta);
    Hector::registerInput("vsew", vsew);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vxrm", vxrm);
    Hector::registerInput("frm", frm);
    Hector::registerInput("eewType_1", eewType_1);
    Hector::registerInput("eewType_2", eewType_2);
    
    Hector::registerOutput("vd", vd);
    Hector::registerOutput("fflags", fflags);
    
    Hector::beginCapture();

    vd = 0;
    fflags = 0;
    if((funct6 == 0x30) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwadd_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x30) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwadd_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x34) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x4)) { tie(vd,fflags) = rv_vfwadd_wf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x34) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x4)) { tie(vd,fflags) = rv_vfwadd_wv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x32) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwsub_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x32) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwsub_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x36) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x4)) { tie(vd,fflags) = rv_vfwsub_wf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x36) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (eewType_1 == 0x4)) { tie(vd,fflags) = rv_vfwsub_wv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x38) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (Mask == 0x0) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwmul_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x38) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (Mask == 0x0) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwmul_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }

    if((funct6 == 0x18) && (funct3 == 0x5) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfeq_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x18) && (funct3 == 0x1) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfeq_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1F) && (funct3 == 0x5) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfge_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1D) && (funct3 == 0x5) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfgt_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x19) && (funct3 == 0x5) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfle_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x19) && (funct3 == 0x1) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfle_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1B) && (funct3 == 0x5) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmflt_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1B) && (funct3 == 0x1) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmflt_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1C) && (funct3 == 0x5) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfne_vf(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x1C) && (funct3 == 0x1) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x1) && (eewType_2 != 0x3) && (vta == 1)) { tie(vd,fflags) = rv_vmfne_vv(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

//    if((funct6 == 0x00) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfadd_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x00) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfadd_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x02) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x02) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsub_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x24) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmul_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x24) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmul_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
//    if((funct6 == 0x27) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (MaskOp == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfrsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    
    Hector::endCapture();
}

