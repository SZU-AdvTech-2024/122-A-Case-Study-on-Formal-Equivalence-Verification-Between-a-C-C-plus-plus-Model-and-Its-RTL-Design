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
              -I ../../../common/cmodel/riscv-isa-sim/include \
              ../../../common/cmodel/hector_wrapper/v_fixedpoint_narrow.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VAluExu -clock clock

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
assume spec.Narrow(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isNarrow(5)
assume spec.Widden(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isWidden(5)
assume spec.Mask(1)    == impl.auto_issue_in_issue_bits_uop_vctrl_vm(5)
assume spec.MaskOp(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_maskOp(5)
assume spec.vstart(1)  == impl.io_vstart(5)
assume spec.vxrm(1)    == impl.io_vcsr[2:1](5)
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

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h11

assume -always impl.redirectIn_valid == 1'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h8))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h2))"
#set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set vnclipu_wi_active    "(spec.funct6(1) == 6'h2E) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnclipu_wv_active    "(spec.funct6(1) == 6'h2E) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnclipu_wx_active    "(spec.funct6(1) == 6'h2E) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnclip_wi_active     "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnclip_wv_active     "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnclip_wx_active     "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"

lemma result_vnclipu_wi    = ($vnclipu_wi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vnclipu_wv    = ($vnclipu_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vnclipu_wx    = ($vnclipu_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vnclip_wi     = ($vnclip_wi_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vnclip_wv     = ($vnclip_wv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vnclip_wx     = ($vnclip_wx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))

lemma vxsat_vnclipu_wi     = ($vnclipu_wi_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
lemma vxsat_vnclipu_wv     = ($vnclipu_wv_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
lemma vxsat_vnclipu_wx     = ($vnclipu_wx_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
lemma vxsat_vnclip_wi      = ($vnclip_wi_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
lemma vxsat_vnclip_wv      = ($vnclip_wv_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
lemma vxsat_vnclip_wx      = ($vnclip_wx_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))


#set vaaddu_vv_active    "(spec.funct6(1) == 6'h08) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vaaddu_vx_active    "(spec.funct6(1) == 6'h08) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vaadd_vv_active     "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vaadd_vx_active     "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vasubu_vv_active    "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vasubu_vx_active    "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vasub_vv_active     "(spec.funct6(1) == 6'h0B) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vasub_vx_active     "(spec.funct6(1) == 6'h0B) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsaddu_vi_active    "(spec.funct6(1) == 6'h20) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsaddu_vv_active    "(spec.funct6(1) == 6'h20) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsaddu_vx_active    "(spec.funct6(1) == 6'h20) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsadd_vi_active     "(spec.funct6(1) == 6'h21) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsadd_vv_active     "(spec.funct6(1) == 6'h21) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsadd_vx_active     "(spec.funct6(1) == 6'h21) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssubu_vv_active    "(spec.funct6(1) == 6'h22) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssubu_vx_active    "(spec.funct6(1) == 6'h22) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssub_vv_active     "(spec.funct6(1) == 6'h23) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssub_vx_active     "(spec.funct6(1) == 6'h23) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssrl_vi_active     "(spec.funct6(1) == 6'h2A) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssrl_vv_active     "(spec.funct6(1) == 6'h2A) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssrl_vx_active     "(spec.funct6(1) == 6'h2A) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssra_vi_active     "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssra_vv_active     "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vssra_vx_active     "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"


#lemma result_vaaddu_vv    = ($vaaddu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vaaddu_vx    = ($vaaddu_vx_active    && ($common_split_rule)) -> (spec.vd(0) == impl.auto_writeback_out_bits_data(7))
#lemma result_vaadd_vv     = ($vaadd_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vaadd_vx     = ($vaadd_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vasubu_vv    = ($vasubu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vasubu_vx    = ($vasubu_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vasub_vv     = ($vasub_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vasub_vx     = ($vasub_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vasub_vv     = ($vasub_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsaddu_vi    = ($vsaddu_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsaddu_vv    = ($vsaddu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsaddu_vx    = ($vsaddu_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsadd_vi     = ($vsadd_vi_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsadd_vv     = ($vsadd_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsadd_vx     = ($vsadd_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssubu_vv    = ($vssubu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssubu_vx    = ($vssubu_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssub_vv     = ($vssub_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssrl_vi     = ($vssrl_vi_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssrl_vv     = ($vssrl_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssrl_vx     = ($vssrl_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssra_vi     = ($vssra_vi_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssra_vv     = ($vssra_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vssra_vx     = ($vssra_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#
#
#lemma vxsat_vsaddu_vi     = ($vsaddu_vi_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vsaddu_vv     = ($vsaddu_vv_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vsaddu_vx     = ($vsaddu_vx_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vsadd_vi      = ($vsadd_vi_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vsadd_vv      = ($vsadd_vv_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vsadd_vx      = ($vsadd_vx_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vssubu_vv     = ($vssubu_vv_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vssubu_vx     = ($vssubu_vx_active    && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vssub_vv      = ($vssub_vv_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))
#lemma vxsat_vssub_vx      = ($vssub_vx_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(7))


lemma valid  = impl.auto_writeback_out_valid(7) == 1'b1

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






