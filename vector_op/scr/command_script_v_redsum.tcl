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

        set_cutpoint bosc_VMacExu.vred.in0_sew8
        set_cutpoint bosc_VMacExu.vred.vs1_zero
        set_cutpoint bosc_VMacExu.vred.reg_vs2m_bits_widen

        vcs -sverilog {+define+SYNTHESIS -f ../../../common/filelist/bosc_XSTop.f}

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

#HDPS lemma use
assume impl.vred._csa_4to2_sew8_4_io_out(7)[7:0] ==  impl.vred.vs1_zero(7)[7:0] \
                                                    +impl.vred.in0_sew8(7)[7:0]     \
                                                    +impl.vred.in0_sew8(7)[15:8]    \
                                                    +impl.vred.in0_sew8(7)[23:16]   \
                                                    +impl.vred.in0_sew8(7)[31:24]   \
                                                    +impl.vred.in0_sew8(7)[39:32]   \
                                                    +impl.vred.in0_sew8(7)[47:40]   \
                                                    +impl.vred.in0_sew8(7)[55:48]   \
                                                    +impl.vred.in0_sew8(7)[63:56]   \
                                                    +impl.vred.in0_sew8(7)[71:64]   \
                                                    +impl.vred.in0_sew8(7)[79:72]   \
                                                    +impl.vred.in0_sew8(7)[87:80]   \
                                                    +impl.vred.in0_sew8(7)[95:88]   \
                                                    +impl.vred.in0_sew8(7)[103:96]  \
                                                    +impl.vred.in0_sew8(7)[111:104] \
                                                    +impl.vred.in0_sew8(7)[119:112] \
                                                    +impl.vred.in0_sew8(7)[127:120]

assume impl.vred._csa_4to2_sew16_4_io_out(7)[15:0] ==  impl.vred.vs1_zero(7)[15:0]     \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[15:0]   \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[31:16]  \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[47:32]  \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[63:48]  \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[79:64]  \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[95:80]  \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[111:96] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[127:112] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[143:128] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[159:144] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[175:160] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[191:176] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[207:192] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[223:208] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[239:224] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[255:240]

assume impl.vred._csa_4to2_sew32_io_out(7)[31:0] ==  impl.vred.vs1_zero(7)[31:0]     \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[31:0]  \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[63:32]  \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[95:64]  \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[127:96] \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[159:128] \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[191:160] \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[223:192] \
                                                    +impl.vred.reg_vs2m_bits_widen(7)[255:224]

assume impl.vred._csa_3to2_sew64_1_io_out(7)[63:0] ==  impl.vred.vs1_zero(7)[63:0]     \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[63:0]  \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[127:64] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[191:128] \
                                                      +impl.vred.reg_vs2m_bits_widen(7)[255:192]

#set_resource_limit 200000 
#set_hector_multiple_solve_scripts true
#set_hector_multiple_solve_scripts_list [list orch_configuration2_a]
#set orch_solve_outputs_effort 2

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

# 2op 2 pipeline
lemma result_vredsum_vs    = ($vredsum_vs_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))

lemma valid  = impl.auto_writeback_out_valid(9) == 1'b1

}

proc hdps_ual {} {

cutpoint vs2sewis8op = impl.vred.in0_sew8(7)
cutpoint vs2sewbe8op = impl.vred.reg_vs2m_bits_widen(7)
cutpoint vs1op       = impl.vred.vs1_zero(7)

lemma check_redsumsew8 = impl.vred._csa_4to2_sew8_4_io_out(7)[7:0] ==  vs1op[7:0]     \
                                                                      +vs2sewis8op[7:0]     \
                                                                      +vs2sewis8op[15:8]    \
                                                                      +vs2sewis8op[23:16]   \
                                                                      +vs2sewis8op[31:24]   \
                                                                      +vs2sewis8op[39:32]   \
                                                                      +vs2sewis8op[47:40]   \
                                                                      +vs2sewis8op[55:48]   \
                                                                      +vs2sewis8op[63:56]   \
                                                                      +vs2sewis8op[71:64]   \
                                                                      +vs2sewis8op[79:72]   \
                                                                      +vs2sewis8op[87:80]   \
                                                                      +vs2sewis8op[95:88]   \
                                                                      +vs2sewis8op[103:96]  \
                                                                      +vs2sewis8op[111:104] \
                                                                      +vs2sewis8op[119:112] \
                                                                      +vs2sewis8op[127:120]

lemma check_redsumsew16 = impl.vred._csa_4to2_sew16_4_io_out(7)[15:0] ==  vs1op[15:0]     \
                                                                         +vs2sewbe8op[15:0]   \
                                                                         +vs2sewbe8op[31:16]  \
                                                                         +vs2sewbe8op[47:32]  \
                                                                         +vs2sewbe8op[63:48]  \
                                                                         +vs2sewbe8op[79:64]  \
                                                                         +vs2sewbe8op[95:80]  \
                                                                         +vs2sewbe8op[111:96] \
                                                                         +vs2sewbe8op[127:112] \
                                                                         +vs2sewbe8op[143:128] \
                                                                         +vs2sewbe8op[159:144] \
                                                                         +vs2sewbe8op[175:160] \
                                                                         +vs2sewbe8op[191:176] \
                                                                         +vs2sewbe8op[207:192] \
                                                                         +vs2sewbe8op[223:208] \
                                                                         +vs2sewbe8op[239:224] \
                                                                         +vs2sewbe8op[255:240]

lemma check_redsumsew32 = impl.vred._csa_4to2_sew32_io_out(7)[31:0] ==  vs1op[31:0]     \
                                                                       +vs2sewbe8op[31:0]  \
                                                                       +vs2sewbe8op[63:32]  \
                                                                       +vs2sewbe8op[95:64]  \
                                                                       +vs2sewbe8op[127:96] \
                                                                       +vs2sewbe8op[159:128] \
                                                                       +vs2sewbe8op[191:160] \
                                                                       +vs2sewbe8op[223:192] \
                                                                       +vs2sewbe8op[255:224]

lemma check_redsumsew64 = impl.vred._csa_3to2_sew64_1_io_out(7)[63:0] ==  vs1op[63:0]     \
                                                                         +vs2sewbe8op[63:0]  \
                                                                         +vs2sewbe8op[127:64] \
                                                                         +vs2sewbe8op[191:128] \
                                                                         +vs2sewbe8op[255:192]

}

proc case_split_vector {} {
    caseSplitStrategy basic

    caseBegin sew8
    caseAssume (spec.vsew(1) == 3'b000)
    caseEnumerate vl_sew8  -expr spec.vl[3:0] -parent sew8  -type full

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

proc run_hdps {} {
     set_user_assumes_lemmas_procedure "hdps_ual"
     
     set_custom_solve_script "orch_custom_bit_operations"
     set_hector_rew_use_dps_engine true
     set_hector_rew_dps_solve_script __hector_orch_custom_dps2
     set_sat_time_limit 200
     set_hector_rew_dps_resource_limit 1200

    run_all_hdps_options -encoding [list auto] hdps
    proofwait
    proofstatus
    listproof
}

proc run_main {} {
    set_user_assumes_lemmas_procedure "ual"
    set_hector_case_splitting_procedure "case_split_vector"
    solveNB p
    proofwait
    proofstatus
    listproof

}






