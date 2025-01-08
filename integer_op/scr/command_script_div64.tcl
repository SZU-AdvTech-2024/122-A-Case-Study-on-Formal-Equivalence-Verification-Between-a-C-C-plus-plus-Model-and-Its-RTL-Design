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
              ../../../common/cmodel/hector_wrapper/idiv64.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_DivExu -clock clock

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

for {set i 0} {$i <= 32} {incr i} {
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

assume spec.rs1(1)          == impl.auto_issue_in_issue_bits_src_0[63:0](5)
assume spec.rs2(1)          == impl.auto_issue_in_issue_bits_src_1[63:0](5)
assume spec.fuOpType[4:2](1)  == 3'b100

assume -always impl.redirectIn_valid == 1'b0
assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuType == 5'b00101

set start_phase 1
set end_phase   16
set always_active "1'b1"
for {set i $start_phase} {$i <= $end_phase} {set i [expr $i+1]} {
      set k [expr $i+1]
      set p [expr 9+$i*2]
      set q [expr 11+$i*2]
      lemma  midstate_iter_qPrevReg_${i}        = (!impl.divs_0.div.divDataModule.specialReg($p) && $always_active) -> (spec.qPrevReg${i}(1)      == impl.divs_0.div.divDataModule.qPrevReg($p))
      lemma  midstate_iter_quotIterReg_${i}     = (!impl.divs_0.div.divDataModule.specialReg($p) && $always_active) -> (spec.quotIterReg${i}(1)   == impl.divs_0.div.divDataModule.quotIterReg($p))
      lemma  midstate_iter_quotM1IterReg_${i}   = (!impl.divs_0.div.divDataModule.specialReg($p) && $always_active) -> (spec.quotM1IterReg${i}(1) == impl.divs_0.div.divDataModule.quotM1IterReg($p))
      lemma  midstate_iter_rSumReg_${i}         = (!impl.divs_0.div.divDataModule.specialReg($p) && $always_active) -> (spec.rSumReg${i}(1)       == impl.divs_0.div.divDataModule.rSumReg($p))
      lemma  midstate_iter_rCarryReg_${i}       = (!impl.divs_0.div.divDataModule.specialReg($p) && $always_active) -> (spec.rCarryReg${i}(1)     == impl.divs_0.div.divDataModule.rCarryReg($p))

      set final_active  "${always_active} && (impl.divs_0.div.divDataModule.iterNumReg($p) == 4'b0)"
      set always_active "${always_active} && (impl.divs_0.div.divDataModule.iterNumReg($p) != 4'b0)"
      lemma  midstate_post_quotIterReg_${k}     = (!impl.divs_0.div.divDataModule.specialReg($p) && $final_active)  -> (spec.quotOddIter${i}(1)   == impl.divs_0.div.divDataModule.quotIterReg($q))
      lemma  midstate_post_quotM1IterReg_${k}   = (!impl.divs_0.div.divDataModule.specialReg($p) && $final_active)  -> (spec.quotM1OddIter${i}(1) == impl.divs_0.div.divDataModule.quotM1IterReg($q))
      lemma  midstate_post_rSumReg_${k}         = (!impl.divs_0.div.divDataModule.specialReg($p) && $final_active)  -> (spec.rSumOddIter${i}(1)   == impl.divs_0.div.divDataModule.rSumReg($q))
      lemma  midstate_post_rCarryReg_${k}       = (!impl.divs_0.div.divDataModule.specialReg($p) && $final_active)  -> (spec.rCarryOddIter${i}(1) == impl.divs_0.div.divDataModule.rCarryReg($q))
}

set start_phase 1
set end_phase   21
set atleast_one "impl.auto_writeback_out_valid(5)"
for {set i $start_phase} {$i <= $end_phase} {set i [expr $i+1]} {
    set j [expr 5+$i*2]
    set k [expr $j-2]
    lemma  result_div_${i}  = (impl.auto_writeback_out_valid($k) && (spec.fuOpType(1) == 7'h10)) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0]($j))
    lemma  result_rem_${i}  = (impl.auto_writeback_out_valid($k) && (spec.fuOpType(1) == 7'h11)) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0]($j))
    lemma  result_divu_${i} = (impl.auto_writeback_out_valid($k) && (spec.fuOpType(1) == 7'h12)) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0]($j))
    lemma  result_remu_${i} = (impl.auto_writeback_out_valid($k) && (spec.fuOpType(1) == 7'h13)) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0]($j))
    set atleast_one "${atleast_one} || impl.auto_writeback_out_valid($k)"
}
lemma  result_valid = $atleast_one

}

proc make {} {
    compile_spec
    compile_impl
    compose
}

proc run_main {} {
    set_user_assumes_lemmas_procedure "ual"
    set_hector_lemma_partition  true
    set_hector_assume_guarantee true
    set_hector_ag_assume_all_prev true
    set_hector_ag_partition_list [list \
        [list *scv*         "-script orch_satonly"] \
        [list midstate*_1   "-script orch_satonly"] \
        [list midstate*_2   "-script orch_configuration2_a"] \
        [list midstate*_3   "-script orch_configuration2_a"] \
        [list midstate*_4   "-script orch_configuration2_a"] \
        [list midstate*_5   "-script orch_configuration2_a"] \
        [list midstate*_6   "-script orch_configuration2_a"] \
        [list midstate*_7   "-script orch_configuration2_a"] \
        [list midstate*_8   "-script orch_configuration2_a"] \
        [list midstate*_9   "-script orch_configuration2_a"] \
        [list midstate*_10  "-script orch_configuration2_a"] \
        [list midstate*_11  "-script orch_configuration2_a"] \
        [list midstate*_12  "-script orch_configuration2_a"] \
        [list midstate*_13  "-script orch_configuration2_a"] \
        [list midstate*_14  "-script orch_configuration2_a"] \
        [list midstate*_15  "-script orch_configuration2_a"] \
        [list midstate*_16  "-script orch_configuration2_a"] \
        [list midstate*_17  "-script orch_configuration2_a"] \
        [list result*       "-script orch_configuration2_a"]]
    solveNB p
    proofwait
    proofstatus
    listproof

}






