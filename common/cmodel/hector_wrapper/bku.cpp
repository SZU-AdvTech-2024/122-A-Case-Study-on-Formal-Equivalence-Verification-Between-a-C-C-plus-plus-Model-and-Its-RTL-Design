#include "Hector.h"
#include <stdio.h>
#include <math.h>
#include <ac_int.h>
#include <rac.h>
#include <aes_common.h>
#include <sm4_common.h>
#include <sha_common.h>
#include <string>
#include <vector>
using namespace std;

// RAC begin

typedef ac_int<2, false> ui2;
typedef ac_int<4, false> ui4;
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

// bku optype begin
ui64 rv_clmul(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rd_tmp = 0;
  int i;
  for(i=0;i<=63;i++){
  	if((rs2 >> i) & ui64(1)) {
  		rd_tmp = rd_tmp ^ (rs1 << i);
  	}
  }
  rd = rd_tmp;
  return rd;
}

ui64 rv_clmulh(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rd_tmp = 0;
  int i;
  for(i=0;i<=63;i++){
  	if((rs2 >> i) & ui64(1)) {
  		rd_tmp = rd_tmp ^ (rs1 >> (64-i));
  	}
  }
  rd = rd_tmp;
  return rd;
}

ui64 rv_clmulr(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 rd_tmp = 0;
  int i;
  for(i=0;i<=63;i++){
  	if((rs2 >> i) & ui64(1)) {
  		rd_tmp = rd_tmp ^ (rs1 >> (64-i-1));
  	}
  }
  rd = rd_tmp;
  return rd;
}

ui4 xperm4_lookup(ui4 idx, ui64 lut) {
  ui64 tmp;
  ui4 lookup;
  tmp = lut >> (idx*4);
  lookup = tmp.slc<4>(0);
  return lookup;
}

ui64 rv_xpermn(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 result = 0;
  int i;
  for(i=0;i<64;i=i+4){
  		result.set_slc(i,xperm4_lookup(rs2.slc<4>(i),rs1));
  }
  rd = result;
  return rd;
}

ui8 xperm8_lookup(ui8 idx, ui64 lut) {
  ui64 tmp;
  ui8 lookup;
  tmp = lut >> (idx*8);
  lookup = tmp.slc<8>(0);
  return lookup;
}

ui64 rv_xpermb(ui64 rs1, ui64 rs2) {
  ui64 rd;
  ui64 result = 0;
  int i;
  for(i=0;i<64;i=i+8){
  		result.set_slc(i,xperm8_lookup(rs2.slc<8>(i),rs1));
  }
  rd = result;
  return rd;
}

int HighestSetBit(ui64 x) {
  int i;
  int bit = -1;
  for(i=64-1;i>=0;i--){
  	if ((x[i] == 1) && (bit == -1)) {
      bit = i;
    }
  }
  return bit;
}

ui64 rv_clz(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = (64-1) - HighestSetBit(rs1);
  return rd;
}

int HighestSetBit32(ui64 x) {
  int i;
  int bit = -1;
  for(i=32-1;i>=0;i--){
  	if ((x[i] == 1) && (bit == -1)) {
      bit = i;
    }
  }
  return bit;
}

ui64 rv_clzw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = (32-1) - HighestSetBit32(rs1);
  return rd;
}

int LowestSetBit(ui64 x) {
  int i;
  int bit = 64;
  for(i=0;i<64;i++){
  	if ((x[i] == 1) && (bit == 64)) {
      bit = i;
    }
  }
  return bit;
}

ui64 rv_ctz(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = LowestSetBit(rs1);
  return rd;
}

int LowestSetBit32(ui64 x) {
  int i;
  int bit = 32;
  for(i=0;i<32;i++){
  	if ((x[i] == 1) && (bit == 32)) {
      bit = i;
    }
  }
  return bit;
}

ui64 rv_ctzw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  rd = LowestSetBit32(rs1);
  return rd;
}

ui64 rv_cpop(ui64 rs1, ui64 rs2) {
  ui64 rd;
  int i;
  int bitcount = 0;
  for(i=0;i<64;i++){
  	if (rs1[i] == 1) {
      bitcount = bitcount + 1;
    }
  }
  rd = bitcount;
  return rd;
}

ui64 rv_cpopw(ui64 rs1, ui64 rs2) {
  ui64 rd;
  int i;
  int bitcount = 0;
  for(i=0;i<32;i++){
  	if (rs1[i] == 1) {
      bitcount = bitcount + 1;
    }
  }
  rd = bitcount;
  return rd;
}

ui64 rv_aes64es(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint64_t temp;
  temp = AES_SHIFROWS_LO((uint64_t)rs1,(uint64_t)rs2);
  temp = (
    ((uint64_t)AES_ENC_SBOX[(temp >>  0) & 0xFF] <<  0) |
    ((uint64_t)AES_ENC_SBOX[(temp >>  8) & 0xFF] <<  8) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 16) & 0xFF] << 16) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 24) & 0xFF] << 24) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 32) & 0xFF] << 32) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 40) & 0xFF] << 40) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 48) & 0xFF] << 48) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 56) & 0xFF] << 56)
    );
  rd = temp;
  return rd;
}

ui64 rv_aes64esm(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint64_t temp;
  temp = AES_SHIFROWS_LO((uint64_t)rs1,(uint64_t)rs2);
  temp = (
    ((uint64_t)AES_ENC_SBOX[(temp >>  0) & 0xFF] <<  0) |
    ((uint64_t)AES_ENC_SBOX[(temp >>  8) & 0xFF] <<  8) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 16) & 0xFF] << 16) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 24) & 0xFF] << 24) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 32) & 0xFF] << 32) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 40) & 0xFF] << 40) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 48) & 0xFF] << 48) |
    ((uint64_t)AES_ENC_SBOX[(temp >> 56) & 0xFF] << 56)
    );

  uint64_t col_0 = temp & 0xFFFFFFFF;
  uint64_t col_1 = temp >> 32;

  col_0 = AES_MIXCOLUMN(col_0);
  col_1 = AES_MIXCOLUMN(col_1);

  rd = ((uint64_t)col_1 <<32) | col_0;
  return rd;
}

ui64 rv_aes64ds(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint64_t temp;
  temp = AES_INVSHIFROWS_LO((uint64_t)rs1,(uint64_t)rs2);
  temp = (
    ((uint64_t)AES_DEC_SBOX[(temp >>  0) & 0xFF] <<  0) |
    ((uint64_t)AES_DEC_SBOX[(temp >>  8) & 0xFF] <<  8) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 16) & 0xFF] << 16) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 24) & 0xFF] << 24) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 32) & 0xFF] << 32) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 40) & 0xFF] << 40) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 48) & 0xFF] << 48) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 56) & 0xFF] << 56)
	);
  rd = temp;
  return rd;
}

ui64 rv_aes64dsm(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint64_t temp;
  temp = AES_INVSHIFROWS_LO((uint64_t)rs1,(uint64_t)rs2);
  temp = (
    ((uint64_t)AES_DEC_SBOX[(temp >>  0) & 0xFF] <<  0) |
    ((uint64_t)AES_DEC_SBOX[(temp >>  8) & 0xFF] <<  8) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 16) & 0xFF] << 16) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 24) & 0xFF] << 24) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 32) & 0xFF] << 32) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 40) & 0xFF] << 40) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 48) & 0xFF] << 48) |
    ((uint64_t)AES_DEC_SBOX[(temp >> 56) & 0xFF] << 56)
	);
  uint64_t col_0 = temp & 0xFFFFFFFF;
  uint64_t col_1 = temp >> 32;

  col_0 = AES_INVMIXCOLUMN(col_0);
  col_1 = AES_INVMIXCOLUMN(col_1);

  rd = ((uint64_t)col_1 << 32) | col_0;
  return rd;
}

ui64 rv_aes64im(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint64_t col_0 = rs1 & 0xFFFFFFFF;
  uint64_t col_1 = rs1 >> 32;

  col_0 = AES_INVMIXCOLUMN(col_0);
  col_1 = AES_INVMIXCOLUMN(col_1);

  rd = ((uint64_t)col_1 << 32) | col_0;
  return rd;
}

ui64 rv_aes64ks1i(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint8_t enc_rcon = 0;
  uint8_t rcon = 0;
  uint32_t temp =(uint32_t)((uint64_t)rs1 >> 32)&0xffffffff;
  enc_rcon = (uint8_t)((uint64_t)rs2 & 0xf);	
  if (enc_rcon != 0xA) {
    temp    = (temp >> 8) | (temp << 24); // Rotate right by 8
    //rcon    = round_consts[enc_rcon];
    switch(enc_rcon){
      case 0 : rcon = 1;break;
      case 1 : rcon = 2;break;
      case 2 : rcon = 4;break;
      case 3 : rcon = 8;break;
      case 4 : rcon = 16;break;
      case 5 : rcon = 32;break;
      case 6 : rcon = 64;break;
      case 7 : rcon = 128;break;
      case 8 : rcon = 27;break;
      case 9 : rcon = 54;break;
      case 10 : rcon = 0;break;
      default: rcon = 0;break;
	}
  }
  temp        =
  ((uint32_t)AES_ENC_SBOX[(temp >> 24) & 0xFF] << 24) |
  ((uint32_t)AES_ENC_SBOX[(temp >> 16) & 0xFF] << 16) |
  ((uint32_t)AES_ENC_SBOX[(temp >>  8) & 0xFF] <<  8) |
  ((uint32_t)AES_ENC_SBOX[(temp >>  0) & 0xFF] <<  0) ;
  temp       ^= rcon;

  rd = ((uint64_t)temp << 32) | temp;
  return rd;
}

ui64 rv_aes64ks2(ui64 rs1, ui64 rs2) {
  ui64 rd;
  uint64_t rs1_lo = rs1 & 0xffffffff;
  uint64_t rs2_lo = rs2 & 0xffffffff;
  uint64_t rs1_high = (rs1>>32) & 0xffffffff;
  uint64_t rs2_high = (rs2>>32) & 0xffffffff;
  uint64_t temp1 = rs1_high ^ rs2_lo;
  uint64_t temp2 = temp1 ^ rs2_high;

  rd = (temp2<<32) | temp1;
  return rd;
}

ui64 rv_sm4ed(ui2 bs, ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t sb_in  = ((uint64_t)rs2 >> (8*(uint8_t)bs)) & 0xFF;
  uint32_t sb_out = (uint32_t)sm4_sbox[sb_in];
  
  uint32_t linear = sb_out ^  (sb_out         <<  8) ^ 
                              (sb_out         <<  2) ^
                              (sb_out         << 18) ^
                             ((sb_out & 0x3f) << 26) ^
                             ((sb_out & 0xC0) << 10) ;
  
  uint32_t rotl   = (linear << (8*bs)) | (linear >> (32-8*bs));
  
  uint32_t result = rotl ^ (uint64_t)rs1;

  rd = (uint64_t)((int32_t)result);
  return rd;
}

ui64 rv_sm4ks(ui2 bs, ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t sb_in  = ((uint64_t)rs2 >> (8*(uint8_t)bs)) & 0xFF;
  uint32_t sb_out = sm4_sbox[sb_in];
  
  uint32_t x      = sb_out ^
      ((sb_out & 0x07) << 29) ^ ((sb_out & 0xFE) <<  7) ^
      ((sb_out & 0x01) << 23) ^ ((sb_out & 0xF8) << 13) ;
  
  uint32_t rotl   = (x << (8*bs)) | (x >> (32-8*bs));
  
  uint32_t result = rotl ^ (uint64_t)rs1;

  rd = (uint64_t)((int32_t)result);
  return rd;
}

ui64 rv_sha256sum0(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t a = rs1;
  uint32_t result = ROR32(a, 2) ^ ROR32(a,13) ^ ROR32(a, 22);
  rd = (uint64_t)((int32_t)result);

  return rd;
}

ui64 rv_sha256sum1(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t a = rs1;
  uint32_t result = ROR32(a, 6) ^ ROR32(a,11) ^ ROR32(a, 25);
  rd = (uint64_t)((int32_t)result);

  return rd;
}

ui64 rv_sha256sig0(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t a = rs1;
  uint32_t result = ROR32(a, 7) ^ ROR32(a,18) ^ (a >> 3);
  rd = (uint64_t)((int32_t)result);

  return rd;
}

ui64 rv_sha256sig1(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t a = rs1;
  uint32_t result = ROR32(a, 17) ^ ROR32(a,19) ^ (a >> 10);
  rd = (uint64_t)((int32_t)result);

  return rd;
}

ui64 rv_sha512sum0(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint64_t a = rs1;
  rd = ROR64(a, 28) ^ ROR64(a,34) ^ ROR64(a,39);

  return rd;
}

ui64 rv_sha512sum1(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint64_t a = rs1;
  rd = ROR64(a, 14) ^ ROR64(a, 18) ^ ROR64(a, 41);

  return rd;
}

ui64 rv_sha512sig0(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint64_t a = rs1;
  rd = ROR64(a,  1) ^ ROR64(a, 8) ^ (a >>  7);

  return rd;
}

ui64 rv_sha512sig1(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint64_t a = rs1;
  rd = ROR64(a, 19) ^ ROR64(a,61) ^ (a >>  6);

  return rd;
}

ui64 rv_sm3p0(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t src = rs1;
  uint32_t result = src ^ ROL32(src, 9) ^ ROL32(src, 17);
  rd = (uint64_t)((int32_t)result);

  return rd;
}

ui64 rv_sm3p1(ui64 rs1, ui64 rs2) {
  ui64 rd;

  uint32_t src = rs1;
  uint32_t result = src ^ ROL32(src, 15) ^ ROL32(src, 23);
  rd = (uint64_t)((int32_t)result);

  return rd;
}
// bku optype end

void hector_wrapper()
{
    ui64 rs1, rs2;
    ui7  fuOpType;
    
    ui64 rd;

    Hector::registerInput("rs1", rs1);
    Hector::registerInput("rs2", rs2);
    Hector::registerInput("fuOpType", fuOpType);
    
    Hector::registerOutput("rd", rd);
    
    Hector::beginCapture();

    switch (fuOpType) {
    // bku optype
    case 0x00: rd = rv_clmul(rs1, rs2);          break;
    case 0x01: rd = rv_clmulh(rs1, rs2);         break;
    case 0x02: rd = rv_clmulr(rs1, rs2);         break;
    case 0x04: rd = rv_xpermn(rs1, rs2);         break;
    case 0x05: rd = rv_xpermb(rs1, rs2);         break;
    case 0x08: rd = rv_clz(rs1, rs2);            break;
    case 0x09: rd = rv_clzw(rs1, rs2);           break;
    case 0x0A: rd = rv_ctz(rs1, rs2);            break;
    case 0x0B: rd = rv_ctzw(rs1, rs2);           break;
    case 0x0C: rd = rv_cpop(rs1, rs2);           break;
    case 0x0D: rd = rv_cpopw(rs1, rs2);          break;
    case 0x20: rd = rv_aes64es(rs1, rs2);        break;
    case 0x21: rd = rv_aes64esm(rs1, rs2);       break;
    case 0x22: rd = rv_aes64ds(rs1, rs2);        break;
    case 0x23: rd = rv_aes64dsm(rs1, rs2);       break;
    case 0x24: rd = rv_aes64im(rs1, rs2);        break;
    case 0x25: rd = rv_aes64ks1i(rs1, rs2);      break;
    case 0x26: rd = rv_aes64ks2(rs1, rs2);       break;
    case 0x28: rd = rv_sm4ed(ui2(0x0), rs1, rs2);break;
    case 0x29: rd = rv_sm4ed(ui2(0x1), rs1, rs2);break;
    case 0x2A: rd = rv_sm4ed(ui2(0x2), rs1, rs2);break;
    case 0x2B: rd = rv_sm4ed(ui2(0x3), rs1, rs2);break;
    case 0x2C: rd = rv_sm4ks(ui2(0x0), rs1, rs2);break;
    case 0x2D: rd = rv_sm4ks(ui2(0x1), rs1, rs2);break;
    case 0x2E: rd = rv_sm4ks(ui2(0x2), rs1, rs2);break;
    case 0x2F: rd = rv_sm4ks(ui2(0x3), rs1, rs2);break;
    case 0x30: rd = rv_sha256sum0(rs1, rs2);     break;
    case 0x31: rd = rv_sha256sum1(rs1, rs2);     break;
    case 0x32: rd = rv_sha256sig0(rs1, rs2);     break;
    case 0x33: rd = rv_sha256sig1(rs1, rs2);     break;
    case 0x34: rd = rv_sha512sum0(rs1, rs2);     break;
    case 0x35: rd = rv_sha512sum1(rs1, rs2);     break;
    case 0x36: rd = rv_sha512sig0(rs1, rs2);     break;
    case 0x37: rd = rv_sha512sig1(rs1, rs2);     break;
    case 0x38: rd = rv_sm3p0(rs1, rs2);          break;
    case 0x39: rd = rv_sm3p1(rs1, rs2);          break;
    }
    
    Hector::endCapture();
}

