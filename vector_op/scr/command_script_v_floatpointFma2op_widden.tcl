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
              -I ../../../common/cmodel/acl2/include \
              -I ../../../common/cmodel/riscv-isa-sim/include \
              -I ../../../common/cmodel/softfloat-3e/build/HECTOR \
              -I ../../../common/cmodel/softfloat-3e/build/Linux-x86_64-GCC \
              -I ../../../common/cmodel/softfloat-3e/source/RISCV \
              -I ../../../common/cmodel/softfloat-3e/source/include \
              ../../../common/cmodel/hector_wrapper/v_floatpointFma2op_widden.cpp \
              $SF_OPTIONS
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VFpExu -clock clock

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

for {set i 0} {$i <= 22} {incr i} {
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
assume spec.Narrow(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isNarrow(5)
assume spec.Widden(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isWidden(5)
assume spec.MaskOp(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_maskOp(5)
assume spec.Mask(1)    == impl.auto_issue_in_issue_bits_uop_vctrl_vm(5)
assume spec.vstart(1)  == impl.io_vstart(5)
assume spec.vxrm(1)    == impl.io_vcsr[2:1](5)
assume spec.frm(1)     == impl.io_frm(5)
assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma(5)
assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta(5)

assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6(5)
assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1) < 3'b100
assume spec.frm(1)  < 3'b100

assume spec.eewType_1(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_1(5)
assume spec.eewType_2(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_2(5)

#assume spec.vxrm(1) == 2'b10

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h12

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
#Widden
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h2))"

#Narrow_to_1
set common_split_rule_narrow "1'b0"
set common_split_rule_narrow "$common_split_rule_narrow || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set common_split_rule_narrow "$common_split_rule_narrow || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set round_to_zero     "spec.frm(1) == 3'b001"

set vfwadd_vf_active         "(spec.funct6(1) == 6'h30) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h3)"
set vfwadd_vv_active         "(spec.funct6(1) == 6'h30) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h3)"
set vfwadd_wf_active         "(spec.funct6(1) == 6'h34) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h4)"
set vfwadd_wv_active         "(spec.funct6(1) == 6'h34) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h4)"
set vfwsub_vf_active         "(spec.funct6(1) == 6'h32) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h3)"
set vfwsub_vv_active         "(spec.funct6(1) == 6'h32) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h3)"
set vfwsub_wf_active         "(spec.funct6(1) == 6'h36) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h4)"
set vfwsub_wv_active         "(spec.funct6(1) == 6'h36) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_1(1) == 3'h4)"
set vfwmul_vf_active         "(spec.funct6(1) == 6'h38) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.eewType_1(1) == 3'h3)"
set vfwmul_vv_active         "(spec.funct6(1) == 6'h38) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.eewType_1(1) == 3'h3)"

set vmfeq_vf_active          "(spec.funct6(1) == 6'h18) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfeq_vv_active          "(spec.funct6(1) == 6'h18) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfge_vf_active          "(spec.funct6(1) == 6'h1F) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfgt_vf_active          "(spec.funct6(1) == 6'h1D) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfle_vf_active          "(spec.funct6(1) == 6'h19) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfle_vv_active          "(spec.funct6(1) == 6'h19) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmflt_vf_active          "(spec.funct6(1) == 6'h1B) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmflt_vv_active          "(spec.funct6(1) == 6'h1B) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfne_vf_active          "(spec.funct6(1) == 6'h1C) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"
set vmfne_vv_active          "(spec.funct6(1) == 6'h1C) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1)"

#Widden
lemma result_vfwadd_vf    = ($vfwadd_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwadd_vv    = ($vfwadd_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwadd_wf    = ($vfwadd_wf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwadd_wv    = ($vfwadd_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwsub_vf    = ($vfwsub_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwsub_vv    = ($vfwsub_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwsub_wf    = ($vfwsub_wf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfwsub_wv    = ($vfwsub_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
#lemma result_vfwmul_vf    = ($vfwmul_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
#lemma result_vfwmul_vv    = ($vfwmul_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))

lemma fflags_vfwadd_vf    = ($vfwadd_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwadd_vv    = ($vfwadd_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwadd_wf    = ($vfwadd_wf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwadd_wv    = ($vfwadd_wv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwsub_vf    = ($vfwsub_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwsub_vv    = ($vfwsub_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwsub_wf    = ($vfwsub_wf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfwsub_wv    = ($vfwsub_wv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
#lemma fflags_vfwmul_vf    = ($vfwmul_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
#lemma fflags_vfwmul_vv    = ($vfwmul_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))

lemma valid_vfwadd_vf     = ($vfwadd_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwadd_vv     = ($vfwadd_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwadd_wf     = ($vfwadd_wf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwadd_wv     = ($vfwadd_wv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwsub_vf     = ($vfwsub_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwsub_vv     = ($vfwsub_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwsub_wf     = ($vfwsub_wf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfwsub_wv     = ($vfwsub_wv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
#lemma valid_vfwmul_vf     = ($vfwmul_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
#lemma valid_vfwmul_vv     = ($vfwmul_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)

#Narrow
#lemma result_vmfeq_vf    = ($vmfeq_vf_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfeq_vv    = ($vmfeq_vv_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfge_vf    = ($vmfge_vf_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfgt_vf    = ($vmfgt_vf_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfle_vf    = ($vmfle_vf_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfle_vv    = ($vmfle_vv_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmflt_vf    = ($vmflt_vf_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmflt_vv    = ($vmflt_vv_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfne_vf    = ($vmfne_vf_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vmfne_vv    = ($vmfne_vv_active    && ($common_split_rule_narrow)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#
#lemma fflags_vmfeq_vf    = ($vmfeq_vf_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vmfeq_vv    = ($vmfeq_vv_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmfge_vf    = ($vmfge_vf_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmfgt_vf    = ($vmfgt_vf_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmfle_vf    = ($vmfle_vf_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmfle_vv    = ($vmfle_vv_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmflt_vf    = ($vmflt_vf_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmflt_vv    = ($vmflt_vv_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmfne_vf    = ($vmfne_vf_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fllags_vmfne_vv    = ($vmfne_vv_active    && ($common_split_rule_narrow)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#
#lemma valid_vmfeq_vf     = ($vmfeq_vf_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfeq_vv     = ($vmfeq_vv_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfge_vf     = ($vmfge_vf_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfgt_vf     = ($vmfgt_vf_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfle_vf     = ($vmfle_vf_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfle_vv     = ($vmfle_vv_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmflt_vf     = ($vmflt_vf_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmflt_vv     = ($vmflt_vv_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfne_vf     = ($vmfne_vf_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vmfne_vv     = ($vmfne_vv_active    && ($common_split_rule_narrow)) -> (impl.auto_writeback_out_valid(15) == 1'b1)

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






