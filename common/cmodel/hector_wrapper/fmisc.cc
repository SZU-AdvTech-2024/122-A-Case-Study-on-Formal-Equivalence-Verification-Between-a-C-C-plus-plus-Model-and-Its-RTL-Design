#include "Hector.h"
#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"
#include "specialize.h"
#include "softfloat.h"

//struct
struct result{
  uint64_t rd;
  uint8_t exceptions;
};

//fmv_x_h
result rv_fmv_x_h(uint64_t rs1, uint8_t rounding_mode){
    result rs;

    int16_t res = (int16_t)rs1;
    rs.rd = (uint64_t)res;

    return rs;
}

//FPtoInt Begin
//fmv_x_w
result rv_fmv_x_w(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;
    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    union ui32_f32 uA;
    uint_fast32_t uiA;
    uA.f = f_rs1;
    uiA = uA.ui;
    if ( isNaNF32UI( uiA ) ) {
        if ( softfloat_isSigNaNF32UI( uiA ) ) {
            softfloat_raiseFlags( softfloat_flag_invalid );
        }
    }

    int32_t res = (int32_t)rs1;
    rs.rd = (uint64_t)res;

    return rs;
}

//fclass_h
result rv_fclass_h(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;
    f_rs1.v = rs1Int16;

    bool isNegative  = ((rs1Int16 & 0x8000) == 0x8000);
    bool isNormal    = ((rs1Int16 & 0x7C00) && ((rs1Int16 & 0x7C00) != 0x7C00));
    bool isSubNormal = (((rs1Int16 & 0x7C00) == 0x0000) && (rs1Int16 & 0x03FF));

    bool isNInf       = (rs1Int16 == 0xFC00);
    bool isNNormal    = isNormal && isNegative;
    bool isNSubNormal = isSubNormal && isNegative;
    bool isNZero      = (rs1Int16 == 0x8000);
    bool isPZero      = (rs1Int16 == 0x0000);
    bool isPSubNormal = isSubNormal && !isNegative;
    bool isPNormal    = isNormal && !isNegative;
    bool isPInf       = (rs1Int16 == 0x7C00);
    bool isSNaN       = (((rs1Int16 & 0x7E00) == 0x7C00) && (rs1Int16 & 0x01FF));
    bool isQNaN       = ((rs1Int16 & 0x7E00) == 0x7E00);
    uint64_t res = 0x0;
    if(isNInf)       res = 0b0000000001;
    if(isNNormal)    res = 0b0000000010;
    if(isNSubNormal) res = 0b0000000100;
    if(isNZero)      res = 0b0000001000;
    if(isPZero)      res = 0b0000010000;
    if(isPSubNormal) res = 0b0000100000;
    if(isPNormal)    res = 0b0001000000;
    if(isPInf)       res = 0b0010000000;
    if(isSNaN)       res = 0b0100000000;
    if(isQNaN)       res = 0b1000000000;

    rs.rd = res;

    return rs;
}

//fclass_s
result rv_fclass_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;
    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    union ui32_f32 uA;
    uint_fast32_t uiA;
    uA.f = f_rs1;
    uiA = uA.ui;
    if ( isNaNF32UI( uiA ) ) {
        if ( softfloat_isSigNaNF32UI( uiA ) ) {
            softfloat_raiseFlags( softfloat_flag_invalid );
        }
    }

    bool isNegative  = ((rs1Int32 & 0x80000000) == 0x80000000);
    bool isNormal    = ((rs1Int32 & 0x7F800000) && ((rs1Int32 & 0x7F800000) != 0x7F800000));
    bool isSubNormal = (((rs1Int32 & 0x7F800000) == 0x00000000) && (rs1Int32 & 0x007FFFFF));

    bool isNInf       = (rs1Int32 == 0xFF800000);
    bool isNNormal    = isNormal && isNegative;
    bool isNSubNormal = isSubNormal && isNegative;
    bool isNZero      = (rs1Int32 == 0x80000000);
    bool isPZero      = (rs1Int32 == 0x00000000);
    bool isPSubNormal = isSubNormal && !isNegative;
    bool isPNormal    = isNormal && !isNegative;
    bool isPInf       = (rs1Int32 == 0x7F800000);
    bool isSNaN       = (((rs1Int32 & 0x7FC00000) == 0x7F800000) && (rs1Int32 & 0x003FFFFF));
    bool isQNaN       = ((rs1Int32 & 0x7FC00000) == 0x7FC00000);
    uint64_t res = 0x0;
    if(isNInf)       res = 0b0000000001;
    if(isNNormal)    res = 0b0000000010;
    if(isNSubNormal) res = 0b0000000100;
    if(isNZero)      res = 0b0000001000;
    if(isPZero)      res = 0b0000010000;
    if(isPSubNormal) res = 0b0000100000;
    if(isPNormal)    res = 0b0001000000;
    if(isPInf)       res = 0b0010000000;
    if(isSNaN)       res = 0b0100000000;
    if(isQNaN)       res = 0b1000000000;

    rs.rd = res;

    return rs;
}

//fcvt_w_h
result rv_fcvt_w_h(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    f_rs1.v = rs1Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f16_to_i32(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_wu_h
result rv_fcvt_wu_h(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    f_rs1.v = rs1Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    int32_t rdTmp = (int32_t)f16_to_ui32(f_rs1, rounding_mode, 1);
    rs.rd = (uint64_t)rdTmp;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_l_h
result rv_fcvt_l_h(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    f_rs1.v = rs1Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f16_to_i64(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_lu_h
result rv_fcvt_lu_h(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    f_rs1.v = rs1Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f16_to_ui64(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_w_s
result rv_fcvt_w_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f32_to_i32(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_wu_s
result rv_fcvt_wu_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    int32_t res = (int32_t)f32_to_ui32(f_rs1, rounding_mode, 1);
    rs.rd = (uint64_t)res;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_l_s
result rv_fcvt_l_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = f32_to_i64(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_lu_s
result rv_fcvt_lu_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = f32_to_ui64(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fmv_x_d
result rv_fmv_x_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    union ui64_f64 uA;
    uint_fast64_t uiA;
    uA.f = f_rs1;
    uiA = uA.ui;
    if ( isNaNF64UI( uiA ) ) {
        if (
            softfloat_isSigNaNF64UI( uiA )
        ) {
            softfloat_raiseFlags( softfloat_flag_invalid );
        }
    }

    rs.rd = rs1;

    return rs;
}

//fclass_d
result rv_fclass_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;
    uint64_t rs1_exp;
    uint64_t rs1_sig;

    rs1_exp = (rs1>>52)&0x7ff;
    rs1_sig = (rs1&0xfffffffffffff);

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    union ui64_f64 uA;
    uint_fast64_t uiA;
    uA.f = f_rs1;
    uiA = uA.ui;
    if ( isNaNF64UI( uiA ) ) {
        if (
            softfloat_isSigNaNF64UI( uiA )
        ) {
            softfloat_raiseFlags( softfloat_flag_invalid );
        }
    }

    bool isNegative  = ((rs1 & 0x8000000000000000) == 0x8000000000000000);
    bool isNormal    = ((rs1 & 0x7FF0000000000000) && ((rs1 & 0x7FF0000000000000) != 0x7FF0000000000000));
    bool isSubNormal = (((rs1 & 0x7FF0000000000000) == 0x0000000000000000) && (rs1 & 0x000FFFFFFFFFFFFF));

    bool isNInf       = (rs1 == 0xFFF0000000000000);
    bool isNNormal    = isNormal && isNegative;
    bool isNSubNormal = isSubNormal && isNegative;
    bool isNZero      = (rs1 == 0x8000000000000000);
    bool isPZero      = (rs1 == 0x0000000000000000);
    bool isPSubNormal = isSubNormal && !isNegative;
    bool isPNormal    = isNormal && !isNegative;
    bool isPInf       = (rs1 == 0x7FF0000000000000);
    bool isSNaN       = (((rs1 & 0x7FF8000000000000) == 0x7FF0000000000000) && (rs1 & 0x0007FFFFFFFFFFFF));
    bool isQNaN       = ((rs1 & 0x7FF8000000000000) == 0x7FF8000000000000);
    uint64_t res = 0x0;
    if(isNInf)       res = 0b0000000001;
    if(isNNormal)    res = 0b0000000010;
    if(isNSubNormal) res = 0b0000000100;
    if(isNZero)      res = 0b0000001000;
    if(isPZero)      res = 0b0000010000;
    if(isPSubNormal) res = 0b0000100000;
    if(isPNormal)    res = 0b0001000000;
    if(isPInf)       res = 0b0010000000;
    if(isSNaN)       res = 0b0100000000;
    if(isQNaN)       res = 0b1000000000;

    rs.rd = res;

    return rs;
}
//fcvt_w_d
result rv_fcvt_w_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;

    f_rs1.v = rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f64_to_i32(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_wu_d
result rv_fcvt_wu_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;

    f_rs1.v = rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    int32_t res = (int32_t)f64_to_ui32(f_rs1, rounding_mode, 1);
    rs.rd = (uint64_t)res;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_l_d
result rv_fcvt_l_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;

    f_rs1.v = rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = f64_to_i64(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_lu_d
result rv_fcvt_lu_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;

    f_rs1.v = rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = f64_to_ui64(f_rs1, rounding_mode, 1);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//Fcmp

//feq_s
result rv_feq_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32, rs2Int32;
    float32_t f_rs1,f_rs2;

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

    rs.rd = (uint64_t)f32_eq(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//flt_h
result rv_flt_h(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16, rs2Int16;
    float16_t f_rs1,f_rs2;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;
    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    f_rs1.v = rs1Int16;
    f_rs2.v = rs2Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f16_lt(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fle_h
result rv_fle_h(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16, rs2Int16;
    float16_t f_rs1,f_rs2;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;
    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    f_rs1.v = rs1Int16;
    f_rs2.v = rs2Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f16_le(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//feq_h
result rv_feq_h(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint16_t rs1Int16, rs2Int16;
    float16_t f_rs1,f_rs2;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;
    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    f_rs1.v = rs1Int16;
    f_rs2.v = rs2Int16;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f16_eq(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//flt_s
result rv_flt_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32, rs2Int32;
    float32_t f_rs1,f_rs2;

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

    rs.rd = (uint64_t)f32_lt(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fle_s
result rv_fle_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    uint32_t rs1Int32, rs2Int32;
    float32_t f_rs1,f_rs2;

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

    rs.rd = (uint64_t)f32_le(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//feq_d
result rv_feq_d(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1,f_rs2;

    f_rs1.v = rs1;
    f_rs2.v = rs2;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f64_eq(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//flt_d
result rv_flt_d(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1,f_rs2;

    f_rs1.v = rs1;
    f_rs2.v = rs2;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f64_lt(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fle_d
result rv_fle_d(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1,f_rs2;

    f_rs1.v = rs1;
    f_rs2.v = rs2;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = (uint64_t)f64_le(f_rs1, f_rs2);
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//FPtoInt End

//fdiv_s
result rv_fdiv_s(uint64_t rs1, uint64_t rs2, uint8_t rounding_mode){
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

    f_rd = f32_div(f_rs1, f_rs2);

    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//FPToFP Begin
//fcvt_h_s
result rv_fcvt_h_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float32_t f_rs1;
    float16_t f_rd;
    uint32_t rs1Int32;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f32_to_f16(f_rs1);

    rs.rd = f_rd.v | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_s_d
result rv_fcvt_s_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;
    float32_t f_rd;

    f_rs1.v = rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f64_to_f32(f_rs1);
    
    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_h_d
result rv_fcvt_h_d(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rs1;
    float16_t f_rd;

    f_rs1.v = rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f64_to_f16(f_rs1);

    rs.rd = f_rd.v | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}


//fcvt_d_s
result rv_fcvt_d_s(uint64_t rs1){
    result rs;
    uint32_t rs1Int32;
    float32_t f_rs1;
    float64_t f_rd;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    f_rs1.v = rs1Int32;

    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f32_to_f64(f_rs1);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_d_h
result rv_fcvt_d_h(uint64_t rs1){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;
    float64_t f_rd;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    f_rs1.v = rs1Int16;

    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f16_to_f64(f_rs1);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_s_h
result rv_fcvt_s_h(uint64_t rs1){
    result rs;
    uint16_t rs1Int16;
    float16_t f_rs1;
    float32_t f_rd;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    f_rs1.v = rs1Int16;

    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f16_to_f32(f_rs1);

    rs.rd = f_rd.v | 0xffffffff00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fsgnj_h
result rv_fsgnj_h(uint64_t rs1, uint64_t rs2){

    result rs;
    uint16_t rs1Int16;
    uint16_t rs2Int16;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    rs.rd = (0xffffffffffff0000)|(rs1Int16 & 0x7FFF) | (rs2Int16 & 0x8000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnjn_h
result rv_fsgnjn_h(uint64_t rs1, uint64_t rs2){

    result rs;
    uint16_t rs1Int16;
    uint16_t rs2Int16;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    rs.rd = (0xffffffffffff0000)|(rs1Int16 & 0x7FFF) | (~rs2Int16 & 0x8000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnjx_h
result rv_fsgnjx_h(uint64_t rs1, uint64_t rs2){

    result rs;
    uint16_t rs1Int16;
    uint16_t rs2Int16;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    rs.rd = (0xffffffffffff0000)|(rs1Int16 & 0x7FFF) | ((rs1Int16 ^ rs2Int16) & 0x8000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnj_s
result rv_fsgnj_s(uint64_t rs1, uint64_t rs2){

    result rs;
    uint32_t rs1Int32;
    uint32_t rs2Int32;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    rs.rd = (0xffffffff00000000)|(rs1Int32 & 0x7FFFFFFF) | (rs2Int32 & 0x80000000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnjn_s
result rv_fsgnjn_s(uint64_t rs1, uint64_t rs2){

    result rs;
    uint32_t rs1Int32;
    uint32_t rs2Int32;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    rs.rd = (0xffffffff00000000)|(rs1Int32 & 0x7FFFFFFF) | ((~rs2Int32) & 0x80000000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnjx_s
result rv_fsgnjx_s(uint64_t rs1, uint64_t rs2){

    result rs;
    uint32_t rs1Int32;
    uint32_t rs2Int32;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    rs.rd = (0xffffffff00000000)|(rs1Int32 & 0x7FFFFFFF) | ((rs1Int32 ^ rs2Int32) & 0x80000000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnj_d
result rv_fsgnj_d(uint64_t rs1, uint64_t rs2){

    result rs;

    rs.rd = (rs1 & 0x7FFFFFFFFFFFFFFF) | (rs2 & 0x8000000000000000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnjn_d
result rv_fsgnjn_d(uint64_t rs1, uint64_t rs2){

    result rs;

    rs.rd = (rs1 & 0x7FFFFFFFFFFFFFFF) | ((~rs2) & 0x8000000000000000);
    rs.exceptions = 0x0;

    return rs;

}

//fsgnjx_d
result rv_fsgnjx_d(uint64_t rs1, uint64_t rs2){

    result rs;

    rs.rd = (rs1 & 0x7FFFFFFFFFFFFFFF) | ((rs1 ^ rs2) & 0x8000000000000000);
    rs.exceptions = 0x0;

    return rs;

}

//fmin_h
result rv_fmin_h(uint64_t rs1, uint64_t rs2){

    result rs;
    uint16_t rs1Int16;
    uint16_t rs2Int16;
    uint8_t fflags;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    float16_t f_rs1;
    float16_t f_rs2;
    f_rs1.v = rs1Int16;
    f_rs2.v = rs2Int16;

    bool islt = f16_lt(f_rs1, f_rs2);
    bool isBothZero = (((rs1Int16 & 0x7FFF) == 0x0) && ((rs2Int16 & 0x7FFF) == 0x0));
    bool hasNZero =   ((rs1Int16 == 0x8000) || (rs2Int16 == 0x8000));
    bool rs1_isNaN = (((rs1Int16 & 0x7C00) == 0x7C00) && (rs1Int16 & 0x03FF));
    bool rs2_isNaN = (((rs2Int16 & 0x7C00) == 0x7C00) && (rs2Int16 & 0x03FF));

    uint16_t res;
    if(islt)
        res = rs1Int16;
    else
        res = rs2Int16;
    if(rs1_isNaN && rs2_isNaN) {
        rs.rd = 0xFFFFFFFFFFFF7E00;
    } else if(!rs1_isNaN && rs2_isNaN) {
        rs.rd = rs1Int16 | 0xFFFFFFFFFFFF0000;
    } else if(rs1_isNaN && !rs2_isNaN) {
        rs.rd = rs2Int16 | 0xFFFFFFFFFFFF0000;
    } else if(isBothZero) {
        if(hasNZero)
            rs.rd = 0xFFFFFFFFFFFF8000;
        else
            rs.rd = 0xFFFFFFFFFFFF0000;
    } else {
        rs.rd = res | 0xFFFFFFFFFFFF0000;
    }
    fflags = 0x0;
    Hector::show("exceptionsTest", fflags);
    if(((rs1Int16 & 0x7E00) == 0x7C00) && (rs1Int16 & 0x01FF) || (((rs2Int16 & 0x7E00) == 0x7C00) && (rs2Int16 & 0x01FF))){
       fflags = 0x10;
    }else{
       fflags = 0x00;
    }
    rs.exceptions = fflags;
    return rs;

}

//fmax_h
result rv_fmax_h(uint64_t rs1, uint64_t rs2){

    result rs;
    uint16_t rs1Int16;
    uint16_t rs2Int16;

    if((rs1 >> 16) == 0xFFFFFFFFFFFF)
        rs1Int16 = (uint16_t)rs1;
    else
        rs1Int16 = 0x7E00;

    if((rs2 >> 16) == 0xFFFFFFFFFFFF)
        rs2Int16 = (uint16_t)rs2;
    else
        rs2Int16 = 0x7E00;

    float16_t f_rs1;
    float16_t f_rs2;
    f_rs1.v = rs1Int16;
    f_rs2.v = rs2Int16;

    bool islt = f16_lt(f_rs1, f_rs2);
    bool isBothZero = (((rs1Int16 & 0x7FFF) == 0x0) && ((rs2Int16 & 0x7FFF) == 0x0));
    bool hasPZero =   ((rs1Int16 == 0x0000) || (rs2Int16 == 0x0000));
    bool rs1_isNaN = (((rs1Int16 & 0x7C00) == 0x7C00) && (rs1Int16 & 0x03FF));
    bool rs2_isNaN = (((rs2Int16 & 0x7C00) == 0x7C00) && (rs2Int16 & 0x03FF));
    
    uint16_t res;
    if(islt)
        res = rs2Int16;
    else
        res = rs1Int16;
    if(rs1_isNaN && rs2_isNaN) {
        rs.rd = 0xFFFFFFFFFFFF7E00;
    } else if(!rs1_isNaN && rs2_isNaN) {
        rs.rd = rs1Int16 | 0xFFFFFFFFFFFF0000;
    } else if(rs1_isNaN && !rs2_isNaN) {
        rs.rd = rs2Int16 | 0xFFFFFFFFFFFF0000;
    } else if(isBothZero) {
        if(hasPZero)
            rs.rd = 0xFFFFFFFFFFFF0000;
        else
            rs.rd = 0xFFFFFFFFFFFF8000;
    } else {
        rs.rd = res | 0xFFFFFFFFFFFF0000;
    }

    if(((rs1Int16 & 0x7E00) == 0x7C00) && (rs1Int16 & 0x01FF) || (((rs2Int16 & 0x7E00) == 0x7C00) && (rs2Int16 & 0x01FF))){
       rs.exceptions = 0x10;
    }else{
       rs.exceptions = 0x00;
    }

    return rs;

}

//fmin_s
result rv_fmin_s(uint64_t rs1, uint64_t rs2){

    result rs;
    uint32_t rs1Int32;
    uint32_t rs2Int32;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    float32_t f_rs1;
    float32_t f_rs2;
    f_rs1.v = rs1Int32;
    f_rs2.v = rs2Int32;

    bool islt = f32_lt(f_rs1, f_rs2);
    bool isBothZero = (((rs1Int32 & 0x7FFFFFFF) == 0x00000000) && ((rs2Int32 & 0x7FFFFFFF) == 0x00000000));
    bool hasNZero = ((rs1Int32 == 0x80000000) || (rs2Int32 == 0x80000000));
    bool rs1_isNaN = (((rs1Int32 & 0x7F800000) == 0x7F800000) && (rs1Int32 & 0x007FFFFF));
    bool rs2_isNaN = (((rs2Int32 & 0x7F800000) == 0x7F800000) && (rs2Int32 & 0x007FFFFF));

    uint32_t res;
    if(islt)
        res = rs1Int32;
    else
        res = rs2Int32;
    if(rs1_isNaN && rs2_isNaN) {
        rs.rd = 0xFFFFFFFF7FC00000;
    } else if(!rs1_isNaN && rs2_isNaN) {
        rs.rd = rs1Int32 | 0xFFFFFFFF00000000;
    } else if(rs1_isNaN && !rs2_isNaN) {
        rs.rd = rs2Int32 | 0xFFFFFFFF00000000;
    } else if(isBothZero) {
        if(hasNZero)
            rs.rd = 0xFFFFFFFF80000000;
        else
            rs.rd = 0xFFFFFFFF00000000;
    } else {
        rs.rd = res | 0xFFFFFFFF00000000;
    }

    if((rs1_isNaN && ~(rs1Int32 | 0xFFBFFFFF)) || (rs2_isNaN && ~(rs2Int32 | 0xFFBFFFFF))){
       rs.exceptions = 0x10;
    }else{
       rs.exceptions = 0x00;
    }

    return rs;

}

//fmax_s
result rv_fmax_s(uint64_t rs1, uint64_t rs2){

    result rs;
    uint32_t rs1Int32;
    uint32_t rs2Int32;

    if((rs1 >> 32) == 0xFFFFFFFF)
        rs1Int32 = (uint32_t)rs1;
    else
        rs1Int32 = 0x7FC00000;

    if((rs2 >> 32) == 0xFFFFFFFF)
        rs2Int32 = (uint32_t)rs2;
    else
        rs2Int32 = 0x7FC00000;

    float32_t f_rs1;
    float32_t f_rs2;
    f_rs1.v = rs1Int32;
    f_rs2.v = rs2Int32;

    bool islt = f32_lt(f_rs1, f_rs2);
    bool isBothZero = (((rs1Int32 & 0x7FFFFFFF) == 0x00000000) && ((rs2Int32 & 0x7FFFFFFF) == 0x00000000));
    bool hasPZero = ((rs1Int32 == 0x00000000) || (rs2Int32 == 0x00000000));
    bool rs1_isNaN = (((rs1Int32 & 0x7F800000) == 0x7F800000) && (rs1Int32 & 0x007FFFFF));
    bool rs2_isNaN = (((rs2Int32 & 0x7F800000) == 0x7F800000) && (rs2Int32 & 0x007FFFFF));

    uint32_t res;
    if(islt)
        res = rs2Int32;
    else
        res = rs1Int32;
    if(rs1_isNaN && rs2_isNaN) {
        rs.rd = 0xFFFFFFFF7FC00000;
    } else if(!rs1_isNaN && rs2_isNaN) {
        rs.rd = rs1Int32 | 0xFFFFFFFF00000000;
    } else if(rs1_isNaN && !rs2_isNaN) {
        rs.rd = rs2Int32 | 0xFFFFFFFF00000000;
    } else if(isBothZero) {
        if(hasPZero)
            rs.rd = 0xFFFFFFFF00000000;
        else
            rs.rd = 0xFFFFFFFF80000000;
    } else {
        rs.rd = res | 0xFFFFFFFF00000000;
    }

    if((rs1_isNaN && ~(rs1Int32 | 0xFFBFFFFF)) || (rs2_isNaN && ~(rs2Int32 | 0xFFBFFFFF))){
       rs.exceptions = 0x10;
    }else{
       rs.exceptions = 0x00;
    }

    return rs;

}

//fmin_d
result rv_fmin_d(uint64_t rs1, uint64_t rs2){

    result rs;

    float64_t f_rs1;
    float64_t f_rs2;
    f_rs1.v = rs1;
    f_rs2.v = rs2;

    bool islt = f64_lt(f_rs1, f_rs2);
    bool isBothZero = (((rs1 & 0x7FFFFFFFFFFFFFFF) == 0) && ((rs2 & 0x7FFFFFFFFFFFFFFF) == 0));
    bool hasNZero = ((rs1 == 0x8000000000000000) || (rs2 == 0x8000000000000000));
    bool rs1_isNaN = (((rs1 & 0x7FF0000000000000) == 0x7FF0000000000000) && (rs1 & 0x000FFFFFFFFFFFFF));
    bool rs2_isNaN = (((rs2 & 0x7FF0000000000000) == 0x7FF0000000000000) && (rs2 & 0x000FFFFFFFFFFFFF));

    uint64_t res;
    if(islt)
        res = rs1;
    else
        res = rs2;
    if(rs1_isNaN && rs2_isNaN) {
        rs.rd = 0x7FF8000000000000;
    } else if(!rs1_isNaN && rs2_isNaN) {
        rs.rd = rs1;
    } else if(rs1_isNaN && !rs2_isNaN) {
        rs.rd = rs2;
    } else if(isBothZero) {
        if(hasNZero)
            rs.rd = 0x8000000000000000;
        else
            rs.rd = 0x0000000000000000;
    } else {
        rs.rd = res;
    }

    if((rs1_isNaN && ~(rs1 | 0xFFF7FFFFFFFFFFFF)) || (rs2_isNaN && ~(rs2 | 0xFFF7FFFFFFFFFFFF))){
       rs.exceptions = 0x10;
    }else{
       rs.exceptions = 0x00;
    }

    return rs;

}

//fmax_d
result rv_fmax_d(uint64_t rs1, uint64_t rs2){

    result rs;

    float64_t f_rs1;
    float64_t f_rs2;

    f_rs1.v = rs1;
    f_rs2.v = rs2;

    bool islt = f64_lt(f_rs1, f_rs2);
    bool isBothZero = (((rs1 & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000) && ((rs2 & 0x7FFFFFFFFFFFFFFF) == 0x0000000000000000));
    bool hasPZero = ((rs1 == 0x0000000000000000) || (rs2 == 0x0000000000000000));
    bool rs1_isNaN = (((rs1 & 0x7FF0000000000000) == 0x7FF0000000000000) && (rs1 & 0x000FFFFFFFFFFFFF));
    bool rs2_isNaN = (((rs2 & 0x7FF0000000000000) == 0x7FF0000000000000) && (rs2 & 0x000FFFFFFFFFFFFF));

    uint64_t res;

    if(islt)
        res = rs2;
    else
        res = rs1;
    if(rs1_isNaN && rs2_isNaN) {
        rs.rd = 0x7FF8000000000000;
    } else if(!rs1_isNaN && rs2_isNaN) {
        rs.rd = rs1;
    } else if(rs1_isNaN && !rs2_isNaN) {
        rs.rd = rs2;
    } else if(isBothZero) {
        if(hasPZero)
            rs.rd = 0x0000000000000000;
        else
            rs.rd = 0x8000000000000000;
    } else {
        rs.rd = res;
    }

    if((rs1_isNaN && ~(rs1 | 0xFFF7FFFFFFFFFFFF)) || (rs2_isNaN && ~(rs2 | 0xFFF7FFFFFFFFFFFF))){
       rs.exceptions = 0x10;
    }else{
       rs.exceptions = 0x00;
    }

    return rs;
}

//FPToFP End

void hector_wrapper()
{
    uint8_t rounding_mode, csr_frm, rm;
    uint8_t exceptions;
    uint64_t rs1, rs2, rd;
    result rs;

    int tagIn, TagOut, wflags, fcvt, typ, fmt, rfWen, fpWen, isBf16;
    int fuType;

    Hector::registerInput("rs1", &rs1, 8 * sizeof(rs1));
    Hector::registerInput("rs2", &rs2, 8 * sizeof(rs2));
    Hector::registerInput("rm",  &rm,  8 * sizeof(rm));
    Hector::registerInput("csr_frm"  , csr_frm);

    Hector::registerInput("tagIn", tagIn);
    Hector::registerInput("TagOut", TagOut);
    Hector::registerInput("wflags", wflags);
    Hector::registerInput("fcvt", fcvt);
    Hector::registerInput("typ", typ);
    Hector::registerInput("fmt", fmt);
    Hector::registerInput("rfWen", rfWen);
    Hector::registerInput("fpWen", fpWen);
    Hector::registerInput("isBf16", isBf16);

    Hector::registerInput("fuType", fuType);

    Hector::registerOutput("rd", &rd, 8 * sizeof(rd));
    Hector::registerOutput("exceptions", &exceptions, 8 * sizeof(exceptions));
    
    Hector::beginCapture();

    int low_tagIn = tagIn & 0b11;
    int low_TagOut = TagOut & 0b11;
    int low_wflags = wflags & 0b1;
    int low_fcvt = fcvt & 0b1;
    int low_typ = typ & 0b11;
    int low_fmt = fmt & 0b11;
    int low_rfWen = rfWen & 0b1;
    int low_fpWen = fpWen & 0b1;
    int low_rm = rounding_mode & 0b111;

    int sign =(low_rfWen << 9) | (low_fpWen << 8) | (low_tagIn << 7) | (low_TagOut << 6) |(low_wflags << 5) | (low_fcvt << 4) | (low_typ << 2) | low_fmt;

    rounding_mode = rm;

////FPToInt
//    if((fuType & 0x1f) == 0xB)
//    {
//      switch (sign & 0x3ff) {
//       case 0x2C0: rs = rv_fmv_x_w(rs1, rounding_mode);      break;
//       case 0x240: rs = rv_fclass_s(rs1, rounding_mode);     break;
//       case 0x270: rs = rv_fcvt_w_s(rs1, rounding_mode);     break;
//       case 0x274: rs = rv_fcvt_wu_s(rs1, rounding_mode);    break;
//       case 0x278: rs = rv_fcvt_l_s(rs1, rounding_mode);     break;
//       case 0x27C: rs = rv_fcvt_lu_s(rs1, rounding_mode);    break;
//
//       case 0x2C1: if(rounding_mode == 0x0){
//                     rs = rv_fmv_x_d(rs1, rounding_mode);
//                   }else if (rounding_mode == 0x1){
//                     rs = rv_fclass_d(rs1, rounding_mode);
//                   } break;
//       case 0x2F1: rs = rv_fcvt_w_d(rs1, rounding_mode);     break;
//       case 0x2F5: rs = rv_fcvt_wu_d(rs1, rounding_mode);    break;
//       case 0x2F9: rs = rv_fcvt_l_d(rs1, rounding_mode);     break;
//       case 0x2FD: rs = rv_fcvt_lu_d(rs1, rounding_mode);    break;
//       //Fcmp
//       case 0x260: if(rounding_mode == 0x2){
//                     rs = rv_feq_s(rs1, rs2, rounding_mode);
//                   }else if(rounding_mode == 0x1){
//                     rs = rv_flt_s(rs1, rs2, rounding_mode);    
//                   }else if (rounding_mode == 0x0){
//                     rs = rv_fle_s(rs1, rs2, rounding_mode);   
//                   } break;
//
//       case 0x2E1: if(rounding_mode == 0x2){ 
//                     rs = rv_feq_d(rs1, rs2, rounding_mode);    
//                   }else if(rounding_mode == 0x1){
//                     rs = rv_flt_d(rs1, rs2, rounding_mode);  
//                   }else if (rounding_mode == 0x0){
//                     rs = rv_fle_d(rs1, rs2, rounding_mode); 
//                   } break;
//      }
//    }
//
////FPToFP
//    if((fuType & 0x1f) == 0xA)
//    {  
//      switch (sign & 0x3ff) {
//       case 0x1B4: rs = rv_fcvt_s_d(rs1, rounding_mode);     break;
//       case 0x171: rs = rv_fcvt_d_s(rs1, rounding_mode);     break;
//
//       case 0x100: if(rounding_mode == 0x0){
//                     rs = rv_fsgnj_s(rs1, rs2);
//                   }else if(rounding_mode == 0x1){
//                     rs = rv_fsgnjn_s(rs1, rs2);
//                   }else if (rounding_mode == 0x2){
//                     rs = rv_fsgnjx_s(rs1, rs2);
//                   } break;
//
//       case 0x1C1: if(rounding_mode == 0x0){ 
//                     rs = rv_fsgnj_d(rs1, rs2);
//                   }else if(rounding_mode == 0x1){
//                     rs = rv_fsgnjn_d(rs1, rs2);
//                   }else if (rounding_mode == 0x2){
//                     rs = rv_fsgnjx_d(rs1, rs2);
//                   } break;
//
//       case 0x120: if(rounding_mode == 0x1){     
//                     rs = rv_fmax_s(rs1, rs2);     
//                   }else if (rounding_mode == 0x0){
//                     rs = rv_fmin_s(rs1, rs2);     
//                   } break;
//
//       case 0x1E1: if(rounding_mode == 0x1){
//                     rs = rv_fmax_d(rs1, rs2);
//                   }else if (rounding_mode == 0x0){
//                     rs = rv_fmin_d(rs1, rs2);
//                   } break;
//       }
//    }

    rd = 0;
    exceptions = 0;

    if((fcvt == 1) && (tagIn == 2) && (TagOut == 0) && (isBf16 == 0)) { rs = rv_fcvt_s_h(rs1); }
    if((fcvt == 1) && (tagIn == 0) && (TagOut == 1) && (isBf16 == 0)) { rs = rv_fcvt_d_s(rs1); }
    if((fcvt == 1) && (tagIn == 2) && (TagOut == 1) && (isBf16 == 0)) { rs = rv_fcvt_d_h(rs1); }
    if((fcvt == 1) && (tagIn == 0) && (TagOut == 2) && (isBf16 == 0)) { rs = rv_fcvt_h_s(rs1, rm); }
    if((fcvt == 1) && (tagIn == 1) && (TagOut == 0) && (isBf16 == 0)) { rs = rv_fcvt_s_d(rs1, rm); }
    if((fcvt == 1) && (tagIn == 1) && (TagOut == 2) && (isBf16 == 0)) { rs = rv_fcvt_h_d(rs1, rm); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 2) && (TagOut == 2) && (rm == 0) && (isBf16 == 0)) { rs = rv_fsgnj_h(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 2) && (TagOut == 2) && (rm == 1) && (isBf16 == 0)) { rs = rv_fsgnjn_h(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 2) && (TagOut == 2) && (rm == 2) && (isBf16 == 0)) { rs = rv_fsgnjx_h(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 0) && (TagOut == 0) && (rm == 0) && (isBf16 == 0)) { rs = rv_fsgnj_s(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 0) && (TagOut == 0) && (rm == 1) && (isBf16 == 0)) { rs = rv_fsgnjn_s(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 0) && (TagOut == 0) && (rm == 2) && (isBf16 == 0)) { rs = rv_fsgnjx_s(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 1) && (TagOut == 1) && (rm == 0) && (isBf16 == 0)) { rs = rv_fsgnj_d(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 1) && (TagOut == 1) && (rm == 1) && (isBf16 == 0)) { rs = rv_fsgnjn_d(rs1, rs2); }
    if((fcvt == 0) && (wflags == 0) && (tagIn == 1) && (TagOut == 1) && (rm == 2) && (isBf16 == 0)) { rs = rv_fsgnjx_d(rs1, rs2); }
    if((fpWen == 1) && (fcvt == 0) && (wflags == 1) && (tagIn == 2) && (TagOut == 2) && (rm == 0) && (isBf16 == 0)) { rs = rv_fmin_h(rs1, rs2); }
    if((fpWen == 1) && (fcvt == 0) && (wflags == 1) && (tagIn == 2) && (TagOut == 2) && (rm == 1) && (isBf16 == 0)) { rs = rv_fmax_h(rs1, rs2); }
    if((fpWen == 1) && (fcvt == 0) && (wflags == 1) && (tagIn == 0) && (TagOut == 0) && (rm == 0) && (isBf16 == 0)) { rs = rv_fmin_s(rs1, rs2); }
    if((fpWen == 1) && (fcvt == 0) && (wflags == 1) && (tagIn == 0) && (TagOut == 0) && (rm == 1) && (isBf16 == 0)) { rs = rv_fmax_s(rs1, rs2); }
    if((fpWen == 1) && (fcvt == 0) && (wflags == 1) && (tagIn == 1) && (TagOut == 1) && (rm == 0) && (isBf16 == 0)) { rs = rv_fmin_d(rs1, rs2); }
    if((fpWen == 1) && (fcvt == 0) && (wflags == 1) && (tagIn == 1) && (TagOut == 1) && (rm == 1) && (isBf16 == 0)) { rs = rv_fmax_d(rs1, rs2); }

    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 2) && (TagOut == 1) && (rm == 0) && (isBf16 == 0)) { rs = rv_fle_h(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 2) && (TagOut == 1) && (rm == 1) && (isBf16 == 0)) { rs = rv_flt_h(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 2) && (TagOut == 1) && (rm == 2) && (isBf16 == 0)) { rs = rv_feq_h(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 0) && (TagOut == 1) && (rm == 0) && (isBf16 == 0)) { rs = rv_fle_s(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 0) && (TagOut == 1) && (rm == 1) && (isBf16 == 0)) { rs = rv_flt_s(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 0) && (TagOut == 1) && (rm == 2) && (isBf16 == 0)) { rs = rv_feq_s(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 1) && (TagOut == 1) && (rm == 0) && (isBf16 == 0)) { rs = rv_fle_d(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 1) && (TagOut == 1) && (rm == 1) && (isBf16 == 0)) { rs = rv_flt_d(rs1, rs2, rm); }
    if((fpWen == 0) && (fcvt == 0) && (wflags == 1) && (tagIn == 1) && (TagOut == 1) && (rm == 2) && (isBf16 == 0)) { rs = rv_feq_d(rs1, rs2, rm); }

//    if((fcvt == 1) && (wflags == 1) && (tagIn == 2) && (typ == 0)) { rs = rv_fcvt_w_h(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 2) && (typ == 1)) { rs = rv_fcvt_wu_h(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 2) && (typ == 2)) { rs = rv_fcvt_l_h(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 2) && (typ == 3)) { rs = rv_fcvt_lu_h(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 0) && (typ == 0)) { rs = rv_fcvt_w_s(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 0) && (typ == 1)) { rs = rv_fcvt_wu_s(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 0) && (typ == 2)) { rs = rv_fcvt_l_s(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 0) && (typ == 3)) { rs = rv_fcvt_lu_s(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 1) && (typ == 0)) { rs = rv_fcvt_w_d(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 1) && (typ == 1)) { rs = rv_fcvt_wu_d(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 1) && (typ == 2)) { rs = rv_fcvt_l_d(rs1, rm); }
//    if((fcvt == 1) && (wflags == 1) && (tagIn == 1) && (typ == 3)) { rs = rv_fcvt_lu_d(rs1, rm); }
//
//    if((fcvt == 0) && (wflags == 0) && (rm == 1) && (tagIn == 2)) { rs = rv_fclass_h(rs1, rm); }
//    if((fcvt == 0) && (wflags == 0) && (rm == 1) && (tagIn == 0)) { rs = rv_fclass_s(rs1, rm); }
//    if((fcvt == 0) && (wflags == 0) && (rm == 1) && (tagIn == 1)) { rs = rv_fclass_d(rs1, rm); }
//
//    if((fcvt == 0) && (wflags == 0) && (rm == 0) && (fmt == 2)) { rs = rv_fmv_x_h(rs1, rm); }
//    if((fcvt == 0) && (wflags == 0) && (rm == 0) && (fmt == 0)) { rs = rv_fmv_x_w(rs1, rm); }
//    if((fcvt == 0) && (wflags == 0) && (rm == 0) && (fmt == 1)) { rs = rv_fmv_x_d(rs1, rm); }


    rd = rs.rd;
    exceptions = rs.exceptions;

    Hector::endCapture();
}
