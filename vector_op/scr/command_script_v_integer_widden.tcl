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
              ../../../common/cmodel/hector_wrapper/v_integer_widden.cpp
         
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
assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma(5)
assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta(5)

assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6(5)
assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1) < 3'b011

assume spec.eewType_1(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_1(5)
assume spec.eewType_2(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_2(5)

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h11

assume -always impl.redirectIn_valid == 1'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
#widden narrow
#set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h8))"
#set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))"
#set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h2))"

#narrow_to_1
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h10))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h8))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

#Mapping for -------------------------------------------------------LMUL <= 1
set vf8_split_rule "1'b0"
set vf8_split_rule "$vf8_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

#Mapping for -------------------------------------------------------LMUL <= 1
set vf4_split_rule "1'b0"
set vf4_split_rule "$vf4_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set vf4_split_rule "$vf4_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

#Mapping for -------------------------------------------------------LMUL <= 1
set vf2_split_rule "1'b0"
set vf2_split_rule "$vf2_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h8))"
set vf2_split_rule "$vf2_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set vf2_split_rule "$vf2_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"


set vwadd_vv_active    "(spec.funct6(1) == 6'h31) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwadd_vx_active    "(spec.funct6(1) == 6'h31) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwadd_wv_active    "(spec.funct6(1) == 6'h35) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwadd_wx_active    "(spec.funct6(1) == 6'h35) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwaddu_vv_active    "(spec.funct6(1) == 6'h30) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwaddu_vx_active    "(spec.funct6(1) == 6'h30) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwaddu_wv_active    "(spec.funct6(1) == 6'h34) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwaddu_wx_active    "(spec.funct6(1) == 6'h34) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwsub_vv_active    "(spec.funct6(1) == 6'h33) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwsub_vx_active    "(spec.funct6(1) == 6'h33) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwsub_wv_active    "(spec.funct6(1) == 6'h37) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwsub_wx_active    "(spec.funct6(1) == 6'h37) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwsubu_vv_active    "(spec.funct6(1) == 6'h32) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwsubu_vx_active    "(spec.funct6(1) == 6'h32) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h3)"
set vwsubu_wv_active    "(spec.funct6(1) == 6'h36) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"
set vwsubu_wx_active    "(spec.funct6(1) == 6'h36) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.eewType_1(1) == 3'h4)"

set vnsra_wi_active    "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnsra_wv_active    "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnsra_wx_active    "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnsrl_wi_active    "(spec.funct6(1) == 6'h2C) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnsrl_wv_active    "(spec.funct6(1) == 6'h2C) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"
set vnsrl_wx_active    "(spec.funct6(1) == 6'h2C) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.eewType_2(1) == 3'h3)"

set vmadc_vvm_active    "(spec.funct6(1) == 6'h11) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b1) && (spec.vta(1) == 1'b1)"
set vmadc_vv_active     "(spec.funct6(1) == 6'h11) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b0) && (spec.vta(1) == 1'b1)"
set vmadc_vxm_active    "(spec.funct6(1) == 6'h11) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b1) && (spec.vta(1) == 1'b1)"
set vmadc_vx_active     "(spec.funct6(1) == 6'h11) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b0) && (spec.vta(1) == 1'b1)"
set vmadc_vim_active    "(spec.funct6(1) == 6'h11) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b1) && (spec.vta(1) == 1'b1)"
set vmadc_vi_active     "(spec.funct6(1) == 6'h11) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b0) && (spec.vta(1) == 1'b1)"
set vmsbc_vvm_active    "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b1) && (spec.vta(1) == 1'b1)"
set vmsbc_vv_active     "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b0) && (spec.vta(1) == 1'b1)"
set vmsbc_vxm_active    "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b1) && (spec.vta(1) == 1'b1)"
set vmsbc_vx_active     "(spec.funct6(1) == 6'h13) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.Mask(1) == 1'b0) && (spec.vta(1) == 1'b1)"

set vmseq_vi_active    "(spec.funct6(1) == 6'h18) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmseq_vv_active    "(spec.funct6(1) == 6'h18) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmseq_vx_active    "(spec.funct6(1) == 6'h18) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsgt_vi_active    "(spec.funct6(1) == 6'h1F) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsgt_vx_active    "(spec.funct6(1) == 6'h1F) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsgtu_vi_active   "(spec.funct6(1) == 6'h1E) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsgtu_vx_active   "(spec.funct6(1) == 6'h1E) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsle_vi_active    "(spec.funct6(1) == 6'h1D) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsle_vv_active    "(spec.funct6(1) == 6'h1D) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsle_vx_active    "(spec.funct6(1) == 6'h1D) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsleu_vi_active   "(spec.funct6(1) == 6'h1C) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsleu_vv_active   "(spec.funct6(1) == 6'h1C) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsleu_vx_active   "(spec.funct6(1) == 6'h1C) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmslt_vv_active    "(spec.funct6(1) == 6'h1B) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmslt_vx_active    "(spec.funct6(1) == 6'h1B) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsltu_vv_active   "(spec.funct6(1) == 6'h1A) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsltu_vx_active   "(spec.funct6(1) == 6'h1A) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsne_vi_active    "(spec.funct6(1) == 6'h19) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsne_vv_active    "(spec.funct6(1) == 6'h19) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"
set vmsne_vx_active    "(spec.funct6(1) == 6'h19) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b1) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b1) && (spec.eewType_2(1) != 3'h3) && (spec.vta(1) == 1'b1)"



#set vsub_vv_active    "(spec.funct6(1) == 6'h02) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsub_vx_active    "(spec.funct6(1) == 6'h02) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vrsub_vv_active   "(spec.funct6(1) == 6'h03) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vrsub_vx_active   "(spec.funct6(1) == 6'h03) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vminu_vv_active   "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vminu_vx_active   "(spec.funct6(1) == 6'h04) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vmin_vv_active    "(spec.funct6(1) == 6'h05) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vmin_vx_active    "(spec.funct6(1) == 6'h05) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vmaxu_vv_active   "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vmaxu_vx_active   "(spec.funct6(1) == 6'h06) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vmax_vv_active    "(spec.funct6(1) == 6'h07) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vmax_vx_active    "(spec.funct6(1) == 6'h07) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vand_vi_active    "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vand_vv_active    "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vand_vx_active    "(spec.funct6(1) == 6'h09) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vor_vi_active     "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vor_vv_active     "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vor_vx_active     "(spec.funct6(1) == 6'h0A) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vxor_vi_active    "(spec.funct6(1) == 6'h0B) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vxor_vv_active    "(spec.funct6(1) == 6'h0B) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vxor_vx_active    "(spec.funct6(1) == 6'h0B) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vadc_vim_active   "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vadc_vvm_active   "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vadc_vxm_active   "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vsdc_vvm_active   "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vsdc_vxm_active   "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vzext_vf8_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.imm(1) == 5'h02)"
#set vsext_vf8_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.imm(1) == 5'h03)"
#set vzext_vf4_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.imm(1) == 5'h04)"
#set vsext_vf4_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.imm(1) == 5'h05)"
#set vzext_vf2_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.imm(1) == 5'h06)"
#set vsext_vf2_active  "(spec.funct6(1) == 6'h12) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.imm(1) == 5'h07)"
#set vmv_v_i_active    "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
#set vmv_v_v_active    "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
#set vmv_v_x_active    "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0)"
#set vmerge_vim_active "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vmerge_vvm_active "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vmerge_vxm_active "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b1)"
#set vsll_vi_active    "(spec.funct6(1) == 6'h25) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsll_vv_active    "(spec.funct6(1) == 6'h25) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsll_vx_active    "(spec.funct6(1) == 6'h25) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsrl_vi_active    "(spec.funct6(1) == 6'h28) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsrl_vv_active    "(spec.funct6(1) == 6'h28) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsrl_vx_active    "(spec.funct6(1) == 6'h28) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsra_vi_active    "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsra_vv_active    "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h00) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"
#set vsra_vx_active    "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h04) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0)"


#lemma result_vwadd_vv    = ($vwadd_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwadd_vx    = ($vwadd_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwadd_wv    = ($vwadd_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwadd_wx    = ($vwadd_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwaddu_vv    = ($vwaddu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwaddu_vx    = ($vwaddu_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwaddu_wv    = ($vwaddu_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwaddu_wx    = ($vwaddu_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsub_vv    = ($vwadd_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsub_vx    = ($vwadd_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsub_wv    = ($vwadd_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsub_wx    = ($vwadd_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsubu_vv    = ($vwaddu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsubu_vx    = ($vwaddu_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsubu_wv    = ($vwaddu_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vwsubu_wx    = ($vwaddu_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#
#lemma result_vnsra_wi    = ($vnsra_wi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vnsra_wv    = ($vnsra_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vnsra_wx    = ($vnsra_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vnsrl_wi    = ($vnsrl_wi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vnsrl_wv    = ($vnsrl_wv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vnsrl_wx    = ($vnsrl_wx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))

lemma result_vmadc_vvm    = ($vmadc_vvm_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmadc_vv     = ($vmadc_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmadc_vxm    = ($vmadc_vxm_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmadc_vx     = ($vmadc_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmadc_vim    = ($vmadc_vim_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmadc_vi     = ($vmadc_vi_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsbc_vvm    = ($vmsbc_vvm_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsbc_vv     = ($vmsbc_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsbc_vxm    = ($vmsbc_vxm_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsbc_vx     = ($vmsbc_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))

lemma result_vmseq_vi    = ($vmseq_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmseq_vv    = ($vmseq_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmseq_vx    = ($vmseq_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsgt_vi    = ($vmsgt_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsgt_vx    = ($vmsgt_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsgtu_vi   = ($vmsgtu_vi_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsgtu_vx   = ($vmsgtu_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsle_vi    = ($vmsle_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsle_vv    = ($vmsle_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsle_vx    = ($vmsle_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsleu_vi   = ($vmsleu_vi_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsleu_vv   = ($vmsleu_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsleu_vx   = ($vmsleu_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmslt_vv    = ($vmslt_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmslt_vx    = ($vmslt_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsltu_vv   = ($vmsltu_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsltu_vx   = ($vmsltu_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsne_vi    = ($vmsne_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsne_vv    = ($vmsne_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsne_vx    = ($vmsne_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))

#lemma result_vsub_vx    = ($vsub_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vrsub_vv   = ($vrsub_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vrsub_vx   = ($vrsub_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vminu_vv   = ($vminu_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vminu_vx   = ($vminu_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmin_vv    = ($vmin_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmin_vx    = ($vmin_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmaxu_vv   = ($vmaxu_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmaxu_vx   = ($vmaxu_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmax_vv    = ($vmax_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmax_vx    = ($vmax_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vand_vi    = ($vand_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vand_vv    = ($vand_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vand_vx    = ($vand_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vor_vi     = ($vor_vi_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vor_vv     = ($vor_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vor_vx     = ($vor_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vxor_vi    = ($vxor_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vxor_vv    = ($vxor_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vxor_vx    = ($vxor_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vadc_vim   = ($vadc_vim_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vadc_vvm   = ($vadc_vvm_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vadc_vxm   = ($vadc_vxm_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsdc_vvm   = ($vsdc_vvm_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsdc_vxm   = ($vsdc_vxm_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vzext_vf8  = ($vzext_vf8_active  && ($vf8_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsext_vf8  = ($vsext_vf8_active  && ($vf8_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vzext_vf4  = ($vzext_vf4_active  && ($vf4_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsext_vf4  = ($vsext_vf4_active  && ($vf4_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vzext_vf2  = ($vzext_vf2_active  && ($vf2_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsext_vf2  = ($vsext_vf2_active  && ($vf2_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#
#lemma result_vmv_v_i    = ($vmv_v_i_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmv_v_v    = ($vmv_v_v_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmv_v_x    = ($vmv_v_x_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmerge_vim = ($vmerge_vim_active && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmerge_vvm = ($vmerge_vvm_active && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmerge_vxm = ($vmerge_vxm_active && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsll_vi    = ($vsll_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsll_vv    = ($vsll_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsll_vx    = ($vsll_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsrl_vi    = ($vsrl_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsrl_vv    = ($vsrl_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsrl_vx    = ($vsrl_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsra_vi    = ($vsra_vi_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsra_vv    = ($vsra_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vsra_vx    = ($vsra_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))


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






