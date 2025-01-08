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

//Widden Begin
tuple<ui128, ui5> rv_vfwcvt_f_bf16_v(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  ui32 vd_arr, oldvd_arr;
  float32_t f_vd;
  bfloat16_t f_vs2;
  for (uint i = 0; i < 4; i++) {
     f_vs2.v   = vs2.slc<16>(i*16);
     oldvd_arr = oldvd.slc<32>(i*32);
     if ((i >= vstart) && (i < vl)) {
        if ((Mask == 1) && (vm[i] == 0)) {
           if (vma) {
              vd_arr  = ui32(0xFFFFFFFF);
           } else {
              vd_arr  = oldvd_arr;
           }
        } else {
           f_vd = bf16_to_f32(f_vs2);
           vd_arr = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
        }
        vd.set_slc(i*32,vd_arr);
     } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
        vd.set_slc(i*32,oldvd_arr);
     } else {
        vd.set_slc(i*32,ui32(0xFFFFFFFF));
     }
  }
  return tuple<ui128, ui5>(vd, fflags);
}

//Narrow Begin
//rv_vfncvt_bf16_f_w
tuple<ui128, ui5> rv_vfncvt_bf16_f_w(ui128 vs1, ui128 vs2, ui128 vm, ui3 frm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd   = 0;
  ui5 fflags = 0;
  softfloat_roundingMode = frm;
  softfloat_exceptionFlags = 0;
  softfloat_detectTininess = softfloat_tininess_afterRounding;
  ui16 vd_arr, oldvd_arr;
  float32_t f_vs2;
  bfloat16_t f_vd;
  for (uint i = 0; i < 4; i++) {
     f_vs2.v   = vs2.slc<32>(i*32);
     oldvd_arr = oldvd.slc<16>(i*16);
     if ((i >= vstart) && (i < vl)) {
        if ((Mask == 1) && (vm[i] == 0)) {
           if (vma) {
              vd_arr  = ui16(0xFFFF);
           } else {
              vd_arr  = oldvd_arr;
           }
        } else {
           f_vd = f32_to_bf16(f_vs2);
           vd_arr = f_vd.v;
           fflags |= (softfloat_exceptionFlags & 0x1F);
        }
        vd.set_slc(i*16,vd_arr);
     } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
        vd.set_slc(i*16,oldvd_arr);
     } else {
        vd.set_slc(i*16,ui16(0xFFFF));
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
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x1) && (MaskOp == 0x0) && (imm == 0x0D) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfwcvt_f_bf16_v(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((funct6 == 0x12) && (funct3 == 0x1) && (Narrow == 0x1) && (Widden == 0x0) && (MaskOp == 0x0) && (imm == 0x1D) && (eewType_1 == 0x3)) { tie(vd,fflags) = rv_vfncvt_bf16_f_w(vs1, vs2, vm, frm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    Hector::endCapture();
}

