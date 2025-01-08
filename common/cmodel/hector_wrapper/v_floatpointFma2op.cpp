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

