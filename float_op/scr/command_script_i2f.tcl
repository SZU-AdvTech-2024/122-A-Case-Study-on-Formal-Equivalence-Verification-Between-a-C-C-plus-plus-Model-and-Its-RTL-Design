
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
          -I ../../../common/cmodel/softfloat-3e/build/HECTOR \
          -I ../../../common/cmodel/softfloat-3e/build/Linux-x86_64-GCC \
          -I ../../../common/cmodel/softfloat-3e/source/RISCV \
          -I ../../../common/cmodel/softfloat-3e/source/include \
          -I ../../../common/cmodel/acl2/include \
          ../../../common/cmodel/hector_wrapper/i2f.cc \
          $SF_OPTIONS

    compile_design spec
}

#========================================================
#    compile_impl
#========================================================
proc compile_impl {} {
    create_design -name impl -top IntToFP -clock clock

    vcs -sverilog /nfs/home/panlingfeng/debug_code/i2f.sv 

    compile_design impl
}

#========================================================
#    user define
#========================================================
proc ual {} {

    for {set i 0} {$i <= 9} {incr i} {
       if {$i < 3} {
          assume impl.reset($i) == 1'b1
          assume impl.io_in_valid($i) == 1'b0
       } elseif {$i < 5} {
          assume impl.reset($i) == 1'b0
          assume impl.io_in_valid($i) == 1'b0
       } elseif {$i < 7} {
          assume impl.reset($i) == 1'b0
          assume impl.io_in_valid($i) == 1'b1
       } else {
          assume impl.reset($i) == 1'b0
          assume impl.io_in_valid($i) == 1'b0
       }
    }

    assume -always impl.io_redirectIn_valid == 1'b0

    assume spec.uirs1(1)   == impl.io_in_bits_src_0(5)
    assume spec.irs1(1)    == impl.io_in_bits_src_0(5)
    assume spec.rm(1)      == impl.rm(5)

    assume spec.TagOut(1) == impl.io_in_bits_uop_ctrl_fpu_typeTagOut(5)
    assume spec.wflags(1) == impl.io_in_bits_uop_ctrl_fpu_wflags(5)
    assume spec.typ(1)    == impl.io_in_bits_uop_ctrl_fpu_typ(5)
    
    set fmv_w_x_active      "(spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b0) && (spec.typ(1) == 2'b00)"
    set fcvt_s_w_active     "(spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b00)"
    set fcvt_s_wu_active    "(spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b01)"
    set fcvt_s_l_active     "(spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b10)"
    set fcvt_s_lu_active    "(spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b11)"
    set fmv_d_x_active      "(spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b0) && (spec.typ(1) == 2'b00)"
    set fcvt_d_w_active     "(spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b00)"
    set fcvt_d_wu_active    "(spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b01)"
    set fcvt_d_l_active     "(spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b10)"
    set fcvt_d_lu_active    "(spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b11)"
    set fmv_h_x_active      "(spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b0) && (spec.typ(1) == 2'b00)"
    set fcvt_h_w_active     "(spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b00)"
    set fcvt_h_wu_active    "(spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b01)"
    set fcvt_h_l_active     "(spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b10)"
    set fcvt_h_lu_active    "(spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.typ(1) == 2'b11)"

#    set rm_enumerate_rule "(spec.rm(1) <= 4) || ((spec.rm(1) == 7) && (spec.csr_frm(1) <= 4))"
#    set rm_assume0_rule   "(spec.rm(1) == 0) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 0))"
#    set rm_assume1_rule   "(spec.rm(1) == 1) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 1))"
#    set rm_assume2_rule   "(spec.rm(1) == 2) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 2))"
#    set rm_assume3_rule   "(spec.rm(1) == 3) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 3))"
#    set rm_assume4_rule   "(spec.rm(1) == 4) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 4))"
#    set rm_assume5_rule   "(spec.rm(1) == 5) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 5))"
#    set rm_assume6_rule   "(spec.rm(1) == 6) || ((spec.rm(1) == 7) && (spec.csr_frm(1) == 6))"

    set rm_enumerate_rule "(spec.rm(1) <= 4)"
    set rm_assume0_rule   "(spec.rm(1) == 0)"
    set rm_assume1_rule   "(spec.rm(1) == 1)"
    set rm_assume2_rule   "(spec.rm(1) == 2)"
    set rm_assume3_rule   "(spec.rm(1) == 3)"
    set rm_assume4_rule   "(spec.rm(1) == 4)"
    set rm_assume5_rule   "(spec.rm(1) == 5)"
    set rm_assume6_rule   "(spec.rm(1) == 6)"

    set result   "(impl.io_out_valid(7) == 1'b1) && (spec.rd(1) == impl.io_out_bits_data(9)) && (spec.exceptions(1) == impl.fflags(9))"
    set resultNe "(impl.io_out_valid(7) == 1'b1) && (spec.rd(1) == impl.io_out_bits_data(9))"

#    lemma valid = impl.auto_writeback_out_valid(9) == 1'b1
#    lemma check = spec.rd(1) == impl.auto_writeback_out_bits_data(9)

#    lemma inexact =   spec.exceptions[0](1) == impl.auto_writeback_out_bits_fflags[0](9)
#    lemma underflow = spec.exceptions[1](1) == impl.auto_writeback_out_bits_fflags[1](9)
#    lemma overflow =  spec.exceptions[2](1) == impl.auto_writeback_out_bits_fflags[2](9)
#    lemma infinity =  spec.exceptions[3](1) == impl.auto_writeback_out_bits_fflags[3](9)
#    lemma invalid =   spec.exceptions[4](1) == impl.auto_writeback_out_bits_fflags[4](9)

    lemma result_fmv_w_x   = (($fmv_w_x_active)    && ($rm_assume0_rule))   -> ($resultNe)
    lemma result_fcvt_s_w  = (($fcvt_s_w_active)   && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_s_wu = (($fcvt_s_wu_active)  && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_s_l  = (($fcvt_s_l_active)   && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_s_lu = (($fcvt_s_lu_active)  && ($rm_enumerate_rule)) -> ($result)
    lemma result_fmv_d_x   = (($fmv_d_x_active)    && ($rm_assume0_rule))   -> ($resultNe)
    lemma result_fcvt_d_w  = (($fcvt_d_w_active)   && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_d_wu = (($fcvt_d_wu_active)  && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_d_l  = (($fcvt_d_l_active)   && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_d_lu = (($fcvt_d_lu_active)  && ($rm_enumerate_rule)) -> ($result)
    lemma result_fmv_h_x   = (($fmv_h_x_active)    && ($rm_assume0_rule))   -> ($resultNe)
    lemma result_fcvt_h_w  = (($fcvt_h_w_active)   && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_h_wu = (($fcvt_h_wu_active)  && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_h_l  = (($fcvt_h_l_active)   && ($rm_enumerate_rule)) -> ($result)
    lemma result_fcvt_h_lu = (($fcvt_h_lu_active)  && ($rm_enumerate_rule)) -> ($result)


}

proc make {} {
    compile_spec
    compile_impl
    compose
}

proc run_main {} {
    set_user_assumes_lemmas_procedure "ual"
    solveNB p
#    proofwait
#    proofstatus
#    listproof

}
