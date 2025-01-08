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

assume impl.mul.mulDataModule.io_result(9)[31:0] == impl.mul.io_in_bits_src_0(5)[31:0] * impl.mul.io_in_bits_src_1(5)[31:0]

set_resource_limit 200000 
set_hector_multiple_solve_scripts true
set_hector_multiple_solve_scripts_list [list orch_multipliers]
set orch_solve_outputs_effort 2

#mul32 optypoe
lemma result_mulw      = (spec.fuOpType(1) == 7'h04) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_mulw7     = (spec.fuOpType(1) == 7'h0C) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))

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

cutpoint mpier  = impl.mul.io_in_bits_src_0(5)
cutpoint mpcand = impl.mul.io_in_bits_src_1(5)

lemma check_mul = impl.mul.mulDataModule.io_result(9)[31:0] == (mpier[31:0] * mpcand[31:0]) + 32'b0

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
    solveNB p
    proofwait
    proofstatus
    listproof

}






