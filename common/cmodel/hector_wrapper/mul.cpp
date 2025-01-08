#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <string>
#include <vector>
using namespace std;

// RAC begin

typedef ac_int<7, false> ui7;
typedef ac_int<8, false> ui8;
typedef ac_int<32, false> ui32;
typedef ac_int<64, false> ui64;
typedef ac_int<128, false> ui128;
typedef ac_int<32, true> si32;
typedef ac_int<64, true> si64;
typedef ac_int<128, true> si128;

// mul optype begin
ui64 rv_mul(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = rs1*rs2;
  return rd;
}

ui64 rv_mulh(ui64 rs1, ui64 rs2) {
  ui64 rd;
  si128 rd_tmp;
  rd_tmp = si64(rs1)*si64(rs2);
  rd = rd_tmp.slc<64>(64);
  return rd;
}

ui64 rv_mulhsu(ui64 rs1, ui64 rs2) {
  ui64 rd;
  si128 rd_tmp;
  rd_tmp = si64(rs1)*rs2;
  rd = rd_tmp.slc<64>(64);
  return rd;
}

ui64 rv_mulhu(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui128 rd_tmp;
  rd_tmp = rs1*rs2;
  rd = rd_tmp.slc<64>(64);
  return rd;
}

ui64 rv_mulw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = ui64(si32(rs1*rs2));
  return rd;
}

ui64 rv_mulw7(ui64 rs1, ui64 rs2) {
  ui7  rs1_7bit = ui7(rs1);
  ui64 rd;
  rd = ui64(si32(rs1_7bit*rs2));
  return rd;
}
// mul optype end

void hector_wrapper()
{
    ui64 rs1, rs2;
    ui7  fuOpType;
    
    ui64 rd;
    ui8  rd0;
    ui8  rd1;
    ui8  rd2;
    ui8  rd3;
    ui8  rd4;
    ui8  rd5;
    ui8  rd6;
    ui8  rd7;

    Hector::registerInput("rs1", rs1);
    Hector::registerInput("rs2", rs2);
    Hector::registerInput("fuOpType", fuOpType);
    
    Hector::registerOutput("rd", rd);
    Hector::registerOutput("rd0", rd0);
    Hector::registerOutput("rd1", rd1);
    Hector::registerOutput("rd2", rd2);
    Hector::registerOutput("rd3", rd3);
    Hector::registerOutput("rd4", rd4);
    Hector::registerOutput("rd5", rd5);
    Hector::registerOutput("rd6", rd6);
    Hector::registerOutput("rd7", rd7);
    
    Hector::beginCapture();

    switch (fuOpType) {
    // bku optype
    case 0x00: rd = rv_mul(rs1, rs2);    break;
    case 0x01: rd = rv_mulh(rs1, rs2);   break;
    case 0x02: rd = rv_mulhsu(rs1, rs2); break;
    case 0x03: rd = rv_mulhu(rs1, rs2);  break;
    case 0x04: rd = rv_mulw(rs1, rs2);   break;
    case 0x0C: rd = rv_mulw7(rs1, rs2);  break;
    }

    rd0    = rd.slc<8>(0);
    rd1    = rd.slc<8>(8);
    rd2    = rd.slc<8>(16);
    rd3    = rd.slc<8>(24);
    rd4    = rd.slc<8>(32);
    rd5    = rd.slc<8>(40);
    rd6    = rd.slc<8>(48);
    rd7    = rd.slc<8>(56);
 
    Hector::endCapture();
}

