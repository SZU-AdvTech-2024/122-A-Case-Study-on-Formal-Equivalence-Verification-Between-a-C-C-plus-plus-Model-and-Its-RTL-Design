
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
    source ../../../common/cmodel/softfloat-3e/softfloat-3e.riscv.tcl
 
    create_design -name spec -top hector_wrapper
    cppan \
          -DSOFTFLOAT_FAST_INT64 \
          -I ../../../common/cmodel/softfloat-3e/build/HECTOR \
          -I ../../../common/cmodel/softfloat-3e/build/Linux-x86_64-GCC \
          -I ../../../common/cmodel/softfloat-3e/source/RISCV \
          -I ../../../common/cmodel/softfloat-3e/source/include \
          -I ../../../common/cmodel/acl2/include \
          ../../../common/cmodel/hector_wrapper/fmac.cpp \
          $SF_OPTIONS

    compile_design spec
}

#========================================================
#    compile_impl
#========================================================
proc compile_impl {} {
    create_design -name impl -top bosc_FmacExu -clock clock

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

    assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuType == 5'b01001
    assume -always impl.redirectIn_valid == 1'b0
    assume -always impl.auto_issue_in_issue_bits_uop_ctrl_rfWen == 1'b0
    assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fpWen == 1'b1

    assume -always impl.auto_issue_in_issue_bits_uop_pdest == 8'b0
    assume -always impl.auto_issue_in_issue_bits_uop_robIdx_flag == 1'b0
    assume -always impl.auto_issue_in_issue_bits_uop_robIdx_value == 8'b0
    
    assume spec.rs1(1)     == impl.auto_issue_in_issue_bits_src_0(5)
    assume spec.rs2(1)     == impl.auto_issue_in_issue_bits_src_1(5)
    assume spec.rm(1)      == impl.auto_issue_in_issue_bits_uop_ctrl_fpu_rm(5)
    assume spec.csr_frm(1) == impl.csr_frm(5)

    assume spec.isAddSub(1) == impl.auto_issue_in_issue_bits_uop_ctrl_fpu_isAddSub(5)
    assume spec.typeTagIn(1) == impl.auto_issue_in_issue_bits_uop_ctrl_fpu_typeTagIn(5)
    assume spec.fmaCmd(1) == impl.auto_issue_in_issue_bits_uop_ctrl_fpu_fmaCmd(5)
    assume spec.ren3(1) == impl.auto_issue_in_issue_bits_uop_ctrl_fpu_ren3(5)

#    lemma valid = impl.auto_writeback_out_valid(9) == 1'b1
#    lemma check = spec.rd(1) == impl.auto_writeback_out_bits_data(9)

#    lemma inexact =   spec.exceptions[0](1) == impl.auto_writeback_out_bits_fflags[0](9)
#    lemma underflow = spec.exceptions[1](1) == impl.auto_writeback_out_bits_fflags[1](9)
#    lemma overflow =  spec.exceptions[2](1) == impl.auto_writeback_out_bits_fflags[2](9)
#    lemma infinity =  spec.exceptions[3](1) == impl.auto_writeback_out_bits_fflags[3](9)
#    lemma invalid =   spec.exceptions[4](1) == impl.auto_writeback_out_bits_fflags[4](9)

    set fadd_s_active "(spec.isAddSub(1) == 1'b1) && (spec.typeTagIn(1) == 1'b0) && (spec.fmaCmd(1) == 2'b00) && (spec.ren3(1) == 1'b0)"
    set fadd_d_active "(spec.isAddSub(1) == 1'b1) && (spec.typeTagIn(1) == 1'b1) && (spec.fmaCmd(1) == 2'b00) && (spec.ren3(1) == 1'b0)"
    set fsub_s_active "(spec.isAddSub(1) == 1'b1) && (spec.typeTagIn(1) == 1'b0) && (spec.fmaCmd(1) == 2'b01) && (spec.ren3(1) == 1'b0)"
    set fsub_d_active "(spec.isAddSub(1) == 1'b1) && (spec.typeTagIn(1) == 1'b1) && (spec.fmaCmd(1) == 2'b01) && (spec.ren3(1) == 1'b0)"

    set rm_enumerate_rule "(spec.rm(1) <= 4) || ((spec.rm(1) == 7) && (spec.csr_frm(1) <= 4))"

    set result "(impl.auto_writeback_out_valid(9) == 1'b1) && (spec.rd(1) == impl.auto_writeback_out_bits_data(9)) && (spec.exceptions(1) == impl.auto_writeback_out_bits_fflags(9))"

    lemma result_fadd_s = (($fadd_s_active) && ($rm_enumerate_rule)) -> ($result)
    lemma result_fadd_d = (($fadd_d_active) && ($rm_enumerate_rule)) -> ($result)
    lemma result_fsub_s = (($fsub_s_active) && ($rm_enumerate_rule)) -> ($result)
    lemma result_fsub_d = (($fsub_d_active) && ($rm_enumerate_rule)) -> ($result)

}


proc make {} {
    compile_spec
    compile_impl
    compose
}

proc run_main {} {
    set_user_assumes_lemmas_procedure "ual"
    solveNB p
    proofwait
    proofstatus
    listproof

}

