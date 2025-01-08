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
              ../../../common/cmodel/hector_wrapper/alu.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_AluExu

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

assume spec.rs1(1)       == impl.auto_issue_in_issue_bits_src_0[63:0](1)
assume spec.rs2(1)       == impl.auto_issue_in_issue_bits_src_1[63:0](1)
assume spec.fuOpType(1)  == impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1)

assume impl.auto_issue_in_issue_valid(1) == 1'b1
assume impl.redirectIn_valid(1) == 1'b0
assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(1) == 5'b00110

#shift optypoe
lemma result_slliuw    = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h00) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sll       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h01) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_bclr      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h02) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_bset      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h03) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_binv      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h04) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_srl       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h05) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_bext      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h06) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sra       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h07) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_rol       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h09) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_ror       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h0B) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
#word optypoe
lemma result_addw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h10) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_oddaddw   = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h11) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_subw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h12) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_addwbit   = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h14) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_addwbyte  = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h15) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_addwzexth = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h16) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_addwsexth = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h17) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sllw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h18) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_srlw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h19) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sraw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h1A) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_rolw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h1C) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_rorw      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h1D) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
#add optypoe
lemma result_adduw     = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h20) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_add       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h21) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_oddadd    = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h22) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sr29add   = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h24) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sr30add   = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h25) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sr31add   = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h26) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sr32add   = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h27) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh1adduw  = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h28) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh1add    = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h29) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh2adduw  = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h2A) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh2add    = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h2B) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh3adduw  = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h2C) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh3add    = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h2D) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sh4add    = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h2F) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
#sub optypoe
lemma result_sub       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h30) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sltu      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h31) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_slt       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h32) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_maxu      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h34) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_minu      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h35) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_max       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h36) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_min       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h37) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
#branch optypoe
lemma result_beq       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h70) -> (spec.taken(1) == impl.auto_writeback_out_bits_redirect_cfiUpdate_taken(1))
lemma result_bne       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h72) -> (spec.taken(1) == impl.auto_writeback_out_bits_redirect_cfiUpdate_taken(1))
lemma result_blt       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h78) -> (spec.taken(1) == impl.auto_writeback_out_bits_redirect_cfiUpdate_taken(1))
lemma result_bge       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h7A) -> (spec.taken(1) == impl.auto_writeback_out_bits_redirect_cfiUpdate_taken(1))
lemma result_bltu      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h7C) -> (spec.taken(1) == impl.auto_writeback_out_bits_redirect_cfiUpdate_taken(1))
lemma result_bgeu      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h7E) -> (spec.taken(1) == impl.auto_writeback_out_bits_redirect_cfiUpdate_taken(1))
#misc optypoe
lemma result_and       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h40) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_andn      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h41) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_or        = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h42) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_orn       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h43) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_xor       = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h44) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_xnor      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h45) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_orcb      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h46) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sextb     = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h48) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_packh     = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h49) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_sexth     = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h4A) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_packw     = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h4B) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_revb      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h50) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_rev8      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h51) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_pack      = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h52) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))
lemma result_orh48     = (impl.auto_issue_in_issue_bits_uop_ctrl_fuOpType(1) == 7'h53) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](1))

lemma valid  = impl.auto_writeback_out_valid(1) == 1'b1

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






