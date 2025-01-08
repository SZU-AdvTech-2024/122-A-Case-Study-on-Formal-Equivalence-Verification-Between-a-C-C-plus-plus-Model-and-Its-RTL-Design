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
              ../../../common/cmodel/hector_wrapper/v_floatpointFma2op.cpp \
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

#assume spec.vxrm(1) == 2'b10

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h12

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
#set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set round_to_zero     "spec.frm(1) == 3'b001"

set vfadd_vf_active         "(spec.funct6(1) == 6'h00) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfadd_vv_active         "(spec.funct6(1) == 6'h00) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsub_vf_active         "(spec.funct6(1) == 6'h02) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsub_vv_active         "(spec.funct6(1) == 6'h02) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmin_vf_active         "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmin_vv_active         "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmax_vf_active         "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmax_vv_active         "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsgnj_vf_active        "(spec.funct6(1) == 6'h08) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsgnj_vv_active        "(spec.funct6(1) == 6'h08) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsgnjn_vf_active       "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsgnjn_vv_active       "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsgnjx_vf_active       "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfsgnjx_vv_active       "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmv_s_f_active         "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
set vfmv_f_s_active         "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
set vfcvt_xu_f_v_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h00)"
set vfcvt_x_f_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h01)"
set vfcvt_f_xu_v_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h02)"
set vfcvt_f_x_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h03)"
set vfcvt_rtz_xu_f_v_active "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h06) && (spec.frm(1) == 3'b001)"
set vfcvt_rtz_x_f_v_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h07) && (spec.frm(1) == 3'b001)"
set vfrsqrt7_v_active       "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h04)"
set vfrec7_v_active         "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h05)"
set vfclass_v_active        "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h10)"
set vfmv_v_f_active         "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmerge_vfm_active      "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
set vfmul_vf_active         "(spec.funct6(1) == 6'h24) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmul_vv_active         "(spec.funct6(1) == 6'h24) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfrsub_vf_active        "(spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmadd_vf_active        "(spec.funct6(1) == 6'h28) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmadd_vv_active        "(spec.funct6(1) == 6'h28) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmadd_vf_active       "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmadd_vv_active       "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmsub_vf_active        "(spec.funct6(1) == 6'h2A) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmsub_vv_active        "(spec.funct6(1) == 6'h2A) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmsub_vf_active       "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmsub_vv_active       "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmacc_vf_active        "(spec.funct6(1) == 6'h2C) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmacc_vv_active        "(spec.funct6(1) == 6'h2C) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmacc_vf_active       "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmacc_vv_active       "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmsac_vf_active        "(spec.funct6(1) == 6'h2E) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfmsac_vv_active        "(spec.funct6(1) == 6'h2E) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmsac_vf_active       "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
set vfnmsac_vv_active       "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"


#-----------VFFMA  Module 2op 6 pipeline
lemma result_vfadd_vf    = ($vfadd_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfadd_vv    = ($vfadd_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfsub_vf    = ($vfsub_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfsub_vv    = ($vfsub_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfmul_vf    = ($vfmul_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfmul_vv    = ($vfmul_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))
lemma result_vfrsub_vf   = ($vfrsub_vf_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(17))

lemma fflags_vfadd_vf    = ($vfadd_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfadd_vv    = ($vfadd_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfsub_vf    = ($vfsub_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfsub_vv    = ($vfsub_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfmul_vf    = ($vfmul_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfmul_vv    = ($vfmul_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))
lemma fflags_vfrsub_vf   = ($vfrsub_vf_active   && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(17))

lemma valid_vfadd_vf    = ($vfadd_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfadd_vv    = ($vfadd_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfsub_vf    = ($vfsub_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfsub_vv    = ($vfsub_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfmul_vf    = ($vfmul_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfmul_vv    = ($vfmul_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)
lemma valid_vfrsub_vf   = ($vfrsub_vf_active   && ($common_split_rule)) -> (impl.auto_writeback_out_valid(17) == 1'b1)

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






