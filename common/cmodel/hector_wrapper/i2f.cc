#include "Hector.h"
#include "softfloat.h"

//struct
struct result{
  uint64_t rd;
  uint8_t exceptions;
};

//Int to FP

//fmv_h_x
result rv_fmv_h_x(uint64_t rs1, uint8_t rounding_mode){
    result rs;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    uint16_t rs1Int16 = (uint16_t)rs1;

    rs.rd = rs1Int16 | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_h_w
result rv_fcvt_h_w(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float16_t f_rd;

    int32_t rs1Int32 = (int32_t)rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = i32_to_f16(rs1Int32);

    rs.rd = f_rd.v | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_h_wu
result rv_fcvt_h_wu(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float16_t f_rd;

    uint32_t rs1Int32 = (uint32_t)rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = ui32_to_f16(rs1Int32);

    rs.rd = f_rd.v | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_h_l
result rv_fcvt_h_l(int64_t rs1, uint8_t rounding_mode){
    result rs;
    float16_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = i64_to_f16(rs1);

    rs.rd = f_rd.v | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_h_lu
result rv_fcvt_h_lu(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float16_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = ui64_to_f16(rs1);

    rs.rd = f_rd.v | 0xFFFFFFFFFFFF0000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fmv_w_x
result rv_fmv_w_x(uint64_t rs1, uint8_t rounding_mode){
    result rs;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    uint32_t rs1Int32 = (uint32_t)rs1;

    rs.rd = rs1Int32 | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_s_w
result rv_fcvt_s_w(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float32_t f_rd;

    int32_t rs1Int32 = (int32_t)rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = i32_to_f32(rs1Int32);

    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_s_wu
result rv_fcvt_s_wu(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float32_t f_rd;

    uint32_t rs1Int32 = (uint32_t)rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = ui32_to_f32(rs1Int32);

    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_s_l
result rv_fcvt_s_l(int64_t rs1, uint8_t rounding_mode){
    result rs;
    float32_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = i64_to_f32(rs1);

    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_s_lu
result rv_fcvt_s_lu(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float32_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = ui64_to_f32(rs1);

    rs.rd = f_rd.v | 0xFFFFFFFF00000000;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fmv_d_x
result rv_fmv_d_x(uint64_t rs1, uint8_t rounding_mode){
    result rs;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    rs.rd = rs1;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_d_w
result rv_fcvt_d_w(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rd;

    int32_t rs1Int32 = (int32_t)rs1;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = i32_to_f64(rs1Int32);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_d_wu
result rv_fcvt_d_wu(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    uint32_t rs1Int32 = (uint32_t)rs1;

    f_rd = ui32_to_f64(rs1Int32);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}   

//fcvt_d_l
result rv_fcvt_d_l(int64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = i64_to_f64(rs1);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

//fcvt_d_lu
result rv_fcvt_d_lu(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float64_t f_rd;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = ui64_to_f64(rs1);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}


void hector_wrapper()
{
    uint8_t rounding_mode, csr_frm, rm;
    uint8_t exceptions;
    int64_t irs1;
    uint64_t uirs1, rd;
    result rs;

    int TagOut, wflags, typ, rfWen, fpWen;

    Hector::registerInput("uirs1", &uirs1, 8 * sizeof(uirs1));
    Hector::registerInput("irs1", &irs1, 8 * sizeof(irs1));
    Hector::registerInput("rm" , &rm , 8 * sizeof(rm));
    Hector::registerInput("csr_frm"  , csr_frm);
    
    Hector::registerInput("TagOut", TagOut);
    Hector::registerInput("wflags", wflags);
    Hector::registerInput("typ", typ);

    Hector::registerInput("rfWen", rfWen);
    Hector::registerInput("fpWen", fpWen);

    Hector::registerOutput("rd", &rd, 8 * sizeof(rd));
    Hector::registerOutput("exceptions", &exceptions, 8 * sizeof(exceptions));
    
    Hector::beginCapture();

    int low_TagOut = TagOut & 0b1;
    int low_wflags = wflags & 0b1;
    int low_typ = typ & 0b11;

    if(rm == 7){
       rounding_mode = csr_frm;
    }else{
       rounding_mode = rm;
    }

    int sign = (low_TagOut << 3) |(low_wflags << 2) | low_typ;

//    switch (sign & 0xf) {
//    case 0x0: rs = rv_fmv_w_x(uirs1, rounding_mode);      break;
//    case 0x4: rs = rv_fcvt_s_w(uirs1, rounding_mode);     break;
//    case 0x5: rs = rv_fcvt_s_wu(uirs1, rounding_mode);    break;
//    case 0x6: rs = rv_fcvt_s_l(irs1, rounding_mode);      break;
//    case 0x7: rs = rv_fcvt_s_lu(uirs1, rounding_mode);    break;
//    case 0x8: rs = rv_fmv_d_x(uirs1, rounding_mode);      break;
//    case 0xC: rs = rv_fcvt_d_w(uirs1, rounding_mode);     break;
//    case 0xD: rs = rv_fcvt_d_wu(uirs1, rounding_mode);    break;
//    case 0xE: rs = rv_fcvt_d_l(irs1, rounding_mode);      break;
//    case 0xF: rs = rv_fcvt_d_lu(uirs1, rounding_mode);    break;
//   }
    rd = 0x0;
    exceptions = 0x0;

    if((TagOut == 0x0) && (typ == 0x0) && (wflags == 0x0)){ rs = rv_fmv_w_x(uirs1, rounding_mode);}      
    if((TagOut == 0x0) && (typ == 0x0) && (wflags == 0x1)){ rs = rv_fcvt_s_w(uirs1, rounding_mode);}     
    if((TagOut == 0x0) && (typ == 0x1) && (wflags == 0x1)){ rs = rv_fcvt_s_wu(uirs1, rounding_mode);}    
    if((TagOut == 0x0) && (typ == 0x2) && (wflags == 0x1)){ rs = rv_fcvt_s_l(irs1, rounding_mode);}      
    if((TagOut == 0x0) && (typ == 0x3) && (wflags == 0x1)){ rs = rv_fcvt_s_lu(uirs1, rounding_mode);}    
    if((TagOut == 0x1) && (typ == 0x0) && (wflags == 0x0)){ rs = rv_fmv_d_x(uirs1, rounding_mode);}      
    if((TagOut == 0x1) && (typ == 0x0) && (wflags == 0x1)){ rs = rv_fcvt_d_w(uirs1, rounding_mode);}     
    if((TagOut == 0x1) && (typ == 0x1) && (wflags == 0x1)){ rs = rv_fcvt_d_wu(uirs1, rounding_mode);}    
    if((TagOut == 0x1) && (typ == 0x2) && (wflags == 0x1)){ rs = rv_fcvt_d_l(irs1, rounding_mode);}      
    if((TagOut == 0x1) && (typ == 0x3) && (wflags == 0x1)){ rs = rv_fcvt_d_lu(uirs1, rounding_mode);}    
    if((TagOut == 0x2) && (typ == 0x0) && (wflags == 0x0)){ rs = rv_fmv_h_x(uirs1, rounding_mode);}
    if((TagOut == 0x2) && (typ == 0x0) && (wflags == 0x1)){ rs = rv_fcvt_h_w(uirs1, rounding_mode);}
    if((TagOut == 0x2) && (typ == 0x1) && (wflags == 0x1)){ rs = rv_fcvt_h_wu(uirs1, rounding_mode);}
    if((TagOut == 0x2) && (typ == 0x2) && (wflags == 0x1)){ rs = rv_fcvt_h_l(irs1, rounding_mode);}
    if((TagOut == 0x2) && (typ == 0x3) && (wflags == 0x1)){ rs = rv_fcvt_h_lu(uirs1, rounding_mode);}

    rd = rs.rd;
    exceptions = rs.exceptions;



    Hector::endCapture();
}
