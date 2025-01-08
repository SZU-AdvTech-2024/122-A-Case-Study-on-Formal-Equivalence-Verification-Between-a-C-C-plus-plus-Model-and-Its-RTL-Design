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

