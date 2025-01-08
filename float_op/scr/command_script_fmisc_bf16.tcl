
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
          ../../../common/cmodel/hector_wrapper/fmisc_bf16.cc \
          $SF_OPTIONS

    compile_design spec
}

#========================================================
#    compile_impl
#========================================================
proc compile_impl {} {
    create_design -name impl -top FPToFP -clock clock
    vcs -sverilog /nfs/home/panlingfeng/debug_code/f2f.sv
#    create_design -name impl -top FPToInt -clock clock
#    vcs -sverilog /nfs/home/panlingfeng/debug_code/f2i.sv

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

    assume spec.rs1(1)     == impl.io_in_bits_src_0(5)
    assume spec.rs2(1)     == impl.io_in_bits_src_1(5)
    assume spec.rm(1)      == impl.rm(5)
#    assume spec.csr_frm(1) == impl.csr_frm(5)

    assume spec.rs1(1)     == 64'hFFFFFFFF8A09C39C
    assume spec.rm(1)      == 3'h3
    assume spec.tagIn(1)  == impl.io_in_bits_uop_ctrl_fpu_typeTagIn(5)
    assume spec.TagOut(1) == impl.io_in_bits_uop_ctrl_fpu_typeTagOut(5)
    assume spec.wflags(1) == impl.io_in_bits_uop_ctrl_fpu_wflags(5)
    assume spec.fcvt(1)   == impl.io_in_bits_uop_ctrl_fpu_fcvt(5)
    assume spec.isBf16(1) == impl.io_in_bits_uop_ctrl_fpu_isBf16(5)
#    assume spec.typ(1)    == impl.io_in_bits_uop_ctrl_fpu_typ(5)
#    assume spec.fmt(1)    == impl.io_in_bits_uop_ctrl_fpu_fmt(5)
#    assume spec.rfWen(1)  == impl.io_in_bits_uop_ctrl_rfWen(5)
    assume spec.fpWen(1)  == impl.io_in_bits_uop_ctrl_fpu_fpWen(5)

##FPToInt single
#    set fmv_x_w_active     "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fclass_s_active    "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fcvt_w_s_active    "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fcvt_wu_s_active   "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b01) && (spec.fmt(1) == 2'b00)"
#    set fcvt_l_s_active    "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b10) && (spec.fmt(1) == 2'b00)"
#    set fcvt_lu_s_active   "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b11) && (spec.fmt(1) == 2'b00)"
#    set feq_s_active       "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set flt_s_active       "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fle_s_active       "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
##FPToInt double
#    set fmv_x_d_active     "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fclass_d_active    "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fcvt_w_d_active    "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fcvt_wu_d_active   "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b01) && (spec.fmt(1) == 2'b01)"
#    set fcvt_l_d_active    "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b10) && (spec.fmt(1) == 2'b01)"
#    set fcvt_lu_d_active   "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b11) && (spec.fmt(1) == 2'b01)"
#    set feq_d_active       "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set flt_d_active       "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fle_d_active       "(spec.fuType(1) == 5'b01011) && (spec.rfWen(1) == 1'b1) && (spec.fpWen(1) == 1'b0) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
##FPToFP single
#    set fsgnj_s_active     "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b0) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fsgnjn_s_active    "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b0) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fsgnjx_s_active    "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b0) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fmin_s_active      "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b0) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
#    set fmax_s_active      "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b0) && (spec.TagOut(1) == 1'b0) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b00)"
##FPToFP double
#    set fsgnj_d_active     "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fsgnjn_d_active    "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fsgnjx_d_active    "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fmin_d_active      "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fmax_d_active      "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b1) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.typ(1) == 2'b00) && (spec.fmt(1) == 2'b01)"
#    set fcvt_s_d_active    "(spec.fuType(1) == 5'b01010) && (spec.rfWen(1) == 1'b0) && (spec.fpWen(1) == 1'b1) && (spec.tagIn(1) == 1'b1) && (spec.TagOut(1) == 1'b0) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1) && (spec.typ(1) == 2'b01) && (spec.fmt(1) == 2'b00)"
    set fcvt_d_s_active    "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b01) && (spec.fcvt(1) == 1'b1)"
    set fcvt_s_h_active    "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b00) && (spec.fcvt(1) == 1'b1) && (spec.isBf16(1) == 1'b0)"
    set fcvt_d_h_active    "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b01) && (spec.fcvt(1) == 1'b1)"
    set fcvt_s_d_active    "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b00) && (spec.fcvt(1) == 1'b1)"
    set fcvt_h_s_active    "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b10) && (spec.fcvt(1) == 1'b1) && (spec.isBf16(1) == 1'b0)"
    set fcvt_h_d_active    "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b10) && (spec.fcvt(1) == 1'b1)"
    set fsgnj_h_active     "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnjn_h_active    "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnjx_h_active    "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnj_s_active     "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnjn_s_active    "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnjx_s_active    "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnj_d_active     "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnjn_d_active    "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fsgnjx_d_active    "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fmin_h_active      "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b1)"
    set fmax_h_active      "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b1)"
    set fmin_s_active      "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b1)"
    set fmax_s_active      "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b1)"
    set fmin_d_active      "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b1)"
    set fmax_d_active      "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b1)"

    set feq_s_active       "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set flt_s_active       "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set fle_s_active       "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set feq_d_active       "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set flt_d_active       "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set fle_d_active       "(spec.tagIn(1) == 2'b01) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set feq_h_active       "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set flt_h_active       "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"
    set fle_h_active       "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b0) && (spec.fpWen(1) == 1'b0)"

    set fcvt_s_bf16_active    "(spec.tagIn(1) == 2'b10) && (spec.TagOut(1) == 2'b00) && (spec.fcvt(1) == 1'b1) && (spec.isBf16(1) == 1'b1)"
    set fcvt_bf16_s_active    "(spec.tagIn(1) == 2'b00) && (spec.TagOut(1) == 2'b10) && (spec.fcvt(1) == 1'b1) && (spec.isBf16(1) == 1'b1)"

    set fcvt_w_h_active    "(spec.tagIn(1) == 2'b10) && (spec.typ(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_wu_h_active   "(spec.tagIn(1) == 2'b10) && (spec.typ(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_l_h_active    "(spec.tagIn(1) == 2'b10) && (spec.typ(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_lu_h_active   "(spec.tagIn(1) == 2'b10) && (spec.typ(1) == 2'b11) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_w_s_active    "(spec.tagIn(1) == 2'b00) && (spec.typ(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_wu_s_active   "(spec.tagIn(1) == 2'b00) && (spec.typ(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_l_s_active    "(spec.tagIn(1) == 2'b00) && (spec.typ(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_lu_s_active   "(spec.tagIn(1) == 2'b00) && (spec.typ(1) == 2'b11) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_w_d_active    "(spec.tagIn(1) == 2'b01) && (spec.typ(1) == 2'b00) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_wu_d_active   "(spec.tagIn(1) == 2'b01) && (spec.typ(1) == 2'b01) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_l_d_active    "(spec.tagIn(1) == 2'b01) && (spec.typ(1) == 2'b10) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fcvt_lu_d_active   "(spec.tagIn(1) == 2'b01) && (spec.typ(1) == 2'b11) && (spec.wflags(1) == 1'b1) && (spec.fcvt(1) == 1'b1)"
    set fclass_h_active    "(spec.tagIn(1) == 2'b10) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fclass_s_active    "(spec.tagIn(1) == 2'b00) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fclass_d_active    "(spec.tagIn(1) == 2'b01) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fmv_x_h_active     "(spec.fmt(1) == 2'b10) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fmv_x_w_active     "(spec.fmt(1) == 2'b00) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"
    set fmv_x_d_active     "(spec.fmt(1) == 2'b01) && (spec.wflags(1) == 1'b0) && (spec.fcvt(1) == 1'b0)"


#    lemma valid = impl.auto_writeback_out_valid(9) == 1'b1
#    lemma check = spec.rd(1) == impl.auto_writeback_out_bits_data(9)

#    lemma inexact =   spec.exceptions[0](1) == impl.auto_writeback_out_bits_fflags[0](9)
#    lemma underflow = spec.exceptions[1](1) == impl.auto_writeback_out_bits_fflags[1](9)
#    lemma overflow =  spec.exceptions[2](1) == impl.auto_writeback_out_bits_fflags[2](9)
#    lemma infinity =  spec.exceptions[3](1) == impl.auto_writeback_out_bits_fflags[3](9)
#    lemma invalid =   spec.exceptions[4](1) == impl.auto_writeback_out_bits_fflags[4](9)

    set rm_enumerate_rule "(spec.rm(1) <= 4)"
    set rm_assume0_rule   "(spec.rm(1) == 0)"
    set rm_assume1_rule   "(spec.rm(1) == 1)"
    set rm_assume2_rule   "(spec.rm(1) == 2)"
    set rm_assume3_rule   "(spec.rm(1) == 3)"
    set rm_assume4_rule   "(spec.rm(1) == 4)"
    set rm_assume5_rule   "(spec.rm(1) == 5)"
    set rm_assume6_rule   "(spec.rm(1) == 6)"

    set result   "(impl.io_out_valid(9) == 1'b1) && (spec.rd(1) == impl.io_out_bits_data(9)) && (spec.exceptions(1) == impl.fflags(9))"
    set resultNe "(impl.io_out_valid(9) == 1'b1) && (spec.rd(1) != impl.io_out_bits_data(9))"
    set resultEq0   "(impl.io_out_valid(9) == 1'b1) && (spec.rd(1) == 64'h0) && (spec.exceptions(1) == 5'b0)"

##FPToInt single
#     lemma result_fmv_x_w   = (($fmv_x_w_active)    && ($rm_assume0_rule))   -> ($resultNe)
#     lemma result_fclass_s  = (($fclass_s_active)   && ($rm_assume1_rule))   -> ($resultNe)
#     lemma result_fcvt_w_s  = (($fcvt_w_s_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_wu_s = (($fcvt_wu_s_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_l_s  = (($fcvt_l_s_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_lu_s = (($fcvt_lu_s_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_feq_s     = (($feq_s_active)      && ($rm_assume2_rule))   -> ($result)
#     lemma result_flt_s     = (($flt_s_active)      && ($rm_assume1_rule))   -> ($result)
#     lemma result_fle_s     = (($fle_s_active)      && ($rm_assume0_rule))   -> ($result)
##FPToInt double
#     lemma result_fmv_x_d   = (($fmv_x_d_active)    && ($rm_assume0_rule))   -> ($resultNe)
#     lemma result_fclass_d  = (($fclass_d_active)   && ($rm_assume1_rule))   -> ($resultNe)
#     lemma result_fcvt_w_d  = (($fcvt_w_d_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_wu_d = (($fcvt_wu_d_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_l_d  = (($fcvt_l_d_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_lu_d = (($fcvt_lu_d_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_feq_d     = (($feq_d_active)      && ($rm_assume2_rule))   -> ($result)
#     lemma result_flt_d     = (($flt_d_active)      && ($rm_assume1_rule))   -> ($result)
#     lemma result_fle_d     = (($fle_d_active)      && ($rm_assume0_rule))   -> ($result)
#
##FPToFP single
#     lemma result_fsgnj_s   = (($fsgnj_s_active)    && ($rm_assume0_rule)) -> ($resultNe)
#     lemma result_fsgnjn_s  = (($fsgnjn_s_active)   && ($rm_assume1_rule)) -> ($resultNe)
#     lemma result_fsgnjx_s  = (($fsgnjx_s_active)   && ($rm_assume2_rule)) -> ($resultNe)
#     lemma result_fmin_s    = (($fmin_s_active)     && ($rm_assume0_rule)) -> ($result)
#     lemma result_fmax_s    = (($fmax_s_active)     && ($rm_assume1_rule)) -> ($result)
##FPToFP double
#     lemma result_fsgnj_d   = (($fsgnj_d_active)    && ($rm_assume0_rule)) -> ($resultNe)
#     lemma result_fsgnjn_d  = (($fsgnjn_d_active)   && ($rm_assume1_rule)) -> ($resultNe)
#     lemma result_fsgnjx_d  = (($fsgnjx_d_active)   && ($rm_assume2_rule)) -> ($resultNe)
#     lemma result_fcvt_d_s  = (($fcvt_d_s_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_s_d  = (($fcvt_s_d_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fmin_d    = (($fmin_d_active)     && ($rm_assume0_rule)) -> ($result)
#     lemma result_fmax_d    = (($fmax_d_active)     && ($rm_assume1_rule)) -> ($result)

#f2f
#     lemma result_fcvt_d_s  = ($fcvt_d_s_active) -> ($result)
#     lemma result_fcvt_s_h  = ($fcvt_s_h_active) -> ($result)
#     lemma result_fcvt_d_h  = ($fcvt_d_h_active) -> ($result)
#     lemma result_fcvt_s_d  = ($fcvt_s_d_active  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_h_s  = ($fcvt_h_s_active  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_h_d  = ($fcvt_h_d_active  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fsgnj_h   = ($fsgnj_h_active   && ($rm_assume0_rule)) -> ($result)
#     lemma result_fsgnjn_h  = ($fsgnjn_h_active  && ($rm_assume1_rule)) -> ($result)
#     lemma result_fsgnjx_h  = ($fsgnjx_h_active  && ($rm_assume2_rule)) -> ($result)
#     lemma result_fsgnj_s   = ($fsgnj_s_active   && ($rm_assume0_rule)) -> ($result)
#     lemma result_fsgnjn_s  = ($fsgnjn_s_active  && ($rm_assume1_rule)) -> ($result)
#     lemma result_fsgnjx_s  = ($fsgnjx_s_active  && ($rm_assume2_rule)) -> ($result)
#     lemma result_fsgnj_d   = ($fsgnj_d_active   && ($rm_assume0_rule)) -> ($result)
#     lemma result_fsgnjn_d  = ($fsgnjn_d_active  && ($rm_assume1_rule)) -> ($result)
#     lemma result_fsgnjx_d  = ($fsgnjx_d_active  && ($rm_assume2_rule)) -> ($result)
#     lemma result_fmin_h    = ($fmin_h_active    && ($rm_assume0_rule)) -> ($result)
#     lemma result_fmax_h    = ($fmax_h_active    && ($rm_assume1_rule)) -> ($result)
#     lemma result_fmin_s    = ($fmin_s_active    && ($rm_assume0_rule)) -> ($result)
#     lemma result_fmax_s    = ($fmax_s_active    && ($rm_assume1_rule)) -> ($result)
#     lemma result_fmin_d    = ($fmin_d_active    && ($rm_assume0_rule)) -> ($result)
#     lemma result_fmax_d    = ($fmax_d_active    && ($rm_assume1_rule)) -> ($result)
#
#     lemma result_feq_h     = (($feq_h_active)   && ($rm_assume2_rule))   -> ($result)
#     lemma result_flt_h     = (($flt_h_active)   && ($rm_assume1_rule))   -> ($result)
#     lemma result_fle_h     = (($fle_h_active)   && ($rm_assume0_rule))   -> ($result)
#     lemma result_feq_s     = (($feq_s_active)   && ($rm_assume2_rule))   -> ($result)
#     lemma result_flt_s     = (($flt_s_active)   && ($rm_assume1_rule))   -> ($result)
#     lemma result_fle_s     = (($fle_s_active)   && ($rm_assume0_rule))   -> ($result)
#     lemma result_feq_d     = (($feq_d_active)   && ($rm_assume2_rule))   -> ($result)
#     lemma result_flt_d     = (($flt_d_active)   && ($rm_assume1_rule))   -> ($result)
#     lemma result_fle_d     = (($fle_d_active)   && ($rm_assume0_rule))   -> ($result)

#     lemma result_fcvt_s_bf16  = ($fcvt_s_bf16_active) -> ($result)
     lemma result_fcvt_bf16_s  = ($fcvt_bf16_s_active  && ($rm_enumerate_rule)) -> ($resultNe)
#     lemma result_fcvt_bf16_s_0  = ($fcvt_bf16_s_active  && ($rm_enumerate_rule)) -> (spec.exceptions[0](1) == impl.fflags[0](9))
#     lemma result_fcvt_bf16_s_1  = ($fcvt_bf16_s_active  && ($rm_enumerate_rule)) -> (spec.exceptions[1](1) == impl.fflags[1](9))
#     lemma result_fcvt_bf16_s_2  = ($fcvt_bf16_s_active  && ($rm_enumerate_rule)) -> (spec.exceptions[2](1) == impl.fflags[2](9))
#     lemma result_fcvt_bf16_s_3  = ($fcvt_bf16_s_active  && ($rm_enumerate_rule)) -> (spec.exceptions[3](1) == impl.fflags[3](9))
#     lemma result_fcvt_bf16_s_4  = ($fcvt_bf16_s_active  && ($rm_enumerate_rule)) -> (spec.exceptions[4](1) == impl.fflags[4](9))


##f2i
#     lemma result_fcvt_w_h  = (($fcvt_w_h_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_wu_h = (($fcvt_wu_h_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_l_h  = (($fcvt_l_h_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_lu_h = (($fcvt_lu_h_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_w_s  = (($fcvt_w_s_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_wu_s = (($fcvt_wu_s_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_l_s  = (($fcvt_l_s_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_lu_s = (($fcvt_lu_s_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_w_d  = (($fcvt_w_d_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_wu_d = (($fcvt_wu_d_active)  && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_l_d  = (($fcvt_l_d_active)   && ($rm_enumerate_rule)) -> ($result)
#     lemma result_fcvt_lu_d = (($fcvt_lu_d_active)  && ($rm_enumerate_rule)) -> ($result)
#
#     lemma result_fclass_h  = (($fclass_h_active)   && ($rm_assume1_rule))   -> ($resultNe)
#     lemma result_fclass_s  = (($fclass_s_active)   && ($rm_assume1_rule))   -> ($resultNe)
#     lemma result_fclass_d  = (($fclass_d_active)   && ($rm_assume1_rule))   -> ($resultNe)
#     lemma result_fmv_x_h   = (($fmv_x_h_active)    && ($rm_assume0_rule))   -> ($resultNe)
#     lemma result_fmv_x_w   = (($fmv_x_w_active)    && ($rm_assume0_rule))   -> ($resultNe)
#     lemma result_fmv_x_d   = (($fmv_x_d_active)    && ($rm_assume0_rule))   -> ($resultNe)
     
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
