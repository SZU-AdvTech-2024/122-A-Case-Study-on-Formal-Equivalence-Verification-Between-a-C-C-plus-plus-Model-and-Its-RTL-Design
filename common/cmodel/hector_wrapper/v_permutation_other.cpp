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


//permutation
//vmv_x_s
ui128 rv_vmv_x_s(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
	ui128 rd = 0;
	if(vsew == 0x0){
		ui8 vs2_arr = vs2.slc<8>(0);
		rd = ui128(ui64(si64(si8(vs2_arr))));
	}
        if(vsew == 0x1){
                ui16 vs2_arr = vs2.slc<16>(0);
                rd = ui128(ui64(si64(si16(vs2_arr))));
        }
        if(vsew == 0x2){
                ui32 vs2_arr = vs2.slc<32>(0);
                rd = ui128(ui64(si64(si32(vs2_arr))));
        }
        if(vsew == 0x3){
                ui64 vs2_arr = vs2.slc<64>(0);
                rd = ui128(vs2_arr);
        }
	return rd;
}

//vmv_s_x
ui128 rv_vmv_s_x(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
        ui128 rd = 0;
        if((vl == 0) || (vstart >= vl)){
          rd = oldvd;
        }else{
          if(vsew == 0x0){
                  ui8 vs1_arr = vs1.slc<8>(0);
                  rd.set_slc(0,  vta ? ui64(0xFFFFFFFFFFFFFF00) | ui64(vs1_arr) : (ui64(oldvd) & ui64(0xFFFFFFFFFFFFFF00) | ui64(vs1_arr)));
                  rd.set_slc(64, vta ? ui64(0xFFFFFFFFFFFFFFFF) : ui64(oldvd >> 64));
          }
          if(vsew == 0x1){
                  ui16 vs1_arr = vs1.slc<16>(0);
                  rd.set_slc(0,  vta ? ui64(0xFFFFFFFFFFFF0000) | ui64(vs1_arr) : (ui64(oldvd) & ui64(0xFFFFFFFFFFFF0000) | ui64(vs1_arr)));
                  rd.set_slc(64, vta ? ui64(0xFFFFFFFFFFFFFFFF) : ui64(oldvd >> 64));
          }
          if(vsew == 0x2){
                  ui32 vs1_arr = vs1.slc<32>(0);
                  rd.set_slc(0,  vta ? ui64(0xFFFFFFFF00000000) | ui64(vs1_arr) : (ui64(oldvd) & ui64(0xFFFFFFFF00000000) | ui64(vs1_arr)));
                  rd.set_slc(64, vta ? ui64(0xFFFFFFFFFFFFFFFF) : ui64(oldvd >> 64));
          }
          if(vsew == 0x3){
                  ui64 vs1_arr = vs1.slc<64>(0);
                  rd.set_slc(0,  ui64(vs1_arr));
                  rd.set_slc(64, vta ? ui64(0xFFFFFFFFFFFFFFFF) : ui64(oldvd >> 64));
          }
        }
        return rd;
}

//vfmv_f_s
ui128 rv_vfmv_f_s(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
        ui128 rd = 0;
        if(vsew == 0x2){
                ui32 vs2_arr = vs2.slc<32>(0);
                rd.set_slc(0, vs2_arr);
                rd.set_slc(32, ui32(0xFFFFFFFF));
        }
        if(vsew == 0x3){
                ui64 vs2_arr = vs2.slc<64>(0);
                rd = ui128(vs2_arr);
        }
        return rd;
}

//vfmv_s_f
ui128 rv_vfmv_s_f(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
        ui128 rd = 0;
        if((vl == 0) || (vstart >= vl)){
          rd = oldvd;
        }else{
          if(vsew == 0x2){
                  ui32 vs1_arr = vs1.slc<32>(0);
                  rd.set_slc(0, (vs1.slc<32>(32) == ui32(0xFFFFFFFF)) ? vs1_arr : ui32(0x7FC00000));
                  rd.set_slc(32, vta ? ui32(0xFFFFFFFF) : oldvd.slc<32>(32));
                  rd.set_slc(64, vta ? ui32(0xFFFFFFFF) : oldvd.slc<32>(64));
                  rd.set_slc(96, vta ? ui32(0xFFFFFFFF) : oldvd.slc<32>(96));
          }
          if(vsew == 0x3){
                  ui64 vs1_arr = vs1.slc<64>(0);
                  rd.set_slc(0, vs1_arr);
                  rd.set_slc(64, vta ? ui64(0xFFFFFFFFFFFFFFFF) : oldvd.slc<64>(64));
          }
        }
        return rd;
}

//vmv<nr>r_v
ui128 rv_vmv_v(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if (i >= vstart) {
           vd_arr = vs2_arr;
           vd.set_slc(i*8,vd_arr);
        } else {
           vd.set_slc(i*8,oldvd_arr);
        }
     }
  }
  if (vsew == 0x1) {
     ui16 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        vs2_arr = vs2.slc<16>(i*16);
        oldvd_arr = oldvd.slc<16>(i*16);
        if (i >= vstart) {
           vd_arr = vs2_arr;
           vd.set_slc(i*16,vd_arr);
        } else {
           vd.set_slc(i*16,oldvd_arr);
        }
     }
  }
  if (vsew == 0x2) {
     ui32 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        vs2_arr = vs2.slc<32>(i*32);
        oldvd_arr = oldvd.slc<32>(i*32);
        if (i >= vstart) {
           vd_arr = vs2_arr;
           vd.set_slc(i*32,vd_arr);
        } else {
           vd.set_slc(i*32,oldvd_arr);
        }
     }
  }
  if (vsew == 0x3) {
     ui64 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        vs2_arr = vs2.slc<64>(i*64);
        oldvd_arr = oldvd.slc<64>(i*64);
        if (i >= vstart) {
           vd_arr = vs2_arr;
           vd.set_slc(i*64,vd_arr);
        } else {
           vd.set_slc(i*64,oldvd_arr);
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
    ui3 funct3, vsew, srcType_0;
    ui8 vl;
    ui7 vstart, fuOpType;

    ui5 fuType;

    Hector::registerInput("fuType", fuType);
    Hector::registerInput("fuOpType", fuOpType);
    Hector::registerInput("srcType_0", srcType_0);

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
    Hector::registerInput("vma", vma);
    Hector::registerInput("vta", vta);
    
    Hector::registerOutput("vd", vd);
    
    Hector::beginCapture();

    vd = 0;

    if((fuType == 0x11) && (funct6 == 0x10) && (funct3 == 0x2) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x0)) { vd = rv_vmv_x_s(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((fuType == 0x17) && (funct6 == 0x10) && (funct3 == 0x6) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x0)) { vd = rv_vmv_s_x(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((fuType == 0x11) && (funct6 == 0x10) && (funct3 == 0x1) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x0)) { vd = rv_vfmv_f_s(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((fuType == 0x17) && (funct6 == 0x10) && (funct3 == 0x5) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x0)) { vd = rv_vfmv_s_f(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    if((fuType == 0x11) && (funct6 == 0x27) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x0)) { vd = rv_vmv_v(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((fuType == 0x11) && (funct6 == 0x27) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x1)) { vd = rv_vmv_v(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((fuType == 0x11) && (funct6 == 0x27) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x3)) { vd = rv_vmv_v(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }
    if((fuType == 0x11) && (funct6 == 0x27) && (funct3 == 0x3) && (Narrow == 0x0) && (Widden == 0x0) && (imm == 0x7)) { vd = rv_vmv_v(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    Hector::endCapture();
}

