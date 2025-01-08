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
typedef ac_int<9, false> ui9;
typedef ac_int<10, false> ui10;
typedef ac_int<11, false> ui11;
typedef ac_int<12, false> ui12;
typedef ac_int<13, false> ui13;
typedef ac_int<16, false> ui16;
typedef ac_int<23, false> ui23;
typedef ac_int<24, false> ui24;
typedef ac_int<25, false> ui25;
typedef ac_int<26, false> ui26;
typedef ac_int<27, false> ui27;
typedef ac_int<28, false> ui28;
typedef ac_int<29, false> ui29;
typedef ac_int<32, false> ui32;
typedef ac_int<42, false> ui42;
typedef ac_int<52, false> ui52;
typedef ac_int<53, false> ui53;
typedef ac_int<54, false> ui54;
typedef ac_int<55, false> ui55;
typedef ac_int<56, false> ui56;
typedef ac_int<57, false> ui57;
typedef ac_int<59, false> ui59;
typedef ac_int<60, false> ui60;
typedef ac_int<64, false> ui64;
typedef ac_int<65, false> ui65;
typedef ac_int<66, false> ui66;
typedef ac_int<67, false> ui67;
typedef ac_int<68, false> ui68;
typedef ac_int<6, true> si6;
typedef ac_int<7, true> si7;
typedef ac_int<8, true> si8;
typedef ac_int<9, true> si9;
typedef ac_int<10, true> si10;
typedef ac_int<12, true> si12;
typedef ac_int<13, true> si13;
typedef ac_int<25, true> si25;
typedef ac_int<32, true> si32;
typedef ac_int<54, true> si54;
typedef ac_int<65, true> si65;


enum IterState {s_idle, s_pre_0, s_pre_1, s_iter, s_post_0, s_post_1, s_finish};

ui7 CLZ64(ui64 s) {
  ui64 x = s;
  array<bool, 64> z;
  array<ui6, 64> c;
  for (uint i=0; i<64; i++) {
    z[i] = !x[i];
    c[i] = 0;
  }
  uint n = 64;
  for (uint k=0; k<6; k++) {
    n = n/2; // n = 2^(5-k)
    for (uint i=0; i<n; i++) {
      c[i] = z[2*i+1] ? c[2*i] : c[2*i+1];
      c[i][k] = z[2*i+1];
      z[i] = z[2*i+1] && z[2*i];
    }
  }
  return c[0];
}

ui5 DetectSign(ui4 signs) {
  ui5 s;
  s[0] =  signs[0] &&  signs[1] &&  signs[2];
  s[1] = !signs[0] &&  signs[1] &&  signs[2];
  s[2] = !signs[1] &&  signs[2];
  s[3] = !signs[1] && !signs[2] &&  signs[3];
  s[4] = !signs[1] && !signs[2] && !signs[3];
  return s;
}

tuple<ui64, ui64> OTFC(ui5 q, ui64 quot, ui64 quotM1) {

  ui66 quotNext = 0;
  ui66 quotM1Next = 0;
  if (q == ui5(0x10)) {
    quotNext.set_slc(0,ui2(2));
    quotNext.set_slc(2,quot);
    quotM1Next.set_slc(0,ui2(1));
    quotM1Next.set_slc(2,quot);
  }
  if (q == ui5(0x08)) {
    quotNext.set_slc(0,ui2(1));
    quotNext.set_slc(2,quot);
    quotM1Next.set_slc(0,ui2(0));
    quotM1Next.set_slc(2,quot);
  }
  if (q == ui5(0x04)) {
    quotNext.set_slc(0,ui2(0));
    quotNext.set_slc(2,quot);
    quotM1Next.set_slc(0,ui2(3));
    quotM1Next.set_slc(2,quotM1);
  }
  if (q == ui5(0x02)) {
    quotNext.set_slc(0,ui2(3));
    quotNext.set_slc(2,quotM1);
    quotM1Next.set_slc(0,ui2(2));
    quotM1Next.set_slc(2,quotM1);
  }
  if (q == ui5(0x01)) {
    quotNext.set_slc(0,ui2(2));
    quotNext.set_slc(2,quotM1);
    quotM1Next.set_slc(0,ui2(1));
    quotM1Next.set_slc(2,quotM1);
  }

  return tuple<ui64, ui64>(quotNext.slc<64>(0),quotM1Next.slc<64>(0));
}

bool csa_sel(ui10 a, ui10 b, ui10 cin) {

  ui10 sum;
  ui10 cout;
  ui10 coutNext;
  ui10 signsSum;
  bool signsBit;
  ui10 a_xor_b;
  ui10 a_and_b;
  a_xor_b = a ^ b;
  a_and_b = a & b;
  sum     = a_xor_b ^ cin;
  cout    = a_and_b | (a_xor_b & cin);
  coutNext = 0;
  coutNext.set_slc(1,cout.slc<9>(0));
  signsSum = sum + coutNext;
  signsBit = signsSum[9];
  return signsBit;
}

tuple<ui13, ui13> csa_spec_s1(ui13 a, ui13 b, ui13 cin) {

  ui13 sum;
  ui13 cout;
  ui13 a_xor_b;
  ui13 a_and_b;
  a_xor_b = a ^ b;
  a_and_b = a & b;
  sum     = a_xor_b ^ cin;
  cout    = a_and_b | (a_xor_b & cin);
  return tuple<ui13, ui13>(sum, cout);
}

bool csa_spec_s2(ui13 a, ui13 b, ui13 cin) {

  ui13 sum;
  ui13 cout;
  ui13 coutNext;
  ui13 signsSum;
  bool signsBit;
  ui13 a_xor_b;
  ui13 a_and_b;
  a_xor_b = a ^ b;
  a_and_b = a & b;
  sum     = a_xor_b ^ cin;
  cout    = a_and_b | (a_xor_b & cin);
  coutNext = 0;
  coutNext.set_slc(1,cout.slc<12>(0));
  signsSum = sum + coutNext;
  signsBit = signsSum[12];
  return signsBit;
}

tuple<ui68, ui68> csa_iter68(ui68 a, ui68 b, ui68 cin) {

  ui68 sum;
  ui68 cout;
  ui68 a_xor_b;
  ui68 a_and_b;
  a_xor_b = a ^ b;
  a_and_b = a & b;
  sum     = a_xor_b ^ cin;
  cout    = a_and_b | (a_xor_b & cin);
  return tuple<ui68, ui68>(sum, cout);
}

ui4 SignSelection(ui10 r2ws, ui10 r2wc, ui5 qPrevReg, array<ui10, 4> rudPmNegN2, array<ui10, 4> rudPmNegN1, array<ui10, 4> rudPmNegZ0, array<ui10, 4> rudPmNegP1, array<ui10, 4> rudPmNegP2) {
  ui4 signs;
  ui10 rudPmNegToCsaTmp;
  array<ui10, 4> rudPmNegToCsa;
  for (uint i=0; i<4; i++) {
    rudPmNegToCsa[i] = 0;
    if (qPrevReg == ui5(0x01)) {
      rudPmNegToCsa[i] = rudPmNegN2[i];
    }
    if (qPrevReg == ui5(0x02)) {
      rudPmNegToCsa[i] = rudPmNegN1[i];
    }
    if (qPrevReg == ui5(0x04)) {
      rudPmNegToCsa[i] = rudPmNegZ0[i];
    }
    if (qPrevReg == ui5(0x08)) {
      rudPmNegToCsa[i] = rudPmNegP1[i];
    }
    if (qPrevReg == ui5(0x10)) {
      rudPmNegToCsa[i] = rudPmNegP2[i];
    }
    rudPmNegToCsaTmp = rudPmNegToCsa[i];
    signs[i] = csa_sel(r2ws,r2wc,rudPmNegToCsaTmp);
  }
  return signs;
}

ui4 Sign2Selection(ui13 r3sum, ui13 r3cout, ui5 qPrevReg, array<ui13, 4> r2udPmNegN2, array<ui13, 4> r2udPmNegN1, array<ui13, 4> r2udPmNegZ0, array<ui13, 4> r2udPmNegP1, array<ui13, 4> r2udPmNegP2) {
  ui4  signs;
  ui13 r2udPmNegToCsaTmp;
  array<ui13, 4> r2udPmNegToCsa;
  for (uint i=0; i<4; i++) {
    r2udPmNegToCsa[i] = 0;
    if (qPrevReg == ui5(0x01)) {
      r2udPmNegToCsa[i] = r2udPmNegN2[i];
    }
    if (qPrevReg == ui5(0x02)) {
      r2udPmNegToCsa[i] = r2udPmNegN1[i];
    }
    if (qPrevReg == ui5(0x04)) {
      r2udPmNegToCsa[i] = r2udPmNegZ0[i];
    }
    if (qPrevReg == ui5(0x08)) {
      r2udPmNegToCsa[i] = r2udPmNegP1[i];
    }
    if (qPrevReg == ui5(0x10)) {
      r2udPmNegToCsa[i] = r2udPmNegP2[i];
    }
    r2udPmNegToCsaTmp = r2udPmNegToCsa[i];
    signs[i] = csa_spec_s2(r3sum,r3cout,r2udPmNegToCsaTmp);
  }
  return signs;
}

tuple<ui12, ui12, ui12, ui12> SignExtmLookupTable(ui3 lookup) {

  ui12 s_mn1, s_mz0, s_mp1, s_mp2;
  si7 mn1, mz0, mp1, mp2;
  switch (lookup) {
  case 0: mn1 = 0x1A; mz0 = 0x04; mp1 = 0x7D; mp2 = 0x68; break;
  case 1: mn1 = 0x1E; mz0 = 0x06; mp1 = 0x7C; mp2 = 0x64; break;
  case 2: mn1 = 0x20; mz0 = 0x06; mp1 = 0x7C; mp2 = 0x62; break;
  case 3: mn1 = 0x24; mz0 = 0x06; mp1 = 0x7C; mp2 = 0x5E; break;
  case 4: mn1 = 0x26; mz0 = 0x08; mp1 = 0x7B; mp2 = 0x5C; break;
  case 5: mn1 = 0x2A; mz0 = 0x08; mp1 = 0x7A; mp2 = 0x58; break;
  case 6: mn1 = 0x2C; mz0 = 0x08; mp1 = 0x7A; mp2 = 0x56; break;
  case 7: mn1 = 0x30; mz0 = 0x08; mp1 = 0x7A; mp2 = 0x52;
  }
  s_mn1 = 0;
  s_mz0 = 0;
  s_mp1 = 0;
  s_mp2 = 0;
  s_mn1.set_slc(1,ui11(mn1));
  s_mz0.set_slc(2,ui10(mz0));
  s_mp1.set_slc(2,ui10(mp1));
  s_mp2.set_slc(1,ui11(mp2));
  return tuple<ui12, ui12, ui12, ui12>(s_mn1, s_mz0, s_mp1, s_mp2);
}

ui64 RightShifter(ui6 shiftNum, ui64 in, bool msb) {

  ui64 signExt;
  for (uint i=0; i<1; i++) {
      signExt[63-i] = msb;
  }
  signExt.set_slc(0,in.slc<63>(1));
  ui64 s0 = shiftNum[0] ? signExt : in;
    
  for (uint i=0; i<2; i++) {
      signExt[63-i] = msb;
  }
  signExt.set_slc(0,s0.slc<62>(2));
  ui64 s1 = shiftNum[1] ? signExt : s0;

  for (uint i=0; i<4; i++) {
      signExt[63-i] = msb;
  }
  signExt.set_slc(0,s1.slc<60>(4));
  ui64 s2 = shiftNum[2] ? signExt : s1;

  for (uint i=0; i<8; i++) {
      signExt[63-i] = msb;
  }
  signExt.set_slc(0,s2.slc<56>(8));
  ui64 s3 = shiftNum[3] ? signExt : s2;

  for (uint i=0; i<16; i++) {
      signExt[63-i] = msb;
  }
  signExt.set_slc(0,s3.slc<48>(16));
  ui64 s4 = shiftNum[4] ? signExt : s3;

  for (uint i=0; i<32; i++) {
      signExt[63-i] = msb;
  }
  signExt.set_slc(0,s4.slc<32>(32));
  ui64 s5 = shiftNum[5] ? signExt : s4;
    
  return s5;
}

ui64 SRT16Divider(ui64 a, ui64 d, bool isW , bool isHi, bool sign) {

  //s_idle:
  ui7  state = 1;
  
  ui64  quotIter;
  ui64  quotM1Iter;
  
  ui68  rNext;
  ui68  rNextPd;
  bool  finalIter = 1;
  
  bool aSign    = sign && a[63];
  bool dSign    = sign && d[63];
  bool dSignReg = dSign;

  ui64 aInverter = ui64(0) - a;
  ui64 dInverter = ui64(0) - d;
  ui64 aAbs    = aSign ? aInverter : a;
  ui64 dAbs    = dSign ? dInverter : d;
  ui64 aAbsReg = aAbs;
  ui64 dAbsReg = dAbs;

  ui7  aLZC  = CLZ64(aAbsReg);
  ui7  dLZC  = CLZ64(dAbsReg);
  ui64 aNorm = aAbsReg << aLZC;
  ui64 dNorm = dAbsReg << dLZC;
  ui64 aNormReg = aNorm;
  ui64 dNormReg = dNorm;
  ui7  lzcWireDiff = ui7(dLZC.slc<6>(0))    - ui7(aLZC.slc<6>(0));
  ui7  lzcRegDiff;
  ui7  aLZCReg;
  ui7  dLZCReg;

  
  //spcial case:
  ui64  aReg = a;

  bool dIsOne;
  if (dLZC.slc<6>(0) == ui6(0x3F)) {
    dIsOne = 1;
  } else {
    dIsOne = 0;
  }
  bool dIsZero;
  if (dNormReg != ui64(0)) {
    dIsZero = 0;
  } else {
    dIsZero = 1;
  }
  bool aIsOne    = aLZC[6];
  bool aTooSmall = aLZC[6] || lzcWireDiff[6];
  bool special   = dIsOne || dIsZero || aTooSmall;
  bool specialReg = special;
  ui64 aRegNeg   = ui64(0) - aReg;
  
  ui64 quotSpecial    = dIsZero ? ui64(0xFFFFFFFFFFFFFFFF) : (aTooSmall ? ui64(0) : ( dSignReg ? aRegNeg : aReg));
  ui64 remSpecial     = (dIsZero || aTooSmall) ? aReg : ui64(0);
  ui64 quotSpecialReg = quotSpecial;
  ui64 remSpecialReg  = remSpecial;

  Hector::show("dLZC", dLZC);
  Hector::show("aLZC", aLZC);
  Hector::show("dNormReg", dNormReg);
  Hector::show("lzcWireDiff", lzcWireDiff);
  Hector::show("quotSpecial", quotSpecial);
  Hector::show("remSpecial", remSpecial);
  Hector::show("aRegNeg", aRegNeg);
  Hector::show("special", special);
  Hector::show("dIsOne", dIsOne);
  Hector::show("dIsZero", dIsZero);
  Hector::show("aIsOne", aIsOne);
  Hector::show("aTooSmall", aTooSmall);

  //s_pre_0:
  aInverter = ui64(0) - a;
  dInverter = ui64(0) - d;
  aAbs    = aSign ? aInverter : a;
  dAbs    = dSign ? dInverter : d;
  aAbsReg = aAbs;
  dAbsReg = dAbs;

  aLZC    = CLZ64(aAbsReg);
  dLZC    = CLZ64(dAbsReg);
  aNorm   = aAbsReg << aLZC;
  dNorm   = dAbsReg << dLZC;
  aNormReg = aNorm;
  dNormReg = dNorm;
  aLZCReg = aLZC;
  dLZCReg = dLZC;
  lzcWireDiff = ui7(dLZC.slc<6>(0))    - ui7(aLZC.slc<6>(0));
  lzcRegDiff  = ui7(dLZCReg.slc<6>(0)) - ui7(aLZCReg.slc<6>(0));

  Hector::show("dLZC", dLZC);
  Hector::show("aLZC", aLZC);
  Hector::show("dNormReg", dNormReg);
  Hector::show("lzcWireDiff", lzcWireDiff);
  
  //s_pre_1
  bool quotSignReg = aSign ^ dSign;
  bool rSign = aSign;
  bool rSignReg = rSign;
  
  bool rShift  = lzcRegDiff[0];
  bool oddIter = lzcRegDiff[1] ^ lzcRegDiff[0];
  ui4  iterNum = ui4((lzcRegDiff + ui7(1)) >> 2);

  ui65 rSumInitA   = 0;
  rSumInitA.set_slc(1,aNormReg);
  ui65 rSumInitB   = 0;
  rSumInitB.set_slc(0,aNormReg);
  ui65 rSumInitTmp = rShift ? rSumInitB : rSumInitA;
  ui68 rSumInit  = 0;
  rSumInit.set_slc(0,rSumInitTmp);
  ui68 rCarryInit = 0;
  
  ui5  rSumInitTrunc  = 0;
  rSumInitTrunc.set_slc(0,rSumInit.slc<4>(61));

  ui5 mInitPos1, mInitPos2;
  switch (dNormReg.slc<3>(60)) {
  case 0: mInitPos1 = 0x04; mInitPos2 = 0x0C; break;
  case 1: mInitPos1 = 0x04; mInitPos2 = 0x0E; break;
  case 2: mInitPos1 = 0x04; mInitPos2 = 0x0F; break;
  case 3: mInitPos1 = 0x06; mInitPos2 = 0x10; break;
  case 4: mInitPos1 = 0x06; mInitPos2 = 0x12; break;
  case 5: mInitPos1 = 0x06; mInitPos2 = 0x14; break;
  case 6: mInitPos1 = 0x06; mInitPos2 = 0x16; break;
  case 7: mInitPos1 = 0x08; mInitPos2 = 0x16;
  }

  bool initCmpPos1, initCmpPos2;
  if ( rSumInitTrunc >= mInitPos1 ) {
   initCmpPos1 = 1;
  } else {
   initCmpPos1 = 0;
  }
  if ( rSumInitTrunc >= mInitPos2 ) {
   initCmpPos2 = 1;
  } else {
   initCmpPos2 = 0;
  }

  ui5 quot_neg_2 = 0x1;
  ui5 quot_neg_1 = 0x2;
  ui5 quot_0     = 0x4;
  ui5 quot_pos_1 = 0x8;
  ui5 quot_pos_2 = 0x10;
  
  ui5 qInit = initCmpPos2 ? quot_pos_2 : (initCmpPos1 ? quot_pos_1 : quot_0); 

  ui65 dPos = 0;
  dPos.set_slc(0,dNormReg);
  ui65 dNeg = ui65(0) - dPos;

  //Vec
  ui12 mNegN1;
  ui12 mNegZ0;
  ui12 mNegP1;
  ui12 mNegP2;
  
  ui10 rudNegN2;
  ui10 rudNegN1;
  ui10 rudNegZ0;
  ui10 rudNegP1;
  ui10 rudNegP2;
  
  ui12 r2udNegN2;
  ui12 r2udNegN1;
  ui12 r2udNegZ0;
  ui12 r2udNegP1;
  ui12 r2udNegP2;
  
  ui68 udNegN2;
  ui68 udNegN1;
  ui68 udNegZ0;
  ui68 udNegP1;
  ui68 udNegP2;
  
  ui5 qSpecN2;
  ui5 qSpecN1;
  ui5 qSpecZ0;
  ui5 qSpecP1;
  ui5 qSpecP2;

  array<ui10, 4> rudPmNegN2;
  array<ui10, 4> rudPmNegN1;
  array<ui10, 4> rudPmNegZ0;
  array<ui10, 4> rudPmNegP1;
  array<ui10, 4> rudPmNegP2;

  array<ui13, 4> r2udPmNegN2;
  array<ui13, 4> r2udPmNegN1;
  array<ui13, 4> r2udPmNegZ0;
  array<ui13, 4> r2udPmNegP1;
  array<ui13, 4> r2udPmNegP2;

  ui3 dForLookup = dPos.slc<3>(60);
  tie(mNegN1, mNegZ0, mNegP1, mNegP2) = SignExtmLookupTable(dNormReg.slc<3>(60));

  udNegN2 = 0; 
  udNegN1 = 0; 
  udNegZ0 = 0; 
  udNegP1 = 0; 
  udNegP2 = 0; 
  udNegN2.set_slc(2,ui66(si65(dPos)));
  udNegN1.set_slc(1,ui67(si65(dPos)));
  udNegZ0 = 0;
  udNegP1.set_slc(1,ui67(si65(dNeg)));
  udNegP2.set_slc(2,ui66(si65(dNeg)));

  rudNegN2 = udNegN2.slc<10>(57); 
  rudNegN1 = udNegN1.slc<10>(57); 
  rudNegZ0 = udNegZ0.slc<10>(57); 
  rudNegP1 = udNegP1.slc<10>(57); 
  rudNegP2 = udNegP2.slc<10>(57); 

  r2udNegN2 = udNegN2.slc<12>(55); 
  r2udNegN1 = udNegN1.slc<12>(55); 
  r2udNegZ0 = udNegZ0.slc<12>(55); 
  r2udNegP1 = udNegP1.slc<12>(55); 
  r2udNegP2 = udNegP2.slc<12>(55); 
  
  rudPmNegN2[0] = ui10(si9(rudNegN2.slc<9>(1))) + mNegN1.slc<10>(1);
  rudPmNegN2[1] = ui10(si9(rudNegN2.slc<9>(1))) + mNegZ0.slc<10>(1);
  rudPmNegN2[2] = ui10(si9(rudNegN2.slc<9>(1))) + mNegP1.slc<10>(1);
  rudPmNegN2[3] = ui10(si9(rudNegN2.slc<9>(1))) + mNegP2.slc<10>(1);

  rudPmNegN1[0] = ui10(si9(rudNegN1.slc<9>(1))) + mNegN1.slc<10>(1);
  rudPmNegN1[1] = ui10(si9(rudNegN1.slc<9>(1))) + mNegZ0.slc<10>(1);
  rudPmNegN1[2] = ui10(si9(rudNegN1.slc<9>(1))) + mNegP1.slc<10>(1);
  rudPmNegN1[3] = ui10(si9(rudNegN1.slc<9>(1))) + mNegP2.slc<10>(1);

  rudPmNegZ0[0] = ui10(si9(rudNegZ0.slc<9>(1))) + mNegN1.slc<10>(1);
  rudPmNegZ0[1] = ui10(si9(rudNegZ0.slc<9>(1))) + mNegZ0.slc<10>(1);
  rudPmNegZ0[2] = ui10(si9(rudNegZ0.slc<9>(1))) + mNegP1.slc<10>(1);
  rudPmNegZ0[3] = ui10(si9(rudNegZ0.slc<9>(1))) + mNegP2.slc<10>(1);

  rudPmNegP1[0] = ui10(si9(rudNegP1.slc<9>(1))) + mNegN1.slc<10>(1);
  rudPmNegP1[1] = ui10(si9(rudNegP1.slc<9>(1))) + mNegZ0.slc<10>(1);
  rudPmNegP1[2] = ui10(si9(rudNegP1.slc<9>(1))) + mNegP1.slc<10>(1);
  rudPmNegP1[3] = ui10(si9(rudNegP1.slc<9>(1))) + mNegP2.slc<10>(1);

  rudPmNegP2[0] = ui10(si9(rudNegP2.slc<9>(1))) + mNegN1.slc<10>(1);
  rudPmNegP2[1] = ui10(si9(rudNegP2.slc<9>(1))) + mNegZ0.slc<10>(1);
  rudPmNegP2[2] = ui10(si9(rudNegP2.slc<9>(1))) + mNegP1.slc<10>(1);
  rudPmNegP2[3] = ui10(si9(rudNegP2.slc<9>(1))) + mNegP2.slc<10>(1);

  r2udPmNegN2[0] = ui13(si12(r2udNegN2)) + ui13(si12(mNegN1));
  r2udPmNegN2[1] = ui13(si12(r2udNegN2)) + ui13(si12(mNegZ0));
  r2udPmNegN2[2] = ui13(si12(r2udNegN2)) + ui13(si12(mNegP1));
  r2udPmNegN2[3] = ui13(si12(r2udNegN2)) + ui13(si12(mNegP2));

  r2udPmNegN1[0] = ui13(si12(r2udNegN1)) + ui13(si12(mNegN1));
  r2udPmNegN1[1] = ui13(si12(r2udNegN1)) + ui13(si12(mNegZ0));
  r2udPmNegN1[2] = ui13(si12(r2udNegN1)) + ui13(si12(mNegP1));
  r2udPmNegN1[3] = ui13(si12(r2udNegN1)) + ui13(si12(mNegP2));

  r2udPmNegZ0[0] = ui13(si12(r2udNegZ0)) + ui13(si12(mNegN1));
  r2udPmNegZ0[1] = ui13(si12(r2udNegZ0)) + ui13(si12(mNegZ0));
  r2udPmNegZ0[2] = ui13(si12(r2udNegZ0)) + ui13(si12(mNegP1));
  r2udPmNegZ0[3] = ui13(si12(r2udNegZ0)) + ui13(si12(mNegP2));

  r2udPmNegP1[0] = ui13(si12(r2udNegP1)) + ui13(si12(mNegN1));
  r2udPmNegP1[1] = ui13(si12(r2udNegP1)) + ui13(si12(mNegZ0));
  r2udPmNegP1[2] = ui13(si12(r2udNegP1)) + ui13(si12(mNegP1));
  r2udPmNegP1[3] = ui13(si12(r2udNegP1)) + ui13(si12(mNegP2));

  r2udPmNegP2[0] = ui13(si12(r2udNegP2)) + ui13(si12(mNegN1));
  r2udPmNegP2[1] = ui13(si12(r2udNegP2)) + ui13(si12(mNegZ0));
  r2udPmNegP2[2] = ui13(si12(r2udNegP2)) + ui13(si12(mNegP1));
  r2udPmNegP2[3] = ui13(si12(r2udNegP2)) + ui13(si12(mNegP2));

  ui10 r2ws;
  ui10 r2wc;
  ui13 r3ws;
  ui13 r3wc;

  ui4  signs;
  ui5  qNext;
  ui5  qNext2;
  ui68 rCarryIter;
  ui68 rSumIter;

  ui64 quotIterFinal = 0;
  ui64 quotM1IterFinal = 0;
  ui68 rSumIterFinal = 0;
  ui68 rCarryIterFinal = 0;

  ui5  qPrevReg;
  ui64 quotIterReg;
  ui64 quotM1IterReg;
  ui68 rSumReg;
  ui68 rCarryReg;
  ui64 quotOddIter;
  ui64 quotM1OddIter;
  ui68 rSumOddIter;
  ui68 rCarryOddIter;

  //midState var declaration
  Hector::show("qPrevReg", qPrevReg);
  Hector::show("quotIterReg", quotIterReg);
  Hector::show("quotM1IterReg", quotM1IterReg);
  Hector::show("rSumReg", rSumReg);
  Hector::show("rCarryReg", rCarryReg);
  Hector::show("quotOddIter", quotOddIter);
  Hector::show("quotM1OddIter", quotM1OddIter);
  Hector::show("rSumOddIter", rSumOddIter);
  Hector::show("rCarryOddIter", rCarryOddIter);

  //midState var 0 step
  Hector::show("qPrevReg", qPrevReg);
  Hector::show("quotIterReg", quotIterReg);
  Hector::show("quotM1IterReg", quotM1IterReg);
  Hector::show("rSumReg", rSumReg);
  Hector::show("rCarryReg", rCarryReg);
  Hector::show("quotOddIter", quotOddIter);
  Hector::show("quotM1OddIter", quotM1OddIter);
  Hector::show("rSumOddIter", rSumOddIter);
  Hector::show("rCarryOddIter", rCarryOddIter);


  
  for (uint j = 0; j < 8; j++) {

    if (j == 0) {
      state = 4;
    } else {
      state = 8;
    }

    qPrevReg      = state[s_pre_1] ? qInit      : qNext2;
    rSumReg       = state[s_pre_1] ? rSumInit   : rSumIter;
    rCarryReg     = state[s_pre_1] ? rCarryInit : rCarryIter;
    quotIterReg   = state[s_pre_1] ? ui64(0)    : quotIter;
    quotM1IterReg = state[s_pre_1] ? ui64(0)    : quotM1Iter;

    r3ws = rSumReg.slc<13>(55); 
    r3wc = rCarryReg.slc<13>(55); 

    r2ws = rSumReg.slc<10>(58); 
    r2wc = rCarryReg.slc<10>(58); 
 
    ui4 signs = SignSelection(r2ws, r2wc, qPrevReg, rudPmNegN2, rudPmNegN1, rudPmNegZ0, rudPmNegP1, rudPmNegP2);
    qNext  = DetectSign(signs);

    ui68 rSumRegShft   = rSumReg << 2;
    ui68 rCarryRegShft = rCarryReg << 2;
    ui68 Mux1HqPrevReg = 0;
    if (qPrevReg == ui5(0x01)) {
      Mux1HqPrevReg = udNegN2;
    }
    if (qPrevReg == ui5(0x02)) {
      Mux1HqPrevReg = udNegN1;
    }
    if (qPrevReg == ui5(0x04)) {
      Mux1HqPrevReg = udNegZ0;
    }
    if (qPrevReg == ui5(0x08)) {
      Mux1HqPrevReg = udNegP1;
    }
    if (qPrevReg == ui5(0x10)) {
      Mux1HqPrevReg = udNegP2;
    }
    Mux1HqPrevReg = Mux1HqPrevReg << 2;
    ui68 rSumHalfIter, rCarryHalfIterTmp;
    tie(rSumHalfIter, rCarryHalfIterTmp) = csa_iter68(rSumRegShft, rCarryRegShft, Mux1HqPrevReg);
    ui68 rSumIterTmpShft = rSumHalfIter << 2;
    ui68 rCarryHalfIter = rCarryHalfIterTmp << 1;
    ui68 rCarryIterTmpShft2 = rCarryHalfIter << 2;
    ui68 Mux1HqNext = 0;
    if (qNext == ui5(0x01)) {
      Mux1HqNext = udNegN2;
    }
    if (qNext == ui5(0x02)) {
      Mux1HqNext = udNegN1;
    }
    if (qNext == ui5(0x04)) {
      Mux1HqNext = udNegZ0;
    }
    if (qNext == ui5(0x08)) {
      Mux1HqNext = udNegP1;
    }
    if (qNext == ui5(0x10)) {
      Mux1HqNext = udNegP2;
    }
    Mux1HqNext = Mux1HqNext << 2;
    ui68 rSumIterNext, rCarryIterNextTmp;
    tie(rSumIterNext, rCarryIterNextTmp) = csa_iter68(rSumIterTmpShft, rCarryIterTmpShft2, Mux1HqNext);
    ui68 rCarryIterNext = rCarryIterNextTmp << 1;
    rSumIter   = rSumIterNext;
    rCarryIter = rCarryIterNext;

    ui13 sum,cout,coutShft,signExtudNegReg;
    ui4  sign2;
    //qSpecN2...
    signExtudNegReg = ui13(si10(udNegN2.slc<10>(57)));
    tie(sum, cout) = csa_spec_s1(r3ws, r3wc, signExtudNegReg);
    coutShft = cout << 1;
    sign2 = Sign2Selection(sum, coutShft, qPrevReg, r2udPmNegN2, r2udPmNegN1, r2udPmNegZ0, r2udPmNegP1, r2udPmNegP2);
    qSpecN2 = DetectSign(sign2);
    //qSpecN1...
    signExtudNegReg = ui13(si10(udNegN1.slc<10>(57)));
    tie(sum, cout) = csa_spec_s1(r3ws, r3wc, signExtudNegReg);
    coutShft = cout << 1;
    sign2 = Sign2Selection(sum, coutShft, qPrevReg, r2udPmNegN2, r2udPmNegN1, r2udPmNegZ0, r2udPmNegP1, r2udPmNegP2);
    qSpecN1 = DetectSign(sign2);
    //qSpecZ0...
    signExtudNegReg = ui13(si10(udNegZ0.slc<10>(57)));
    tie(sum, cout) = csa_spec_s1(r3ws, r3wc, signExtudNegReg);
    coutShft = cout << 1;
    sign2 = Sign2Selection(sum, coutShft, qPrevReg, r2udPmNegN2, r2udPmNegN1, r2udPmNegZ0, r2udPmNegP1, r2udPmNegP2);
    qSpecZ0 = DetectSign(sign2);
    //qSpecP1...
    signExtudNegReg = ui13(si10(udNegP1.slc<10>(57)));
    tie(sum, cout) = csa_spec_s1(r3ws, r3wc, signExtudNegReg);
    coutShft = cout << 1;
    sign2 = Sign2Selection(sum, coutShft, qPrevReg, r2udPmNegN2, r2udPmNegN1, r2udPmNegZ0, r2udPmNegP1, r2udPmNegP2);
    qSpecP1 = DetectSign(sign2);
    //qSpecP2...
    signExtudNegReg = ui13(si10(udNegP2.slc<10>(57)));
    tie(sum, cout) = csa_spec_s1(r3ws, r3wc, signExtudNegReg);
    coutShft = cout << 1;
    sign2 = Sign2Selection(sum, coutShft, qPrevReg, r2udPmNegN2, r2udPmNegN1, r2udPmNegZ0, r2udPmNegP1, r2udPmNegP2);
    qSpecP2 = DetectSign(sign2);
 
    qNext2 = 0;
    if (qNext == ui5(0x01)) {
      qNext2 = qSpecN2;
    }
    if (qNext == ui5(0x02)) {
      qNext2 = qSpecN1;
    }
    if (qNext == ui5(0x04)) {
      qNext2 = qSpecZ0;
    }
    if (qNext == ui5(0x08)) {
      qNext2 = qSpecP1;
    }
    if (qNext == ui5(0x10)) {
      qNext2 = qSpecP2;
    }
   
    ui64 quotHalfIter,quotM1HalfIter,quotIterNext,quotM1IterNext;
    tie(quotHalfIter, quotM1HalfIter) = OTFC(qPrevReg, quotIterReg, quotM1IterReg);
    tie(quotIterNext, quotM1IterNext) = OTFC(qNext, quotHalfIter, quotM1HalfIter);
    quotIter    = quotIterNext;
    quotM1Iter  = quotM1IterNext;

    rSumOddIter    = (!oddIter) ? rSumHalfIter   : rSumIterNext;
    rCarryOddIter  = (!oddIter) ? rCarryHalfIter : rCarryIterNext;

    quotOddIter    = (!oddIter) ? quotHalfIter   : quotIterNext;
    quotM1OddIter  = (!oddIter) ? quotM1HalfIter : quotM1IterNext;

    if (iterNum == ui4(j)) {
      rSumIterFinal    = rSumOddIter;
      rCarryIterFinal  = rCarryOddIter;

      quotIterFinal    = quotOddIter;
      quotM1IterFinal  = quotM1OddIter;
    }

    //midState var j step
    Hector::show("qPrevReg", qPrevReg);
    Hector::show("quotIterReg", quotIterReg);
    Hector::show("quotM1IterReg", quotM1IterReg);
    Hector::show("rSumReg", rSumReg);
    Hector::show("rCarryReg", rCarryReg);
    Hector::show("quotOddIter", quotOddIter);
    Hector::show("quotM1OddIter", quotM1OddIter);
    Hector::show("rSumOddIter", rSumOddIter);
    Hector::show("rCarryOddIter", rCarryOddIter);

  }

  rSumIter   = rSumIterFinal;
  rCarryIter = rCarryIterFinal;

  quotIter    = quotIterFinal;
  quotM1Iter  = quotM1IterFinal;

  //post_0
  ui64 quotIterNeg   = ui64(0) - quotIter;
  ui64 quotM1IterNeg = ui64(0) - quotM1Iter;
  quotIterReg   = quotSignReg ? quotIterNeg   : quotIter;
  quotM1IterReg = quotSignReg ? quotM1IterNeg : quotM1Iter;
  rSumReg       = rSumIter;
  rCarryReg     = rCarryIter;
 
  ui68 dNormRegTmp = 0; 
  dNormRegTmp.set_slc(3,dNormReg);
  if (rSignReg) {
    rNext   = ~rSumReg + ~rCarryReg + ui68(2); 
    rNextPd = ~rSumReg + ~rCarryReg + ~dNormRegTmp + ui68(3);
  } else {
    rNext   = rSumReg + rCarryReg; 
    rNextPd = rSumReg + rCarryReg + dNormRegTmp;
  }

  ui65 rNextReg    = rNext.slc<65>(3);
  ui65 rNextPdReg  = rNextPd.slc<65>(3);

  //post_1  
  ui65 r   = rNextReg;
  ui65 rPd = rNextPdReg;
  bool rIsZero = !(r != ui65(0));
  bool needCorr = rSignReg ? (!r[64] && (r != ui65(0))) : r[64];
  ui65 rPreShifted = needCorr ? rPd : r;
  ui64 rShifted = RightShifter(ui6(dLZCReg), ui64(rPreShifted), (!(rPreShifted != ui65(0)) ? 0 : rSignReg));
  ui64 rFinal = specialReg ? remSpecialReg  : rShifted;
  ui64 qFinal = specialReg ? quotSpecialReg : (needCorr ? quotM1IterReg : quotIterReg);
  ui64 res    = isHi ? rFinal : qFinal;
  ui64 out    = isW  ? ui64(si32(res.slc<32>(0))) : res;

  Hector::show("rSignReg", rSignReg);
  Hector::show("dLZCReg", dLZCReg);
  Hector::show("specialReg", specialReg);
  Hector::show("remSpecialReg", remSpecialReg);
  Hector::show("quotSpecialReg", quotSpecialReg);

  return out;
}

ui64 iDiv(ui64 src1, ui64 src2, ui7 func) {
  
  ui64 SignExt, ZeroExt;
  ui64 in1, in2;

  bool isW = func[2];
  bool isH = (func[4] && func[0]) || (!func[4] && (func.slc<2>(0) != ui2(0)));
  bool isDivSign = func[4] && !func[1];

  SignExt = ui64(si32(src1.slc<32>(0)));
  ZeroExt = ui64(src1.slc<32>(0));
  in1 = isW ? ( isDivSign ? SignExt : ZeroExt ) : src1;
  SignExt = ui64(si32(src2.slc<32>(0)));
  ZeroExt = ui64(src2.slc<32>(0));
  in2 = isW ? ( isDivSign ? SignExt : ZeroExt ) : src2;

  Hector::show("isW", isW);
  Hector::show("isH", isH);
  Hector::show("isDivSign", isDivSign);
  Hector::show("SignExt", SignExt);
  Hector::show("ZeroExt", ZeroExt);
  Hector::show("in1", in1);
  Hector::show("in2", in2);

  ui64 out = SRT16Divider(in1, in2, isW, isH, isDivSign);

  return out;

}

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

    rd = iDiv(rs1, rs2, fuOpType);
    
    Hector::endCapture();
}

