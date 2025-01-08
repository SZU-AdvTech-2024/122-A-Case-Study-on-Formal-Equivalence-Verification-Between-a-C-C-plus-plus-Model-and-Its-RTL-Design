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
typedef ac_int<128, true> si128;

//ui128 rv_vcpop_m(ui128 vs2, ui128 vm, ui8 vl, bool Mask) {
//  ui128 vd = 0;
//  for (uint i = 0; i < 128; i++) {
//     if (i < vl) {
//        if (Mask) {
//           vd += (vs2[i] && vm[i]);
//        } else {
//           vd += vs2[i];
//        }
//     }
//  }
//  return vd;
//}

ui128 rv_vcpop_m(ui128 vs2, ui128 vm, ui8 vl, ui3 vsew) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     for (uint i = 0; i < 16; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  if (vsew == 0x1) {
     for (uint i = 0; i < 8; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  if (vsew == 0x2) {
     for (uint i = 0; i < 4; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  if (vsew == 0x3) {
     for (uint i = 0; i < 2; i++) {
        if (i < vl) {
           vd += (vs2[i] && vm[i]);
        }
     }
  }
  return vd;
}

ui128 rv_vfirst_m(ui128 vs2, ui128 vm, ui8 vl, bool Mask) {
  ui128 vd = 0;
  int first = -1;
  bool flag = 1;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] && (vm[i] | !Mask)) && flag) {
           first = i;
           flag  = 0;
        };
     }
  }
  vd = ui64(first);
  return vd;
}

ui128 rv_vmsbf_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask) {
  ui128 vd = 0;
  bool flag = 0;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] & (vm[i] | !Mask)) || flag) {
           vd[i] = 0;
           flag  = 1;
        } else {
           vd[i] = 1;
        }
        if ((Mask == 1) && (vm[i] == 0)) {
           vd[i] = vma | oldvd[i];
        }
     } else if (vl == 0) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmsof_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask) {
  ui128 vd = 0;
  bool flag = 1;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] & (vm[i] | !Mask)) && flag) {
           vd[i] = 1;
           flag  = 0;
        } else {
           vd[i] = 0;
        }
        if ((Mask == 1) && (vm[i] == 0)) {
           vd[i] = vma | oldvd[i];
        }
     } else if (vl == 0) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmsif_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask) {
  ui128 vd = 0;
  bool flag = 1;
  for (uint i = 0; i < 128; i++) {
     if (i < vl) {
        if ((vs2[i] & (vm[i] | !Mask)) && flag) {
           vd[i] = 1;
           flag  = 0;
        } else if (!flag) {
           vd[i] = 0;
        } else {
           vd[i] = 1;
        }
        if ((Mask == 1) && (vm[i] == 0)) {
           vd[i] = vma | oldvd[i];
        }
     } else if (vl == 0) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmandn_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] & !vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmand_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] & vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] | vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmxor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] ^ vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmorn_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = vs2[i] | !vs1[i];
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmnand_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = !(vs2[i] & vs1[i]);
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmnor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = !(vs2[i] | vs1[i]);
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_vmxnor_mm(ui128 vs1, ui128 vs2, ui128 oldvd, ui7 vstart, ui8 vl) {
  ui128 vd = 0;
  for (uint i = 0; i < 128; i++) {
     if ((i >= vstart) && (i < vl)) {
        vd[i] = !(vs2[i] ^ vs1[i]);
     } else if ((i < vstart) || (vstart >= vl)) {
        vd[i] = oldvd[i];
     } else {
        vd[i] = 1;
     }
  }
  return vd;
}

ui128 rv_viota_m(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask, ui3 vsew, ui7 vstart, bool vta) {
  ui128 vd = 0;
  ui128 count = 0;
  if (vsew == 0x0) {
     ui8 oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if(!Mask){
             if(vs2[i]){
               vd_arr = (ui8)count;
               count++;
             }else{
               vd_arr = (ui8)count;
             }
           }else{
               if(vs2[i]&vm[i]){
                 vd_arr = (ui8)count;
                 count++;
               }
               if(!vs2[i]&vm[i]){
                 vd_arr = (ui8)count;
               }
               if(!vm[i]&vma){
                 vd_arr = ui8(0xFF);
               }
               if(!vm[i]&!vma){
                 vd_arr = oldvd_arr;
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
     ui16 oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        oldvd_arr = oldvd.slc<16>(i*16); 
        if ((i >= vstart) && (i < vl)) {
           if(!Mask){
             if(vs2[i]){
               vd_arr = (ui16)count;
               count++;
             }else{
               vd_arr = (ui16)count;
             }
           }else{
               if(vs2[i]&vm[i]){
                 vd_arr = (ui16)count;
                 count++;
               }
               if(!vs2[i]&vm[i]){
                 vd_arr = (ui16)count;
               }
               if(!vm[i]&vma){
                 vd_arr = ui16(0xFFFF);
               }
               if(!vm[i]&!vma){
                 vd_arr = oldvd_arr;
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
     ui32 oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        oldvd_arr = oldvd.slc<32>(i*32); 
        if ((i >= vstart) && (i < vl)) {
           if(!Mask){
             if(vs2[i]){
               vd_arr = (ui32)count;
               count++;
             }else{
               vd_arr = (ui32)count;
             }
           }else{
               if(vs2[i]&vm[i]){
                 vd_arr = (ui32)count;
                 count++;
               }
               if(!vs2[i]&vm[i]){
                 vd_arr = (ui32)count;
               }
               if(!vm[i]&vma){
                 vd_arr = ui32(0xFFFFFFFF);
               }
               if(!vm[i]&!vma){
                 vd_arr = oldvd_arr;
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
     ui64 oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        oldvd_arr = oldvd.slc<64>(i*64); 
        if ((i >= vstart) && (i < vl)) {
           if(!Mask){
             if(vs2[i]){
               vd_arr = (ui64)count;
               count++;
             }else{
               vd_arr = (ui64)count;
             }
           }else{
               if(vs2[i]&vm[i]){
                 vd_arr = (ui64)count;
                 count++;
               }
               if(!vs2[i]&vm[i]){
                 vd_arr = (ui64)count;
               }
               if(!vm[i]&vma){
                 vd_arr = ui64(0xFFFFFFFFFFFFFFFF);
               }
               if(!vm[i]&!vma){
                 vd_arr = oldvd_arr;
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

ui128 rv_vid_v(ui128 vs2, ui128 vm, ui128 oldvd, ui8 vl, bool vma, bool Mask, ui3 vsew, ui7 vstart, bool vta) {
  ui128 vd = 0;
  if (vsew == 0x0) {
     ui8 oldvd_arr, vd_arr;
     for (uint i = 0; i < 16; i++) {
        oldvd_arr = oldvd.slc<8>(i*8);
        if ((i >= vstart) && (i < vl)) {
           if(Mask & !vm[i]& !vma){
             vd_arr = oldvd_arr;
           }else if(Mask & !vm[i]& vma){
             vd_arr = ui8(0xFF);
           }else{
             vd_arr = (ui8)i;
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
     ui16 oldvd_arr, vd_arr;
     for (uint i = 0; i < 8; i++) {
        oldvd_arr = oldvd.slc<16>(i*16);
        if ((i >= vstart) && (i < vl)) {
           if(Mask & !vm[i]& !vma){
             vd_arr = oldvd_arr; 
           }else if(Mask & !vm[i]& vma){
             vd_arr = ui16(0xFFFF);
           }else{
             vd_arr = (ui16)i;
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
     ui32 oldvd_arr, vd_arr;
     for (uint i = 0; i < 4; i++) {
        oldvd_arr = oldvd.slc<32>(i*32);
        if ((i >= vstart) && (i < vl)) {
           if(Mask & !vm[i]& !vma){
             vd_arr = oldvd_arr;
           }else if(Mask & !vm[i]& vma){
             vd_arr = ui32(0xFFFFFFFF);
           }else{
             vd_arr = (ui32)i;
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
     ui64 oldvd_arr, vd_arr;
     for (uint i = 0; i < 2; i++) {
        oldvd_arr = oldvd.slc<64>(i*64);
        if ((i >= vstart) && (i < vl)) {
           if(Mask & !vm[i]& !vma){
             vd_arr = oldvd_arr;
           }else if(Mask & !vm[i]& vma){
             vd_arr = ui64(0xFFFFFFFFFFFFFFFF);
           }else{
             vd_arr = (ui64)i;
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
   
    bool Mask, vma, vta; 
    ui6 funct6;
    ui3 funct3, vsew;
    ui8 vl;
    ui7 vstart;

    Hector::registerInput("vs1", vs1);
    Hector::registerInput("vs2", vs2);
    Hector::registerInput("oldvd", oldvd);
    Hector::registerInput("vm", vm);
    Hector::registerInput("imm", imm);
    Hector::registerInput("Mask", Mask);
    Hector::registerInput("vstart", vstart);
    Hector::registerInput("vma", vma);
    Hector::registerInput("vta", vta);
    Hector::registerInput("funct6", funct6);
    Hector::registerInput("funct3", funct3);
    Hector::registerInput("vl", vl);
    Hector::registerInput("vsew", vsew);
    
    Hector::registerOutput("vd", vd);
    
    Hector::beginCapture();

    vd = 0;
    if((funct6 == 0x10) && (funct3 == 0x2) && (Mask == 0x1) && (vstart == 0x0) && (imm == 0x10)) { vd = rv_vcpop_m(vs2, vm, vl, vsew); }
    if((funct6 == 0x10) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x11)) { vd = rv_vfirst_m(vs2, vm, vl, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x01)) { vd = rv_vmsbf_m(vs2, vm, oldvd, vl, vma, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x02)) { vd = rv_vmsof_m(vs2, vm, oldvd, vl, vma, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x03)) { vd = rv_vmsif_m(vs2, vm, oldvd, vl, vma, Mask); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x10)) { vd = rv_viota_m( vs2, vm, oldvd, vl, vma, Mask, vsew, vstart, vta); }
    if((funct6 == 0x14) && (funct3 == 0x2) && (vstart == 0x0) && (imm == 0x11)) { vd = rv_vid_v( vs2, vm, oldvd, vl, vma, Mask, vsew, vstart, vta); }
    if((funct6 == 0x18) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmandn_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x19) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmand_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1A) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmor_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1B) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmxor_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1C) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmorn_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1D) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmnand_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1E) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmnor_mm(vs1, vs2, oldvd, vstart, vl); }
    if((funct6 == 0x1F) && (funct3 == 0x2) && (Mask == 0x0)) { vd = rv_vmxnor_mm(vs1, vs2, oldvd, vstart, vl); }
    
    Hector::endCapture();
}

