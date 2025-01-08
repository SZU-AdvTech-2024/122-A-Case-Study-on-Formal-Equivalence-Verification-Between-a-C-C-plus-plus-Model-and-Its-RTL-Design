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

//fcvt_s_bf16
result rv_fcvt_s_bf16(uint64_t rs1){
    result rs;
    uint16_t rs1Int16;
    bfloat16_t f_rs1;
    float32_t f_rd;

    rs1Int16 = (uint16_t)rs1;

    f_rs1.v = rs1Int16;

    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = bf16_to_f32(f_rs1);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}
//fcvt_bf16_s
result rv_fcvt_bf16_s(uint64_t rs1, uint8_t rounding_mode){
    result rs;
    float32_t f_rs1;
    bfloat16_t f_rd;
    uint32_t rs1Int32;

    rs1Int32 = (uint32_t)rs1;

    f_rs1.v = rs1Int32;

    softfloat_roundingMode = rounding_mode;
    softfloat_exceptionFlags = 0;
    softfloat_detectTininess = softfloat_tininess_afterRounding;

    f_rd = f32_to_bf16(f_rs1);

    rs.rd = f_rd.v;
    rs.exceptions = softfloat_exceptionFlags & 0x1f;

    return rs;
}

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


    rd = 0;
    exceptions = 0;

//    rs = rv_fcvt_s_bf16(rs1); 
    rs = rv_fcvt_bf16_s(rs1, rm); 

    rd = rs.rd;
    exceptions = rs.exceptions;

    Hector::endCapture();
}
