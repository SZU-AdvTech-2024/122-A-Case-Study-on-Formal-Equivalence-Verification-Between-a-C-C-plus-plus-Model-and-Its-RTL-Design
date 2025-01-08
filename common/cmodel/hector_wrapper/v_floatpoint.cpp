#include "Hector.h"
#include "softfloat.h"
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

//float32_t f32_class( float32_t a ) {
//  uint32_t res = 0;
//  float32_t fd;
//  bool isNegative  = ((a.v & 0x80000000) == 0x80000000);
//  bool isNormal    = ((a.v & 0x7F800000) && ((a.v & 0x7F800000) != 0x7F800000));
//  bool isSubNormal = (((a.v & 0x7F800000) == 0x00000000) && (a.v & 0x007FFFFF));
//
//  bool isNInf       = (a.v == 0xFF800000);
//  bool isNNormal    = isNormal && isNegative;
//  bool isNSubNormal = isSubNormal && isNegative;
//  bool isNZero      = (a.v == 0x80000000);
//  bool isPZero      = (a.v == 0x00000000);
//  bool isPSubNormal = isSubNormal && !isNegative;
//  bool isPNormal    = isNormal && !isNegative;
//  bool isPInf       = (a.v == 0x7F800000);
//  bool isSNaN       = (((a.v & 0x7FC00000) == 0x7F800000) && (a.v & 0x003FFFFF));
//  bool isQNaN       = ((a.v & 0x7FC00000) == 0x7FC00000);
//  if(isNInf)       res = 0b0000000001;
//  if(isNNormal)    res = 0b0000000010;
//  if(isNSubNormal) res = 0b0000000100;
//  if(isNZero)      res = 0b0000001000;
//  if(isPZero)      res = 0b0000010000;
//  if(isPSubNormal) res = 0b0000100000;
//  if(isPNormal)    res = 0b0001000000;
//  if(isPInf)       res = 0b0010000000;
//  if(isSNaN)       res = 0b0100000000;
//  if(isQNaN)       res = 0b1000000000;
//  fd.v = res;
//  return fd;
//}

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

//float64_t f64_class( float64_t a ) {
//  uint64_t res = 0;
//  float64_t fd;
//  bool isNegative  = ((a.v & 0x8000000000000000) == 0x8000000000000000);
//  bool isNormal    = ((a.v & 0x7FF0000000000000) && ((a.v & 0x7FF0000000000000) != 0x7FF0000000000000));
//  bool isSubNormal = (((a.v & 0x7FF0000000000000) == 0x0000000000000000) && (a.v & 0x000FFFFFFFFFFFFF));
//
//  bool isNInf       = (a.v == 0xFFF0000000000000);
//  bool isNNormal    = isNormal && isNegative;
//  bool isNSubNormal = isSubNormal && isNegative;
//  bool isNZero      = (a.v == 0x8000000000000000);
//  bool isPZero      = (a.v == 0x0000000000000000);
//  bool isPSubNormal = isSubNormal && !isNegative;
//  bool isPNormal    = isNormal && !isNegative;
//  bool isPInf       = (a.v == 0x7FF0000000000000);
//  bool isSNaN       = (((a.v & 0x7FF8000000000000) == 0x7FF0000000000000) && (a.v & 0x0007FFFFFFFFFFFF));
//  bool isQNaN       = ((a.v & 0x7FF8000000000000) == 0x7FF8000000000000);
//  if(isNInf)       res = 0b0000000001;
//  if(isNNormal)    res = 0b0000000010;
//  if(isNSubNormal) res = 0b0000000100;
//  if(isNZero)      res = 0b0000001000;
//  if(isPZero)      res = 0b0000010000;
//  if(isPSubNormal) res = 0b0000100000;
//  if(isPNormal)    res = 0b0001000000;
//  if(isPInf)       res = 0b0010000000;
//  if(isSNaN)       res = 0b0100000000;
//  if(isQNaN)       res = 0b1000000000;
//  fd.v = res;
//  return fd;
//}

tuple<ui128, ui5> rv_vfadd_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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

tuple<ui128, ui5> rv_vfmin_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
           f_vd    = f32_min(f_vs2, f_vs1);
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
           f_vd    = f64_min(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfmin_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  Hector::show("fflags", fflags);
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_min(f_vs2, f_vs1);
           vd_arr  = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
           Hector::show("fflags", fflags);
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
           f_vd    = f64_min(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfmax_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
           f_vd    = f32_max(f_vs2, f_vs1);
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
           f_vd    = f64_max(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfmax_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_max(f_vs2, f_vs1);
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
           f_vd    = f64_max(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfsgnj_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
           f_vd    = f32_sgnj(f_vs2, f_vs1);
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
           f_vd    = f64_sgnj(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfsgnj_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_sgnj(f_vs2, f_vs1);
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
           f_vd    = f64_sgnj(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfsgnjn_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
           f_vd    = f32_sgnjn(f_vs2, f_vs1);
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
           f_vd    = f64_sgnjn(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfsgnjn_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_sgnjn(f_vs2, f_vs1);
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
           f_vd    = f64_sgnjn(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfsgnjx_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
           f_vd    = f32_sgnjx(f_vs2, f_vs1);
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
           f_vd    = f64_sgnjx(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfsgnjx_vv(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs1, f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs1.v   = vs1.slc<32>(i*32);
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_sgnjx(f_vs2, f_vs1);
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
           f_vd    = f64_sgnjx(f_vs2, f_vs1);
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

tuple<ui128, ui5> rv_vfrsqrt7_v(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_rsqrte7(f_vs2);
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
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_rsqrte7(f_vs2);
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

tuple<ui128, ui5> rv_vfrec7_v(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2, f_vd;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f32_recip7(f_vs2);
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
     float64_t f_vs2, f_vd;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           f_vd    = f64_recip7(f_vs2);
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

tuple<ui128, ui5> rv_vfclass_v(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
  if (vsew == 0x2) {
     ui32 vd_arr, oldvd_arr;
     float32_t f_vs2;
     for (uint i = 0; i < 4; i++) {
        f_vs2.v   = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           vd_arr  = f32_classify(f_vs2);
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
     float64_t f_vs2;
     for (uint i = 0; i < 2; i++) {
        f_vs2.v   = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           vd_arr  = f64_classify(f_vs2);
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

tuple<ui128, ui5> rv_vfmv_v_f(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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

tuple<ui128, ui5> rv_vfrsub_vf(ui128 vs1, ui128 vs2, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vta) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_beforeRounding;
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
   
    bool Narrow , Widden, Mask, vma, vta; 
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
    if((funct6 == 0x00) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfadd_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x00) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfadd_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x02) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x02) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsub_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x04) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmin_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x04) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmin_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x06) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmax_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x06) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmax_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x08) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsgnj_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x08) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsgnj_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x09) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsgnjn_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x09) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsgnjn_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x0A) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsgnjx_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x0A) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfsgnjx_vv(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x13) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0) && (imm == 0x04)) { tie(vd,fflags) = rv_vfrsqrt7_v(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x13) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0) && (imm == 0x05)) { tie(vd,fflags) = rv_vfrec7_v(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x13) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0) && (imm == 0x10)) { tie(vd,fflags) = rv_vfclass_v(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfmv_v_f(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x17) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x1)) { tie(vd,fflags) = rv_vfmerge_vfm(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vta); }
    if((funct6 == 0x27) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (Mask == 0x0)) { tie(vd,fflags) = rv_vfrsub_vf(vs1, vs2, frm, oldvd, vstart, vl, vsew, vta); }
    
    Hector::endCapture();
}

