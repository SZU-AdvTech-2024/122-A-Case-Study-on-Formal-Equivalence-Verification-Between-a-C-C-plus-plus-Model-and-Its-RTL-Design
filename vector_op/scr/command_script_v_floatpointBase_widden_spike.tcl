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
    source ../../../common/cmodel/riscv-isa-sim-master/softfloat/cmodelist.tcl

    create_design -name spec -top hector_wrapper
    cppan \
          -I ../../../common/cmodel/riscv-isa-sim-master/build \
          -I ../../../common/cmodel/riscv-isa-sim-master/build2/HECTOR \
          -I ../../../common/cmodel/riscv-isa-sim-master/softfloat \
          -I ../../../common/cmodel/acl2/include \
          -I ../../../common/cmodel/riscv-isa-sim-master/softfloat/specialize.h \
          ../../../common/cmodel/hector_wrapper/v_floatpointBase_widden_spike.cpp \
          $SF_OPTIONS

    compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VFpExu -clock clock
        #create_design -name impl -top VFpExu -clock clock

        #vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f
        vcs -sverilog  {+define+SYSTHESIS ../../../common/filelist/XSTop.sv}
        #vcs -sverilog /nfs/home/guanmingxing/work-nh3/share_for_formal/SimTop.sv

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

assume spec.eewType_1(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_1(5)
assume spec.eewType_2(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_2(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1) < 3'b100

#assume spec.vxrm(1) == 2'b10

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h12

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h2))"
#set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

#Rounding Mode
set round_to_zero     "spec.frm(1) == 3'b001"
set common_round      "spec.frm(1) <= 3'b100"

#Widden
set vfwcvt_f_f_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h0C) && (spec.eewType_1(1) == 3'h3)"
set vfwcvt_xu_f_v_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h08) && (spec.eewType_1(1) == 3'h3)"
set vfwcvt_x_f_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h09) && (spec.eewType_1(1) == 3'h3)"
set vfwcvt_f_xu_v_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h0A) && (spec.eewType_1(1) == 3'h3)"
set vfwcvt_f_x_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h0B) && (spec.eewType_1(1) == 3'h3)"
set vfwcvt_rtz_xu_f_v_active "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h0E) && (spec.eewType_1(1) == 3'h3)"
set vfwcvt_rtz_x_f_v_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h0F) && (spec.eewType_1(1) == 3'h3)"
set vfwcvtbf16_f_f_v_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h0D) && (spec.eewType_1(1) == 3'h3)"

#Narrow
set vfncvt_f_f_w_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h14) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_xu_f_w_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h10) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_x_f_w_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h11) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_f_xu_w_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h12) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_f_x_w_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h13) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_rtz_xu_f_w_active "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h16) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_rtz_x_f_w_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h17) && (spec.eewType_2(1) == 3'h3)"
set vfncvt_rod_f_f_w_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h15) && (spec.eewType_2(1) == 3'h3)"
set vfncvtbf16_f_f_v_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h1D) && (spec.eewType_1(1) == 3'h3)"


#set vfmin_vf_active         "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfmin_vv_active         "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfmax_vf_active         "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfmax_vv_active         "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfsgnj_vf_active        "(spec.funct6(1) == 6'h08) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfsgnj_vv_active        "(spec.funct6(1) == 6'h08) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfsgnjn_vf_active       "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfsgnjn_vv_active       "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfsgnjx_vf_active       "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfsgnjx_vv_active       "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0)"
#set vfmv_s_f_active         "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vfmv_f_s_active         "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vfcvt_xu_f_v_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h00)"
#set vfcvt_x_f_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h01)"
#set vfcvt_f_xu_v_active     "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h02)"
#set vfcvt_f_x_v_active      "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h03)"
#set vfcvt_rtz_xu_f_v_active "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h06) && (spec.frm(1) == 3'b001)"
#set vfcvt_rtz_x_f_v_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h07) && (spec.frm(1) == 3'b001)"
#set vfrsqrt7_v_active       "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h04)"
#set vfrec7_v_active         "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h05)"
#set vfclass_v_active        "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.imm(1) == 5'h10)"
#set vfmv_v_f_active         "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
#set vfmerge_vfm_active      "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.Mask(1) == 1'b1)"

#-----------VFMisc Module 5 pipeline
#lemma result_vfmin_vf    = ($vfmin_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfmin_vv    = ($vfmin_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfmax_vf    = ($vfmax_vf_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfmax_vv    = ($vfmax_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfsgnj_vf   = ($vfsgnj_vf_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfsgnj_vv   = ($vfsgnj_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfsgnjn_vf  = ($vfsgnjn_vf_active  && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfsgnjn_vv  = ($vfsgnjn_vv_active  && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfsgnjx_vf  = ($vfsgnjx_vf_active  && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfsgnjx_vv  = ($vfsgnjx_vv_active  && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfclass_v   = ($vfclass_v_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfmv_v_f    = ($vfmv_v_f_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfmerge_vfm = ($vfmerge_vfm_active && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#
#lemma fflags_vfmin_vf    = ($vfmin_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfmin_vv    = ($vfmin_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfmax_vf    = ($vfmax_vf_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfmax_vv    = ($vfmax_vv_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfsgnj_vf   = ($vfsgnj_vf_active   && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfsgnj_vv   = ($vfsgnj_vv_active   && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfsgnjn_vf  = ($vfsgnjn_vf_active  && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfsgnjn_vv  = ($vfsgnjn_vv_active  && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfsgnjx_vf  = ($vfsgnjx_vf_active  && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfsgnjx_vv  = ($vfsgnjx_vv_active  && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfclass_v   = ($vfclass_v_active   && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfmv_v_f    = ($vfmv_v_f_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfmerge_vfm = ($vfmerge_vfm_active && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#
#lemma valid_vfmin_vf    = ($vfmin_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfmin_vv    = ($vfmin_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfmax_vf    = ($vfmax_vf_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfmax_vv    = ($vfmax_vv_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfsgnj_vf   = ($vfsgnj_vf_active   && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfsgnj_vv   = ($vfsgnj_vv_active   && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfsgnjn_vf  = ($vfsgnjn_vf_active  && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfsgnjn_vv  = ($vfsgnjn_vv_active  && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfsgnjx_vf  = ($vfsgnjx_vf_active  && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfsgnjx_vv  = ($vfsgnjx_vv_active  && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfclass_v   = ($vfclass_v_active   && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfmv_v_f    = ($vfmv_v_f_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfmerge_vfm = ($vfmerge_vfm_active && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)

#-----------VFCVT Module 5 pipeline
#lemma result_vfcvt_xu_f_v      = ($vfcvt_xu_f_v_active      && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfcvt_x_f_v       = ($vfcvt_x_f_v_active       && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfcvt_f_xu_v      = ($vfcvt_f_xu_v_active      && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfcvt_f_x_v       = ($vfcvt_f_x_v_active       && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfcvt_rtz_xu_f_v  = ($vfcvt_rtz_xu_f_v_active  && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfcvt_rtz_x_f_v   = ($vfcvt_rtz_x_f_v_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#
#lemma fflags_vfcvt_xu_f_v      = ($vfcvt_xu_f_v_active      && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfcvt_x_f_v       = ($vfcvt_x_f_v_active       && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfcvt_f_xu_v      = ($vfcvt_f_xu_v_active      && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfcvt_f_x_v       = ($vfcvt_f_x_v_active       && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfcvt_rtz_xu_f_v  = ($vfcvt_rtz_xu_f_v_active  && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfcvt_rtz_x_f_v   = ($vfcvt_rtz_x_f_v_active   && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#
#
#lemma valid_vfcvt_xu_f_v      = ($vfcvt_xu_f_v_active      && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfcvt_x_f_v       = ($vfcvt_x_f_v_active       && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfcvt_f_xu_v      = ($vfcvt_f_xu_v_active      && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfcvt_f_x_v       = ($vfcvt_f_x_v_active       && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfcvt_rtz_xu_f_v  = ($vfcvt_rtz_xu_f_v_active  && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfcvt_rtz_x_f_v   = ($vfcvt_rtz_x_f_v_active   && ($common_split_rule)) -> (impl.auto_writeback_out_valid(15) == 1'b1)

#lemma result_vfwcvt_f_f_v       = ($vfwcvt_f_f_v_active       && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfwcvt_xu_f_v      = ($vfwcvt_xu_f_v_active      && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfwcvt_x_f_v       = ($vfwcvt_x_f_v_active       && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfwcvt_f_xu_v      = ($vfwcvt_f_xu_v_active      && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfwcvt_f_x_v       = ($vfwcvt_f_x_v_active       && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfwcvt_rtz_xu_f_v  = ($vfwcvt_rtz_xu_f_v_active  && ($common_split_rule)  && ($round_to_zero)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfwcvt_rtz_x_f_v   = ($vfwcvt_rtz_x_f_v_active   && ($common_split_rule)  && ($round_to_zero)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
lemma result_vfwcvtbf16_f_f_v   = ($vfwcvtbf16_f_f_v_active   && ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#
#lemma fflags_vfwcvt_f_f_v       = ($vfwcvt_f_f_v_active       && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfwcvt_xu_f_v      = ($vfwcvt_xu_f_v_active      && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfwcvt_x_f_v       = ($vfwcvt_x_f_v_active       && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfwcvt_f_xu_v      = ($vfwcvt_f_xu_v_active      && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfwcvt_f_x_v       = ($vfwcvt_f_x_v_active       && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfwcvt_rtz_xu_f_v  = ($vfwcvt_rtz_xu_f_v_active  && ($common_split_rule)  && ($round_to_zero)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfwcvt_rtz_x_f_v   = ($vfwcvt_rtz_x_f_v_active   && ($common_split_rule)  && ($round_to_zero)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
lemma fflags_vfwcvtbf16_f_f_v   = ($vfwcvtbf16_f_f_v_active   && ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))

#lemma valid_vfwcvt_f_f_v       = ($vfwcvt_f_f_v_active       && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfwcvt_xu_f_v      = ($vfwcvt_xu_f_v_active      && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfwcvt_x_f_v       = ($vfwcvt_x_f_v_active       && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfwcvt_f_xu_v      = ($vfwcvt_f_xu_v_active      && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfwcvt_f_x_v       = ($vfwcvt_f_x_v_active       && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfwcvt_rtz_xu_f_v  = ($vfwcvt_rtz_xu_f_v_active  && ($common_split_rule)  && ($round_to_zero)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfwcvt_rtz_x_f_v   = ($vfwcvt_rtz_x_f_v_active   && ($common_split_rule)  && ($round_to_zero)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
lemma valid_vfwcvtbf16_f_f_v   = ($vfwcvtbf16_f_f_v_active   && ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#
#lemma result_vfncvt_f_f_w       = ($vfncvt_f_f_w_active       && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_xu_f_w      = ($vfncvt_xu_f_w_active      && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_x_f_w       = ($vfncvt_x_f_w_active       && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_f_xu_w      = ($vfncvt_f_xu_w_active      && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_f_x_w       = ($vfncvt_f_x_w_active       && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_rtz_xu_f_w  = ($vfncvt_rtz_xu_f_w_active  && ($common_split_rule)  && ($round_to_zero)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_rtz_x_f_w   = ($vfncvt_rtz_x_f_w_active   && ($common_split_rule)  && ($round_to_zero)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#lemma result_vfncvt_rod_f_f_w   = ($vfncvt_rod_f_f_w_active   && ($common_split_rule)  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
lemma result_vfncvtbf16_f_f_v   = ($vfncvtbf16_f_f_v_active   && ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))  && ($common_round)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(15))
#
#lemma fflags_vfncvt_f_f_w       = ($vfncvt_f_f_w_active       && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_xu_f_w      = ($vfncvt_xu_f_w_active      && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_x_f_w       = ($vfncvt_x_f_w_active       && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_f_xu_w      = ($vfncvt_f_xu_w_active      && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_f_x_w       = ($vfncvt_f_x_w_active       && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_rtz_xu_f_w  = ($vfncvt_rtz_xu_f_w_active  && ($common_split_rule)  && ($round_to_zero)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_rtz_x_f_w   = ($vfncvt_rtz_x_f_w_active   && ($common_split_rule)  && ($round_to_zero)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#lemma fflags_vfncvt_rod_f_f_w   = ($vfncvt_rod_f_f_w_active   && ($common_split_rule)  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
lemma fflags_vfncvtbf16_f_f_v   = ($vfncvtbf16_f_f_v_active   && ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))  && ($common_round)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(15))
#
#lemma valid_vfncvt_f_f_w       = ($vfncvt_f_f_w_active       && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_xu_f_w      = ($vfncvt_xu_f_w_active      && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_x_f_w       = ($vfncvt_x_f_w_active       && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_f_xu_w      = ($vfncvt_f_xu_w_active      && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_f_x_w       = ($vfncvt_f_x_w_active       && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_rtz_xu_f_w  = ($vfncvt_rtz_xu_f_w_active  && ($common_split_rule)  && ($round_to_zero)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_rtz_x_f_w   = ($vfncvt_rtz_x_f_w_active   && ($common_split_rule)  && ($round_to_zero)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
#lemma valid_vfncvt_rod_f_f_w   = ($vfncvt_rod_f_f_w_active   && ($common_split_rule)  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)
lemma valid_vfncvtbf16_f_f_v   = ($vfncvtbf16_f_f_v_active   && ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))  && ($common_round)) -> (impl.auto_writeback_out_valid(15) == 1'b1)

#-----------VFRec Module 4 pipeline
#lemma result_vfrsqrt7_v  = ($vfrsqrt7_v_active  && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(13))
#lemma result_vfrec7_v    = ($vfrec7_v_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(13))
#
#lemma fflags_vfrsqrt7_v  = ($vfrsqrt7_v_active  && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(13))
#lemma fflags_vfrec7_v    = ($vfrec7_v_active    && ($common_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(13))
#
#lemma valid_vfrsqrt7_v  = ($vfrsqrt7_v_active  && ($common_split_rule)) -> (impl.auto_writeback_out_valid(13) == 1'b1)
#lemma valid_vfrec7_v    = ($vfrec7_v_active    && ($common_split_rule)) -> (impl.auto_writeback_out_valid(13) == 1'b1)

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






