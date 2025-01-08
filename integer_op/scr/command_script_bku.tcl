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
              ../../../common/cmodel/hector_wrapper/bku.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_MulExu -clock clock

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

assume spec.rs1(1)       == impl.auto_issue_in_issue_bits_src_0[63:0](5)
assume spec.rs2(1)       == impl.auto_issue_in_issue_bits_src_1[63:0](5)

assume -always impl.redirectIn_valid == 1'b0
assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuType == 5'b00111

#bku optypoe
lemma result_clmul      = (spec.fuOpType(1) == 7'h00) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_clmulh     = (spec.fuOpType(1) == 7'h01) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_clmulr     = (spec.fuOpType(1) == 7'h02) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_xpermn     = (spec.fuOpType(1) == 7'h04) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_xpermb     = (spec.fuOpType(1) == 7'h05) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_clz        = (spec.fuOpType(1) == 7'h08) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_clzw       = (spec.fuOpType(1) == 7'h09) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_ctz        = (spec.fuOpType(1) == 7'h0A) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_ctzw       = (spec.fuOpType(1) == 7'h0B) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_cpop       = (spec.fuOpType(1) == 7'h0C) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_cpopw      = (spec.fuOpType(1) == 7'h0D) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64es    = (spec.fuOpType(1) == 7'h20) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64esm   = (spec.fuOpType(1) == 7'h21) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64ds    = (spec.fuOpType(1) == 7'h22) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64dsm   = (spec.fuOpType(1) == 7'h23) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64im    = (spec.fuOpType(1) == 7'h24) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64ks1i  = ((spec.fuOpType(1) == 7'h25) && (impl.auto_issue_in_issue_bits_src_1[63:0](5) <= 64'hA)) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_aes64ks2   = (spec.fuOpType(1) == 7'h26) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ed0     = (spec.fuOpType(1) == 7'h28) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ed1     = (spec.fuOpType(1) == 7'h29) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ed2     = (spec.fuOpType(1) == 7'h2A) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ed3     = (spec.fuOpType(1) == 7'h2B) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ks0     = (spec.fuOpType(1) == 7'h2C) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ks1     = (spec.fuOpType(1) == 7'h2D) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ks2     = (spec.fuOpType(1) == 7'h2E) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm4ks3     = (spec.fuOpType(1) == 7'h2F) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha256sum0 = (spec.fuOpType(1) == 7'h30) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha256sum1 = (spec.fuOpType(1) == 7'h31) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha256sig0 = (spec.fuOpType(1) == 7'h32) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha256sig1 = (spec.fuOpType(1) == 7'h33) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha512sum0 = (spec.fuOpType(1) == 7'h34) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha512sum1 = (spec.fuOpType(1) == 7'h35) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha512sig0 = (spec.fuOpType(1) == 7'h36) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sha512sig1 = (spec.fuOpType(1) == 7'h37) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm3p0      = (spec.fuOpType(1) == 7'h38) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))
lemma result_sm3p1      = (spec.fuOpType(1) == 7'h39) -> (spec.rd(1) == impl.auto_writeback_out_bits_data[63:0](9))

lemma valid  = impl.auto_writeback_out_valid(9) == 1'b1

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






