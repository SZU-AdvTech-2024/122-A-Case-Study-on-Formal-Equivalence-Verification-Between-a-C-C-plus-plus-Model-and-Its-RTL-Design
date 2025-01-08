#include "Hector.h"
#include "softfloat.h"
#include <ac_int.h>
#include <rac.h>
#include <string>
#include <stdio.h>
using namespace std;

//RAC begin
typedef ac_int<1, false> ui1;
typedef ac_int<2, false> ui2;
typedef ac_int<8, false> ui8;
typedef ac_int<32, false> ui32;
typedef ac_int<64, false> ui64;
typedef ac_int<8, true> si8;

//struct
struct result{
  uint64_t rd;
  uint8_t exceptions;
};

//fadd_s
result rv_fadd_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs_fadd_s;
    uint32_t rs1Int32, rs2Int32;
    float32_t f_rs1, f_rs2, f_rd;
   
    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;
    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;
   
    f_rs1.v = rs1Int32;
    f_rs2.v = rs2Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f32_add(f_rs1, f_rs2);

    rs_fadd_s.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs_fadd_s.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs_fadd_s;
}

//fadd_d
result rv_fadd_d(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs_fadd_d;
    float64_t f_rs1, f_rs2, f_rd;

    f_rs1.v = rs1;
    f_rs2.v = rs2;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f64_add(f_rs1, f_rs2);

    rs_fadd_d.rd = f_rd.v; 
    rs_fadd_d.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs_fadd_d;
}

//fsub_s
result rv_fsub_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs_fsub_s;
    uint32_t rs1Int32, rs2Int32;
    float32_t f_rs1, f_rs2, f_rd;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;
    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;
    f_rs2.v = rs2Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f32_sub(f_rs1, f_rs2);

    rs_fsub_s.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs_fsub_s.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs_fsub_s;
}

//fsub_d
result rv_fsub_d(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1, f_rs2, f_rd;

    f_rs1.v = rs1;
    f_rs2.v = rs2;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f64_sub(f_rs1, f_rs2);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fmul_s
result rv_fmul_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32, rs2Int32;
    float32_t f_rs1, f_rs2, f_rd;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;
    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;
    f_rs2.v = rs2Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f32_mul(f_rs1, f_rs2);

    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}


void hector_wrapper()
{
    uint8_t rounding_mode, csr_frm, rm;
    uint8_t exceptions;
    uint64_t rs1, rs2, rd;
    result rs;

    int isAddSub, typeTagIn, fmaCmd, ren3;
    int sign;

    Hector::registerInput("rs1", &rs1, 8 * sizeof(rs1));
    Hector::registerInput("rs2", &rs2, 8 * sizeof(rs2));
    Hector::registerInput("rm" , &rm , 8 * sizeof(rm));
    Hector::registerInput("csr_frm"  , csr_frm);

    Hector::registerInput("isAddSub" , isAddSub);
    Hector::registerInput("typeTagIn", typeTagIn);
    Hector::registerInput("fmaCmd"   , fmaCmd);
    Hector::registerInput("ren3"     , ren3);    

    Hector::registerOutput("rd", &rd, 8 * sizeof(rd));
    Hector::registerOutput("exceptions", &exceptions, 8 * sizeof(exceptions));
    
    Hector::beginCapture();
  
    int low_isAddSub = isAddSub & 0b1;
    int low_typeTagIn = typeTagIn & 0b1;
    int low_fmaCmd = fmaCmd & 0b11;
    int low_ren3 = ren3 & 0b1;

    if(rm == 7){
       rounding_mode = csr_frm;
    }else{
       rounding_mode = rm;
    }

    sign =(low_isAddSub << 4) | (low_typeTagIn << 3) | (low_fmaCmd << 1) | low_ren3;

    switch (sign & 0x1f) {
    case 0x10: rs = rv_fadd_s(rs1, rs2, rounding_mode);    break;
    case 0x18: rs = rv_fadd_d(rs1, rs2, rounding_mode);    break;
    case 0x12: rs = rv_fsub_s(rs1, rs2, rounding_mode);    break;
    case 0x1A: rs = rv_fsub_d(rs1, rs2, rounding_mode);    break;

    }

    rd = rs.rd;
    exceptions = rs.exceptions;
    
    Hector::endCapture();
}
