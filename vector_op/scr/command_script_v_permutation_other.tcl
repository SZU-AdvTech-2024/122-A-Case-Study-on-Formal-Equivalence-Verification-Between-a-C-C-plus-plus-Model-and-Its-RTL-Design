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
              ../../../common/cmodel/hector_wrapper/v_permutation_other.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VAluExu -clock clock

#        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f
        #vcs -sverilog /nfs/home/guanmingxing/work-nh3/share_for_formal/SimTop.sv
        vcs -sverilog {+define+SYNTHESIS  -f /nfs/projects/nanhu-v3/panlingfeng/Nanhu-V3-Release-Jan-23-2024-fd1f0289-100TAG/rtl/bosc_XSTop.f}

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
assume spec.vstart(1)  == impl.io_vstart(5)
assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma(5)
assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta(5)

assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6(5)
assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1) < 3'b100

assume spec.fuType(1)    == impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5)
assume spec.fuOpType(1)  == impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(5)
assume spec.srcType_0(1) == impl.auto_issue_in_issue_bits_uop_ctrl_srcType_0(5)

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
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

set vmv_x_s_active   "(spec.fuType(1) == 5'h11) && (spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h02) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h00)"
set vmv_s_x_active   "(spec.fuType(1) == 5'h17) && (spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h06) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h00) && (spec.srcType_0(1) == 3'h0) && (spec.fuOpType(1) == 7'h2)"
set vfmv_f_s_active  "(spec.fuType(1) == 5'h11) && (spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h00)"
set vfmv_s_f_active  "(spec.fuType(1) == 5'h17) && (spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h05) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h00) && (spec.srcType_0(1) == 3'h2) && (spec.fuOpType(1) == 7'h2)"

set vmv1r_v_active   "(spec.fuType(1) == 5'h11) && (spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h00)"
set vmv2r_v_active   "(spec.fuType(1) == 5'h11) && (spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h01)"
set vmv4r_v_active   "(spec.fuType(1) == 5'h11) && (spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h03)"
set vmv8r_v_active   "(spec.fuType(1) == 5'h11) && (spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h03) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.Mask(1) == 1'b0) && (spec.imm(1) == 5'h07)"

#lemma result_vmv_x_s    = ($vmv_x_s_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmv_s_x    = ($vmv_s_x_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vfmv_f_s   = ($vfmv_f_s_active   && ($vf4_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vfmv_s_f   = ($vfmv_s_f_active   && ($vf4_split_rule))    -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#
#lemma result_vmv1r_v    = ($vmv1r_v_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmv2r_v    = ($vmv2r_v_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmv4r_v    = ($vmv4r_v_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
#lemma result_vmv8r_v    = ($vmv8r_v_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))

lemma valu_valid  = (spec.fuType(1) == 5'h11) -> (impl.auto_writeback_out_valid(7) == 1'b1)
lemma s2v_valid   = (spec.fuType(1) == 5'h17) -> (impl.auto_writeback_out_valid(7) == 1'b1)

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






