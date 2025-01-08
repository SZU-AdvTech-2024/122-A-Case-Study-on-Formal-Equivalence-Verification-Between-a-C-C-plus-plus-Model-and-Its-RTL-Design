#========================================================
#    Setting  variable
#========================================================
set _hector_comp_use_new_flow true
set _hector_softfloat_version custom
set _hector_multiple_solve_scripts true

set_host_file "../../scr/flow_com/host.qsub"

#========================================================
#    compile_spec
#========================================================

proc compile_spec {} {


        create_design -name spec -top hector_wrapper
        cppan \
              -I ../../../common/cmodel/acl2/include \
              ../../../common/cmodel/hector_wrapper/v_reduction.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VMacExu -clock clock

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

for {set i 0} {$i <= 10} {incr i} {
   if {$i < 3} {
      assume impl.reset($i) == 1'b1
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
   } elseif {$i < 5} {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
   } elseif {$i < 7} {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b1
   } else {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
   }
}

assume spec.vs1(1)     == impl.auto_issue_in_issue_bits_src_0(5)
assume spec.vs2(1)     == impl.auto_issue_in_issue_bits_src_1(5)
assume spec.oldvd(1)   == impl.auto_issue_in_issue_bits_src_2(5)
assume spec.vm(1)      == impl.auto_issue_in_issue_bits_vm(5)
assume spec.imm(1)     == impl.auto_issue_in_issue_bits_uop_ctrl_imm[4:0](5)
assume spec.Widden(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isWidden(5)
assume spec.Mask(1)    == impl.auto_issue_in_issue_bits_uop_vctrl_vm(5)
assume spec.vstart(1)  == impl.io_vstart(5)
assume spec.vxrm(1)    == impl.io_vcsr[2:1](5)
assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma(5)
assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta(5)

assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6(5)
assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1) < 3'b100

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h15

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h10))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h8))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set sew8_split_rule  "((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h10))"
set sew16_split_rule "((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h8))"
set sew32_split_rule "((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set sew64_split_rule "((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set vredsum_vs_active   "(spec.funct6(1) == 6'h00) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredand_vs_active   "(spec.funct6(1) == 6'h01) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredor_vs_active    "(spec.funct6(1) == 6'h02) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredxor_vs_active   "(spec.funct6(1) == 6'h03) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredminu_vs_active  "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredmin_vs_active   "(spec.funct6(1) == 6'h05) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredmaxu_vs_active  "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
set vredmax_vs_active   "(spec.funct6(1) == 6'h07) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0) && (spec.vstart(1) == 7'h0)"


# 2op 2 pipeline
lemma result_vredsum_vs    = ($vredsum_vs_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredand_vs    = ($vredand_vs_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredor_vs     = ($vredor_vs_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredxor_vs    = ($vredxor_vs_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredminu_vs   = ($vredminu_vs_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredmin_vs    = ($vredmin_vs_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredmaxu_vs   = ($vredmaxu_vs_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vredmax_vs    = ($vredmax_vs_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))

#lemma result_vredsum_vs_sew8     = ($vredsum_vs_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredand_vs_sew8     = ($vredand_vs_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredor_vs_sew8      = ($vredor_vs_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredxor_vs_sew8     = ($vredxor_vs_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredminu_vs_sew8    = ($vredminu_vs_active   && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmin_vs_sew8     = ($vredmin_vs_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmaxu_vs_sew8    = ($vredmaxu_vs_active   && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmax_vs_sew8     = ($vredmax_vs_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#
#lemma result_vredsum_vs_sew16    = ($vredsum_vs_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredand_vs_sew16    = ($vredand_vs_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredor_vs_sew16     = ($vredor_vs_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredxor_vs_sew16    = ($vredxor_vs_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredminu_vs_sew16   = ($vredminu_vs_active   && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmin_vs_sew16    = ($vredmin_vs_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmaxu_vs_sew16   = ($vredmaxu_vs_active   && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmax_vs_sew16    = ($vredmax_vs_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#
#lemma result_vredsum_vs_sew32    = ($vredsum_vs_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredand_vs_sew32    = ($vredand_vs_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredor_vs_sew32     = ($vredor_vs_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredxor_vs_sew32    = ($vredxor_vs_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredminu_vs_sew32   = ($vredminu_vs_active   && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmin_vs_sew32    = ($vredmin_vs_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmaxu_vs_sew32   = ($vredmaxu_vs_active   && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmax_vs_sew32    = ($vredmax_vs_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#
#lemma result_vredsum_vs_sew64    = ($vredsum_vs_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredand_vs_sew64    = ($vredand_vs_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredor_vs_sew64     = ($vredor_vs_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredxor_vs_sew64    = ($vredxor_vs_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredminu_vs_sew64   = ($vredminu_vs_active   && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmin_vs_sew64    = ($vredmin_vs_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmaxu_vs_sew64   = ($vredmaxu_vs_active   && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vredmax_vs_sew64    = ($vredmax_vs_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))

lemma valid  = impl.auto_writeback_out_valid(9) == 1'b1

}

proc case_split_vector {} {
    caseSplitStrategy basic -script "orch_configuration2_a"

    caseBegin sew8
    caseAssume (spec.vsew(1) == 3'b000)
    caseEnumerate vl_sew8  -expr spec.vl[3:0] -parent sew8 -type full

    caseBegin sew16
    caseAssume (spec.vsew(1) == 3'b001)
    caseEnumerate vl_sew16 -expr spec.vl[2:0] -parent sew16 -type full

    caseBegin sew32
    caseAssume (spec.vsew(1) == 3'b010)
    caseEnumerate vl_sew32 -expr spec.vl[1:0] -parent sew32 -type full

    caseBegin sew64
    caseAssume (spec.vsew(1) == 3'b011)
    caseEnumerate vl_sew64 -expr spec.vl[0]   -parent sew64 -type full

}



proc make {} {
    compile_spec
    compile_impl
    compose
}

proc run_main {} {
    set_user_assumes_lemmas_procedure "ual"
    set_hector_case_splitting_procedure "case_split_vector"
    solveNB p
    proofwait
    proofstatus
    listproof

}






