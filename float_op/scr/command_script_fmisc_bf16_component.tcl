
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
          ../../../common/cmodel/hector_wrapper/fmisc_bf16_component.cc \
          $SF_OPTIONS

    compile_design spec
}

#========================================================
#    compile_impl
#========================================================
proc compile_impl {} {
    create_design -name impl -top F32ToBF16 
    vcs -sverilog /nfs/home/panlingfeng/debug_code/bf16_component.sv
#    create_design -name impl -top FPToInt -clock clock
#    vcs -sverilog /nfs/home/panlingfeng/debug_code/f2i.sv

    compile_design impl
}

#========================================================
#    user define
#========================================================
proc ual {} {
    assume spec.rs1(1)     == impl.io_src(1)
    assume spec.rm(1)      == impl.io_rm(1)
    assume spec.rm(1) <= 3'h4

    lemma result = (spec.rd(1) == impl.io_data(1)) && (spec.exceptions(1) == impl.io_fflags(1))
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
