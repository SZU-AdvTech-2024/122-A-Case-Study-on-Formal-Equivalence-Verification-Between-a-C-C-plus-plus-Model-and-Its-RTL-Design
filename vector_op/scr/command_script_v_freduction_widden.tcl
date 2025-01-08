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
              ../../../common/cmodel/hector_wrapper/v_freduction_widden.cpp \
              $SF_OPTIONS
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VFpExu -clock clock
        #create_design -name impl -top VFpExu -clock clock

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f
        #vcs -sverilog /nfs/home/guanmingxing/work-nh3/share_for_formal/SimTop.sv

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

for {set i 0} {$i <= 72} {incr i} {
   if {$i < 3} {
      assume impl.reset($i) == 1'b1
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
   } elseif {$i < 5} {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
   } elseif {$i < 27} {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b1
      assume spec.Narrow(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isNarrow($i)
      assume spec.Widden(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isWidden($i)
      assume spec.MaskOp(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_maskOp($i)
      assume spec.Mask(1)    == impl.auto_issue_in_issue_bits_uop_vctrl_vm($i)
      assume spec.vstart(1)  == impl.io_vstart($i)
      assume spec.vxrm(1)    == impl.io_vcsr[2:1]($i)
      assume spec.frm(1)     == impl.io_frm($i)
      assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma($i)
      assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta($i)
      
      assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6($i)
      assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3($i)
      
      assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl($i)
      assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew($i)
   } else {
      assume impl.reset($i) == 1'b0
      assume impl.auto_issue_in_issue_valid($i) == 1'b0
   }
}

assume spec.vs1(1)     == impl.auto_issue_in_issue_bits_src_0(25)
assume spec.vs2(1)     == impl.auto_issue_in_issue_bits_src_1(25)
assume spec.oldvd(1)   == impl.auto_issue_in_issue_bits_src_2(25)
assume spec.vm(1)      == impl.auto_issue_in_issue_bits_vm(25)
assume spec.imm(1)     == impl.auto_issue_in_issue_bits_uop_ctrl_imm[4:0](25)
assume spec.eewType_1(1) == impl.auto_issue_in_issue_bits_uop_vctrl_eewType_1(25)

#assume impl.auto_issue_in_issue_bits_src_0(25) == 128'h80000000000000008000000000000000 
#assume impl.auto_issue_in_issue_bits_src_1(25) == 128'h80000000000000008000000000000000
#assume impl.auto_issue_in_issue_bits_src_0(25) == 128'h00000000000000000000000000000000 
#assume impl.auto_issue_in_issue_bits_src_1(25) == 128'h00000000000000000000000000000000

assume spec.vsew(1) < 3'b100
assume spec.frm(1)  < 3'b100

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_ctrl_fuType == 5'h12
assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set sew32_split_rule "((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
#set sew64_split_rule "((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

#set sew32_split_rule "((spec.vsew(1) == 3'b010) && (spec.vl(1) == 8'h1))"
#set sew64_split_rule "((spec.vsew(1) == 3'b011) && (spec.vl(1) == 8'h1))"

set vfwredusum_vs_active        "(spec.funct6(1) == 6'h31) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.vstart(1) == 7'h0) && (spec.eewType_1(1) == 3'h3)"
set vfwredosum_vs_active        "(spec.funct6(1) == 6'h33) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b1) && (spec.MaskOp(1) == 1'b0) && (spec.vstart(1) == 7'h0) && (spec.eewType_1(1) == 3'h3)"

#set vfredusum_vs_active        "(spec.funct6(1) == 6'h01) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
#set vfredosum_vs_active        "(spec.funct6(1) == 6'h03) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
#set vfredmin_vs_active         "(spec.funct6(1) == 6'h05) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.vstart(1) == 7'h0)"
#set vfredmax_vs_active         "(spec.funct6(1) == 6'h07) && (spec.funct3(1) == 3'h01) && (spec.Narrow(1) == 1'b0) && (spec.Widden(1) == 1'b0) && (spec.MaskOp(1) == 1'b0) && (spec.vstart(1) == 7'h0)"


#-----------VFReduction Module 4 pipeline
lemma result_vfwredusum_vs_sew32 = ($vfwredusum_vs_active && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(71))
lemma result_vfwredosum_vs_sew32 = ($vfwredosum_vs_active && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(71))
lemma fflags_vfwredusum_vs_sew32 = ($vfwredusum_vs_active && ($sew32_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(71))
lemma fflags_vfwredosum_vs_sew32 = ($vfwredosum_vs_active && ($sew32_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(71))
lemma valid_vfwredusum_vs_sew32  = ($vfwredusum_vs_active && ($sew32_split_rule)) -> (impl.auto_writeback_out_valid(71) == 1'b1)
lemma valid_vfwredosum_vs_sew32  = ($vfwredosum_vs_active && ($sew32_split_rule)) -> (impl.auto_writeback_out_valid(71) == 1'b1)

#lemma result_vfredusum_vs_sew32 = ($vfredusum_vs_active && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(71))
#lemma result_vfredosum_vs_sew32 = ($vfredosum_vs_active && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(71))
#lemma result_vfredmin_vs_sew32  = ($vfredmin_vs_active  && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(55))
#lemma result_vfredmax_vs_sew32  = ($vfredmax_vs_active  && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(55))
#
#lemma fflags_vfredusum_vs_sew32 = ($vfredusum_vs_active && ($sew32_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(71))
#lemma fflags_vfredosum_vs_sew32 = ($vfredosum_vs_active && ($sew32_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(71))
#lemma fflags_vfredmin_vs_sew32  = ($vfredmin_vs_active  && ($sew32_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(55))
#lemma fflags_vfredmax_vs_sew32  = ($vfredmax_vs_active  && ($sew32_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(55))
#
#lemma valid_vfredusum_vs_sew32  = ($vfredusum_vs_active && ($sew32_split_rule)) -> (impl.auto_writeback_out_valid(71) == 1'b1)
#lemma valid_vfredosum_vs_sew32  = ($vfredosum_vs_active && ($sew32_split_rule)) -> (impl.auto_writeback_out_valid(71) == 1'b1)
#lemma valid_vfredmin_vs_sew32   = ($vfredmin_vs_active  && ($sew32_split_rule)) -> (impl.auto_writeback_out_valid(55) == 1'b1)
#lemma valid_vfredmax_vs_sew32   = ($vfredmax_vs_active  && ($sew32_split_rule)) -> (impl.auto_writeback_out_valid(55) == 1'b1)

#lemma result_vfredusum_vs_sew64 = ($vfredusum_vs_active && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(51))
#lemma result_vfredosum_vs_sew64 = ($vfredosum_vs_active && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(51))
#lemma result_vfredmin_vs_sew64  = ($vfredmin_vs_active  && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(47))
#lemma result_vfredmax_vs_sew64  = ($vfredmax_vs_active  && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(47))
#
#lemma fflags_vfredusum_vs_sew64 = ($vfredusum_vs_active && ($sew64_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(51))
#lemma fflags_vfredosum_vs_sew64 = ($vfredosum_vs_active && ($sew64_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(51))
#lemma fflags_vfredmin_vs_sew64  = ($vfredmin_vs_active  && ($sew64_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(47))
#lemma fflags_vfredmax_vs_sew64  = ($vfredmax_vs_active  && ($sew64_split_rule)) -> (spec.fflags(1) == impl.auto_writeback_out_bits_fflags(47))
#
#lemma valid_vfredusum_vs_sew64  = ($vfredusum_vs_active && ($sew64_split_rule)) -> (impl.auto_writeback_out_valid(51) == 1'b1)
#lemma valid_vfredosum_vs_sew64  = ($vfredosum_vs_active && ($sew64_split_rule)) -> (impl.auto_writeback_out_valid(51) == 1'b1)
#lemma valid_vfredmin_vs_sew64   = ($vfredmin_vs_active  && ($sew64_split_rule)) -> (impl.auto_writeback_out_valid(47) == 1'b1)
#lemma valid_vfredmax_vs_sew64   = ($vfredmax_vs_active  && ($sew64_split_rule)) -> (impl.auto_writeback_out_valid(47) == 1'b1)

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






