#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <string>
#include <vector>
using namespace std;

// RAC begin

typedef ac_int<5, false> ui5;
typedef ac_int<6, false> ui6;
typedef ac_int<7, false> ui7;
typedef ac_int<8, false> ui8;
typedef ac_int<32, false> ui32;
typedef ac_int<64, false> ui64;
typedef ac_int<8, true> si8;
typedef ac_int<16, true> si16;
typedef ac_int<32, true> si32;
typedef ac_int<64, true> si64;

// shift optype begin
//64bit : shamt = src2[5:0]  32bit: shamt = src2[4:0]
//slliuw spec : rd = zext(src1[31:0]) << shamt
ui64 rv_slliuw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = ui64(ui32(rs1)) << shamt;
  return rd;
}

//sll spec : rd = src1 << shamt
ui64 rv_sll(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = rs1 << shamt;
  return rd;
}

//bclr spec : rd = src1 & ~(1 << shamt)
ui64 rv_bclr(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = rs1 & ~(ui64(1) << shamt);
  return rd;
}

//bset spec : rd = src1 | (1 << shamt)
ui64 rv_bset(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = rs1 | (ui64(1) << shamt);
  return rd;
}

//binv spec : rd = src1 ^ (1 << shamt)
ui64 rv_binv(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = rs1 ^ (ui64(1) << shamt);
  return rd;
}

//srl spec : rd = src1 >>u src2
ui64 rv_srl(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = rs1 >> shamt;
  return rd;
}

//bext spec : rd = (src1 >> src2)[0]
ui64 rv_bext(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  rd = (rs1 >> shamt) & ui64(1);
  return rd;
}

//sra spec : rd = src1 >>s src2
ui64 rv_sra(ui64 rs1, ui64 rs2) {
  ui64 rd;
  si64 s_rs1 = si64(rs1);
  ui6 shamt = rs2.slc<6>(0);
  rd = ui64(s_rs1 >> shamt);
  return rd;
}

//rol spec : rd = (src1 << src2) | (src1 >> (xlen-src2))
ui64 rv_rol(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  ui6 revshamt = ui6(64) - shamt;
  rd = (rs1 << shamt) | (rs1 >> revshamt);
  return rd;
}

//ror spec : rd = (src1 >> src2) | (src1 << (xlen-src2))
ui64 rv_ror(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui6 shamt = rs2.slc<6>(0);
  ui6 revshamt = ui6(64) - shamt;
  rd = (rs1 >> shamt) | (rs1 << revshamt);
  return rd;
}
// shift optype end

// word optype begin
//addw spec : rd = sext((src1 + src2)[31:0])
ui64 rv_addw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = ui64(si32(rs1 + rs2));
  return rd;
}

//oddaddw spec : rd = sext((src1[0] + src2)[31:0])
ui64 rv_oddaddw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp[0] = rs1[0];
  rd = ui64(si32(rs1_tmp + rs2));
  return rd;
}

//subw spec : rd = sext((src1 - src2)[31:0])
ui64 rv_subw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = ui64(si32(rs1 - rs2));
  return rd;
}

//addwbit spec : rd = (src1 + src2)[0]
ui64 rv_addwbit(ui64 rs1, ui64 rs2) {
  ui64 rd = 0;
  ui64 sum ;
  sum = rs1 + rs2;
  rd[0] = sum[0];
  return rd;
}

//addwbyte spec : rd = (src1 + src2)[7:0]
ui64 rv_addwbyte(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 sum;
  sum = rs1 + rs2;
  rd = ui64(sum.slc<8>(0));
  return rd;
}

//addwzexth spec : rd = zext((src1 + src2)[15:0])
ui64 rv_addwzexth(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 sum;
  sum = rs1 + rs2;
  rd = ui64(sum.slc<16>(0));
  return rd;
}

//addwsexth spec : rd = sext((src1 + src2)[15:0])
ui64 rv_addwsexth(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 sum;
  sum = rs1 + rs2;
  rd = ui64(si16(sum.slc<16>(0)));
  return rd;
}

//sllw spec : rd = sext((src1 << shamt)[31:0])
ui64 rv_sllw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui5 shamt = rs2.slc<5>(0);
  rd = ui64(si32(rs1 << shamt));
  return rd;
}

//srlw spec : rd = sext((src1[31:0] >>u shamt)[31:0])
ui64 rv_srlw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui5 shamt = rs2.slc<5>(0);
  rd = ui64(si32(ui32(rs1) >> shamt));
  return rd;
}

//sraw spec : rd = sext((src1[31:0] >>s shamt)[31:0])
ui64 rv_sraw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui5 shamt = rs2.slc<5>(0);
  rd = ui64(si32(si32(rs1) >> shamt));
  return rd;
}

//rolw spec : rd = sext((src1[31:0] << shamt) | ( src1[31:0] >> shamt)[31:0])
ui64 rv_rolw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui5 shamt = rs2.slc<5>(0);
  ui5 revshamt = ui5(32) - shamt;
  rd = ui64(si32((ui32(rs1) << shamt) | (ui32(rs1) >> revshamt)));
  return rd;
}

//rorw spec : rd = sext((src1[31:0] >> shamt) | ( src1[31:0] << shamt)[31:0])
ui64 rv_rorw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui5 shamt = rs2.slc<5>(0);
  ui5 revshamt = ui5(32) - shamt;
  rd = ui64(si32((ui32(rs1) >> shamt) | (ui32(rs1) << revshamt)));
  return rd;
}
// word optype end

// add optype begin
//adduw spec : rd = src1[31:0] + src2
ui64 rv_adduw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = ui64(ui32(rs1)) + rs2;
  return rd;
}

//add spec : rd = src1 + src2
ui64 rv_add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 + rs2;
  return rd;
}

//oddadd spec : rd = src1[0] + src2
ui64 rv_oddadd(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp[0] = rs1[0];
  rd = rs1_tmp + rs2;
  return rd;
}

//sr29add spec : rd = src1[63:29] + src2
ui64 rv_sr29add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(0,rs1.slc<35>(29));
  rd = rs1_tmp + rs2;
  return rd;
}

//sr30add spec : rd = src1[63:30] + src2
ui64 rv_sr30add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(0,rs1.slc<34>(30));
  rd = rs1_tmp + rs2;
  return rd;
}

//sr31add spec : rd = src1[63:31] + src2
ui64 rv_sr31add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(0,rs1.slc<33>(31));
  rd = rs1_tmp + rs2;
  return rd;
}

//sr32add spec : rd = src1[63:32] + src2
ui64 rv_sr32add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(0,rs1.slc<32>(32));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh1adduw spec : rd = {src1[31:0],1'b0} + src2
ui64 rv_sh1adduw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(1,rs1.slc<32>(0));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh1add spec : rd = {src1[62:0],1'b0} + src2
ui64 rv_sh1add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(1,rs1.slc<63>(0));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh2adduw spec : rd = {src1[31:0],2'b0} + src2
ui64 rv_sh2adduw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(2,rs1.slc<32>(0));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh2add spec : rd = {src1[61:0],2'b0} + src2
ui64 rv_sh2add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(2,rs1.slc<62>(0));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh3adduw spec : rd = {src1[31:0],3'b0} + src2
ui64 rv_sh3adduw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(3,rs1.slc<32>(0));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh3add spec : rd = {src1[60:0],3'b0} + src2
ui64 rv_sh3add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(3,rs1.slc<61>(0));
  rd = rs1_tmp + rs2;
  return rd;
}

//sh4add spec : rd = {src1[59:0],4'b0} + src2
ui64 rv_sh4add(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(4,rs1.slc<60>(0));
  rd = rs1_tmp + rs2;
  return rd;
}
// add optype end

// sub optype start
//sub spec : rd = src1 - src2
ui64 rv_sub(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 - rs2;
  return rd;
}

//sltu spec : rd = src1 <u src2
ui64 rv_sltu(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 < rs2;
  return rd;
}

//slt spec : rd = src1 <s src2
ui64 rv_slt(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = si64(rs1) < si64(rs2);
  return rd;
}

//maxu spec : if src1 <u src2 return src2
ui64 rv_maxu(ui64 rs1, ui64 rs2) {
  ui64 rd;
  if(rs1 < rs2) {
    rd = rs2;
  } else {
    rd = rs1;
  }
  return rd;
}

//minu spec : if src1 <u src2 return src1
ui64 rv_minu(ui64 rs1, ui64 rs2) {
  ui64 rd;
  if(rs1 < rs2) {
    rd = rs1;
  } else {
    rd = rs2;
  }
  return rd;
}

//maxu spec : if src1 <s src2 return src2
ui64 rv_max(ui64 rs1, ui64 rs2) {
  ui64 rd;
  if(si64(rs1) < si64(rs2)) {
    rd = rs2;
  } else {
    rd = rs1;
  }
  return rd;
}

//min spec : if src1 <s src2 return src1
ui64 rv_min(ui64 rs1, ui64 rs2) {
  ui64 rd;
  if(si64(rs1) < si64(rs2)) {
    rd = rs1;
  } else {
    rd = rs2;
  }
  return rd;
}
// sub optype end

// branch optype start
bool rv_beq(ui64 rs1, ui64 rs2) {
  bool taken = 0;
  if(si64(rs1) == si64(rs2)){
    taken = 1;
  }
  return taken;
}

bool rv_bne(ui64 rs1, ui64 rs2) {
  bool taken = 0;
  if(si64(rs1) != si64(rs2)){
    taken = 1;
  }
  return taken;
}

bool rv_blt(ui64 rs1, ui64 rs2) {
  bool taken = 0;
  if(si64(rs1) < si64(rs2)){
    taken = 1;
  }
  return taken;
}

bool rv_bge(ui64 rs1, ui64 rs2) {
  bool taken = 0;
  if(si64(rs1) >= si64(rs2)){
    taken = 1;
  }
  return taken;
}

bool rv_bltu(ui64 rs1, ui64 rs2) {
  bool taken = 0;
  if(rs1 < rs2){
    taken = 1;
  }
  return taken;
}

bool rv_bgeu(ui64 rs1, ui64 rs2) {
  bool taken = 0;
  if(rs1 >= rs2){
    taken = 1;
  }
  return taken;
}
// branch optype end

// misc optype start
//and spec : rd = src1 & src2
ui64 rv_and(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 & rs2;
  return rd;
}

//andn spec : rd = src1 & ~src2
ui64 rv_andn(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 & ~rs2;
  return rd;
}

//or spec : rd = src1 | src2
ui64 rv_or(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 | rs2;
  return rd;
}

//orn spec : rd = src1 | ~src2
ui64 rv_orn(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 | ~rs2;
  return rd;
}

//xor spec : rd = src1 ^ src2
ui64 rv_xor(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 ^ rs2;
  return rd;
}

//xnor spec : rd = src1 ^ ~src2
ui64 rv_xnor(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1 ^ ~rs2;
  return rd;
}

//orcb spec : 
//for ( i form 0 to (xlen -8) by 8) {
//   output[(i+7)..i] = if input[(i+7)..i] == 0
//                      then 0b00000000
//                      else 0b11111111;
//}
ui64 rv_orcb(ui64 rs1, ui64 rs2) {
  ui64 rd;
  int i;
  for(i=0;i<64;i=i+8){
    if(rs1.slc<8>(i) == 0){
  	  rd.set_slc(i,ui8(0x00));
  	} else {
  	  rd.set_slc(i,ui8(0xFF));
  	}
  }	
  return rd;
}

//sextb spec : rd = sext(src1[7:0])
ui64 rv_sextb(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = ui64(si8(rs1));
  return rd;
}

//packh spec : rd = {src2[7:0],src1[7:0]}
ui64 rv_packh(ui64 rs1, ui64 rs2) {
  ui64 rd = 0;
  rd.set_slc(0,rs1.slc<8>(0));
  rd.set_slc(8,rs2.slc<8>(0));
  return rd;
}

//sexth spec : rd = sext(src1[15:0])
ui64 rv_sexth(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = ui64(si16(rs1));
  return rd;
}

//packw spec : rd = sext({src2[15:0],src1[15:0]})
ui64 rv_packw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui32 rd_tmp = 0;
  rd_tmp.set_slc(0,rs1.slc<16>(0));
  rd_tmp.set_slc(16,rs2.slc<16>(0));
  rd = ui64(si32(rd_tmp));
  return rd;
}

//revb spec : rd = {src1[56..63],src1[48..55],...,src1[0..7]}
ui64 rv_revb(ui64 rs1, ui64 rs2) {
  ui64 rd = 0;
  int i,j,k;
  for(i=0;i<64;i=i+8){
    k = 0;
  	for(j=i+7;j>=i;j--){
  		rd[j] = rs1[i+k];
  		k = k + 1;
  	}
  }	
  return rd;
}

//rev8 spec : rd = {src1[7:0],src1[15:8],...,src1[63:56]}
ui64 rv_rev8(ui64 rs1, ui64 rs2) {
  ui64 rd = 0;
  int i;
  int j = 64 - 1;
  for(i=0;i<64;i=i+8){
  	rd.set_slc(i,rs1.slc<8>(j-7));
    j = j - 8;
  }	
  return rd;
}

//packh spec : rd = {src2[31:0],src1[31:0]}
ui64 rv_pack(ui64 rs1, ui64 rs2) {
  ui64 rd = 0;
  rd.set_slc(0,rs1.slc<32>(0));
  rd.set_slc(32,rs2.slc<32>(0));
  return rd;
}

//orh48 spec : rd = {src1[63:8],8'b0} | src2
ui64 rv_orh48(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rs1_tmp = 0;
  rs1_tmp.set_slc(8,rs1.slc<56>(8));
  rd = rs1_tmp | rs2;
  return rd;
}
// misc optype end

void hector_wrapper()
{
    ui64 rs1, rs2;
    ui7  fuOpType;
    
    ui64 rd;
    bool taken;

    Hector::registerInput("rs1", rs1);
    Hector::registerInput("rs2", rs2);
    Hector::registerInput("fuOpType", fuOpType);
    
    Hector::registerOutput("rd", rd);
    Hector::registerOutput("taken", taken);
    
    Hector::beginCapture();

    switch (fuOpType) {
    // shift optype
    case 0x00: rd = rv_slliuw(rs1, rs2);    break;
    case 0x01: rd = rv_sll(rs1, rs2);       break;
    case 0x02: rd = rv_bclr(rs1, rs2);      break;
    case 0x03: rd = rv_bset(rs1, rs2);      break;
    case 0x04: rd = rv_binv(rs1, rs2);      break;
    case 0x05: rd = rv_srl(rs1, rs2);       break;
    case 0x06: rd = rv_bext(rs1, rs2);      break;
    case 0x07: rd = rv_sra(rs1, rs2);       break;
    case 0x09: rd = rv_rol(rs1, rs2);       break;
    case 0x0B: rd = rv_ror(rs1, rs2);       break;
    // word optype
    case 0x10: rd = rv_addw(rs1, rs2);      break;
    case 0x11: rd = rv_oddaddw(rs1, rs2);   break;
    case 0x12: rd = rv_subw(rs1, rs2);      break;
    case 0x14: rd = rv_addwbit(rs1, rs2);   break;
    case 0x15: rd = rv_addwbyte(rs1, rs2);  break;
    case 0x16: rd = rv_addwzexth(rs1, rs2); break;
    case 0x17: rd = rv_addwsexth(rs1, rs2); break;
    case 0x18: rd = rv_sllw(rs1, rs2);      break;
    case 0x19: rd = rv_srlw(rs1, rs2);      break;
    case 0x1A: rd = rv_sraw(rs1, rs2);      break;
    case 0x1C: rd = rv_rolw(rs1, rs2);      break;
    case 0x1D: rd = rv_rorw(rs1, rs2);      break;
    // add optype
    case 0x20: rd = rv_adduw(rs1, rs2);     break;
    case 0x21: rd = rv_add(rs1, rs2);       break;
    case 0x22: rd = rv_oddadd(rs1, rs2);    break;
    case 0x24: rd = rv_sr29add(rs1, rs2);   break;
    case 0x25: rd = rv_sr30add(rs1, rs2);   break;
    case 0x26: rd = rv_sr31add(rs1, rs2);   break;
    case 0x27: rd = rv_sr32add(rs1, rs2);   break;
    case 0x28: rd = rv_sh1adduw(rs1, rs2);  break;
    case 0x29: rd = rv_sh1add(rs1, rs2);    break;
    case 0x2A: rd = rv_sh2adduw(rs1, rs2);  break;
    case 0x2B: rd = rv_sh2add(rs1, rs2);    break;
    case 0x2C: rd = rv_sh3adduw(rs1, rs2);  break;
    case 0x2D: rd = rv_sh3add(rs1, rs2);    break;
    case 0x2F: rd = rv_sh4add(rs1, rs2);    break;
    // sub optype
    case 0x30: rd = rv_sub(rs1, rs2);       break;
    case 0x31: rd = rv_sltu(rs1, rs2);      break;
    case 0x32: rd = rv_slt(rs1, rs2);       break;
    case 0x34: rd = rv_maxu(rs1, rs2);      break;
    case 0x35: rd = rv_minu(rs1, rs2);      break;
    case 0x36: rd = rv_max(rs1, rs2);       break;
    case 0x37: rd = rv_min(rs1, rs2);       break;
    // branch optype
    case 0x70: taken = rv_beq(rs1, rs2);    break;
    case 0x72: taken = rv_bne(rs1, rs2);    break;
    case 0x78: taken = rv_blt(rs1, rs2);    break;
    case 0x7A: taken = rv_bge(rs1, rs2);    break;
    case 0x7C: taken = rv_bltu(rs1, rs2);   break;
    case 0x7E: taken = rv_bgeu(rs1, rs2);   break;
    // misc optype
    case 0x40: rd = rv_and(rs1, rs2);       break;
    case 0x41: rd = rv_andn(rs1, rs2);      break;
    case 0x42: rd = rv_or(rs1, rs2);        break;
    case 0x43: rd = rv_orn(rs1, rs2);       break;
    case 0x44: rd = rv_xor(rs1, rs2);       break;
    case 0x45: rd = rv_xnor(rs1, rs2);      break;
    case 0x46: rd = rv_orcb(rs1, rs2);      break;
    case 0x48: rd = rv_sextb(rs1, rs2);     break;
    case 0x49: rd = rv_packh(rs1, rs2);     break;
    case 0x4A: rd = rv_sexth(rs1, rs2);     break;
    case 0x4B: rd = rv_packw(rs1, rs2);     break;
    case 0x50: rd = rv_revb(rs1, rs2);      break;
    case 0x51: rd = rv_rev8(rs1, rs2);      break;
    case 0x52: rd = rv_pack(rs1, rs2);      break;
    case 0x53: rd = rv_orh48(rs1, rs2);     break;
    }
    
    Hector::endCapture();
}

