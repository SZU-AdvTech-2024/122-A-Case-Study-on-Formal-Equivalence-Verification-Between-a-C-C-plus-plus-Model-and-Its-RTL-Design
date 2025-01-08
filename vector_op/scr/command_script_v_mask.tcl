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
              ../../../common/cmodel/hector_wrapper/v_mask.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {

        create_design -name impl -top bosc_VAluExu -clock clock

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f
        #vcs -sverilog /nfs/home/guanmingxing/work-nh3/share_for_formal/SimTop.sv

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
assume spec.Mask(1)    == impl.auto_issue_in_issue_bits_uop_vctrl_vm(5)
assume spec.vstart(1)  == impl.io_vstart(5)
assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma(5)
assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta(5)

assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6(5)
assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1)    <  3'b100

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h14

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h10))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h8))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set all_vl_rule "spec.vl(1) <= 8'h80"



set vcpop_m_active      "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b1) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h10)"
set vfirst_m_active     "(spec.funct6(1) == 6'h10) && (spec.funct3(1) == 3'h02) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h11)"
set vmsbf_m_active      "(spec.funct6(1) == 6'h14) && (spec.funct3(1) == 3'h02) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h01)"
set vmsof_m_active      "(spec.funct6(1) == 6'h14) && (spec.funct3(1) == 3'h02) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h02)"
set vmsif_m_active      "(spec.funct6(1) == 6'h14) && (spec.funct3(1) == 3'h02) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h03)"
set viota_m_active      "(spec.funct6(1) == 6'h14) && (spec.funct3(1) == 3'h02) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h10)"
set vid_v_active        "(spec.funct6(1) == 6'h14) && (spec.funct3(1) == 3'h02) && (spec.vstart(1) == 7'h0) && (spec.imm(1) == 5'h11)"
set vmandn_mm_active    "(spec.funct6(1) == 6'h18) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmand_mm_active     "(spec.funct6(1) == 6'h19) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmor_mm_active      "(spec.funct6(1) == 6'h1A) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmxor_mm_active     "(spec.funct6(1) == 6'h1B) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmorn_mm_active     "(spec.funct6(1) == 6'h1C) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmnand_mm_active    "(spec.funct6(1) == 6'h1D) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmnor_mm_active     "(spec.funct6(1) == 6'h1E) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"
set vmxnor_mm_active    "(spec.funct6(1) == 6'h1F) && (spec.funct3(1) == 3'h02) && (spec.Mask(1) == 1'b0)"


lemma result_vcpop_m    = ($vcpop_m_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vfirst_m   = ($vfirst_m_active    && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsbf_m    = ($vmsbf_m_active     && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsof_m    = ($vmsof_m_active     && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmsif_m    = ($vmsif_m_active     && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_viota_m    = ($viota_m_active     && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vid_v      = ($vid_v_active       && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmandn_mm  = ($vmandn_mm_active   && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmand_mm   = ($vmand_mm_active    && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmor_mm    = ($vmor_mm_active     && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmxor_mm   = ($vmxor_mm_active    && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmorn_mm   = ($vmorn_mm_active    && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmnand_mm  = ($vmnand_mm_active   && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmnor_mm   = ($vmnor_mm_active    && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))
lemma result_vmxnor_mm  = ($vmxnor_mm_active   && ($all_vl_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(7))


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






