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

//vcompress_vm
ui128 rv_vcompress_vm(ui128 vs1, ui128 vs2, ui128 vm, ui128 oldvd, ui7 vstart, ui8 vl, ui3 vsew, bool vma, bool vta, bool Mask) {
  ui128 vd = 0;
  ui8 index = 0;
  bool run = false;
  if (vsew == 0x0) {
     ui8 vs2_arr, oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        vs2_arr = vs2.slc<8>(i*8);
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if(vm[i]){
              vd_arr = vs2_arr;
              vd.set_slc(index*8,vd_arr);
              index++;
           }
//        } else if ((i < vstart) || ((i >= vl) && (vta == 0)) || (vstart >= vl)) {
        } else if ((i < vstart) || ((i >= vl) && (vta == 0))) {
              vd.set_slc(i*8,oldvd_arr);
        } else {
              vd.set_slc(i*8,ui8(0xFF));
        }
     }
     for(uint i = 0; i < 16; i++){
        oldvd_arr = oldvd.slc<8>(i*8);
        if((i >= index) && (i < vl)){
           if(vta == 0){vd_arr = oldvd_arr;}
           if(vta == 1){vd_arr = ui8(0xFF);}
           vd.set_slc(i*8,vd_arr);
        }
     }
  }
  return vd;
}



void hector_wrapper()
{
    ui128 vs1, vs2, oldvd, vm, rdata, rvalid;
    ui5 imm;
    ui128 vd;
   
    bool Narrow , Widden, Mask, vma, vta; 
    ui6 funct6;
    ui3 funct3, vsew, vlmul;
    ui8 vl;
    ui7 vstart;

//    Hector::registerInput("vs1", vs1);
    Hector::registerInput("vs2", vs2);
    Hector::registerInput("oldvd", oldvd);
    Hector::registerInput("vm", vm);

    Hector::registerInput("Mask", Mask);
    Hector::registerInput("vta", vta);
    Hector::registerInput("vma", vma);

    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vl", vl);
//    Hector::registerInput("vlmul", vlmul);

    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
//    Hector::registerInput("imm", imm);
    Hector::registerInput("vsew", vsew);
    
    Hector::registerOutput("vd", vd);
    
    Hector::beginCapture();

    vd = 0;

    if((funct6 == 0x17) && (funct3 == 0x2) && (vstart == 0x0)) { vd = rv_vcompress_vm(vs1, vs2, vm, oldvd, vstart, vl, vsew, vma, vta, Mask); }

    Hector::endCapture();
}

