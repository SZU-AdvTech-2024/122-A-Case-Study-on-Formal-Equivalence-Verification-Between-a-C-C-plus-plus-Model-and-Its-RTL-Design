#========================================================
#    Setting  variable
#========================================================
set _hector_comp_use_new_flow true
set _hector_softfloat_version custom
#set _hector_multiple_solve_scripts true

set_host_file "../../scr/flow_com/host.qsub"

#========================================================
#    compile_spec
#========================================================

proc compile_spec {} {


        create_design -name spec -top hector_wrapper
        cppan \
              -I ../../../common/cmodel/acl2/include \
              ../../../common/cmodel/hector_wrapper/mul.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_MulExu -clock clock

        set_cutpoint bosc_MulExu.mul.io_in_bits_src_0
        set_cutpoint bosc_MulExu.mul.io_in_bits_src_1

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

for {set i 0} {$i <= 9} {incr i} {
   if {$i < 3} {
      assume impl.reset($i) == 1'b1
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
      assume spec.fuOpType(1)  == impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType($i)
   } elseif {$i < 5} {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
      assume spec.fuOpType(1)  == impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType($i)
   } elseif {$i < 7} {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b1
      assume spec.fuOpType(1)  == impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType($i)
   } else {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
      assume spec.fuOpType(1)  == impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType($i)
   }
}

assume spec.rs1(1)       == impl.auto_issue_in_issue_bits_src_0[63:0](5)
assume spec.rs2(1)       == impl.auto_issue_in_issue_bits_src_1[63:0](5)

assume -always impl.redirectIn_valid == 1'b0
assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuType == 5'b00100
assume spec.fuOpType(1)  == 7'b00

assume impl.mul.mulDataModule.io_result(9) == impl.mul.io_in_bits_src_0(5)[63:0] * impl.mul.io_in_bits_src_1(5)[63:0]

set_resource_limit 200000 
set_hector_multiple_solve_scripts true
set_hector_multiple_solve_scripts_list [list orch_multipliers]
set orch_solve_outputs_effort 2

#mul optypoe
#lemma result_mul       = (spec.fuOpType(1) == 7'h00) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_mul0       = spec.rd0(1) == impl.auto_writeback_out_bits_data[7:0](9)
lemma result_mul1       = spec.rd1(1) == impl.auto_writeback_out_bits_data[15:8](9)
lemma result_mul2       = spec.rd2(1) == impl.auto_writeback_out_bits_data[23:16](9)
lemma result_mul3       = spec.rd3(1) == impl.auto_writeback_out_bits_data[31:24](9)
lemma result_mul4       = spec.rd4(1) == impl.auto_writeback_out_bits_data[39:32](9)
lemma result_mul5       = spec.rd5(1) == impl.auto_writeback_out_bits_data[47:40](9)
lemma result_mul6       = spec.rd6(1) == impl.auto_writeback_out_bits_data[55:48](9)
lemma result_mul7       = spec.rd7(1) == impl.auto_writeback_out_bits_data[63:56](9)
#lemma result_mulh      = (spec.fuOpType(1) == 7'h01) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
#lemma result_mulhsu    = (spec.fuOpType(1) == 7'h02) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
#lemma result_mulhu     = (spec.fuOpType(1) == 7'h03) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))

lemma valid  = impl.auto_writeback_out_valid(9) == 1'b1

}

proc hdps_ual {} {

for {set i 0} {$i <= 9} {incr i} {
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

assume -always impl.redirectIn_valid == 1'b0
assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuType == 4'b0100

assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType == 7'h00

lemma check_mul = impl.mul.mulDataModule.io_result(9) == impl.auto_issue_in_issue_bits_src_0(5)[63:0] * impl.auto_issue_in_issue_bits_src_1(5)[63:0]

}

proc case_split_64 {} {
    caseSplitStrategy basic -script "orch_multipliers"

    caseBegin A_inf_NaN_64
    caseAssume (spec.rs1(1)[62:52] == 11'h7ff)

    caseBegin B_inf_NaN_64
    caseAssume (spec.rs2(1)[62:52] == 11'h7ff)

    caseBegin dnorm_dnorm_64
    caseAssume (spec.rs1(1)[62:52] == 11'h000)
    caseAssume (spec.rs2(1)[62:52] == 11'h000)

    caseBegin norm_norm_64
    caseAssume (spec.rs1(1)[62:52] != 11'h000)
    caseAssume (spec.rs1(1)[62:52] != 11'h7ff)
    caseAssume (spec.rs2(1)[62:52] != 11'h000)
    caseAssume (spec.rs2(1)[62:52] != 11'h7ff)

    caseBegin dnorm_norm_64
    caseAssume (spec.rs1(1)[62:52] == 11'h000)
    caseAssume (spec.rs2(1)[62:52] != 11'h000)
    caseAssume (spec.rs2(1)[62:52] != 11'h7ff)
    caseEnumerate dnn64_ier -expr spec.rs1[51:0] -parent dnorm_norm_64 -type leading1

    caseBegin norm_dnorm_64
    caseAssume (spec.rs1(1)[62:52] != 11'h000)
    caseAssume (spec.rs1(1)[62:52] != 11'h7ff)
    caseAssume (spec.rs2(1)[62:52] == 11'h000)
    caseEnumerate ndn64_and -expr spec.rs2[51:0] -parent norm_dnorm_64 -type leading1

}

proc make {} {
    compile_spec
    compile_impl
    compose
}

proc run_hdps {} {
     set_user_assumes_lemmas_procedure "hdps_ual"
     
     set_custom_solve_script "orch_multipliers"
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
    set_hector_case_splitting_procedure "case_split_64"
    solveNB p
    proofwait
    proofstatus
    listproof

}






