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
              ../../../common/cmodel/hector_wrapper/v_permutation.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_Permutation -clock clock

        vcs -sverilog -f ../../../common/filelist/bosc_XSTop.f
        #vcs -sverilog /nfs/home/guanmingxing/work-nh3/share_for_formal/SimTop.sv

        compile_design impl
}

#========================================================
#    user define
#========================================================

proc ual {} {

for {set i 0} {$i <= 32} {incr i} {
   if {$i < 3} {
      assume impl.reset($i) == 1'b1
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b0
   } elseif {$i < 5} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b0
   } elseif {$i < 7} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b1
      assume impl.io_in_rvalid($i) == 1'b0
   } elseif {$i < 9} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b1
   } elseif {$i < 11} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b0
   } elseif {$i < 13} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b1
   } elseif {$i < 15} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b0
   } elseif {$i < 19} {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b1
   } else {
      assume impl.reset($i) == 1'b0
      assume impl.io_in_uop_valid($i) == 1'b0
      assume impl.io_in_rvalid($i) == 1'b0
   }
}
assume spec.vs2(1)     == impl.io_in_rdata(15)
assume spec.oldvd(1)   == impl.io_in_rdata(15)
assume spec.vm(1)      == impl.io_in_rdata(7)
assume spec.vm(1)[15:0] == 16'hffff

assume spec.Mask(1)    == impl.io_in_uop_ctrl_vm(5)
assume spec.vta(1)     == impl.io_in_uop_info_ta(5)
assume spec.vma(1)     == impl.io_in_uop_info_ma(5)
assume spec.vta(1) == 3'b1

assume spec.funct6(1)  == impl.io_in_uop_ctrl_funct6(5)
assume spec.funct3(1)  == impl.io_in_uop_ctrl_funct3(5)
assume spec.vstart(1)  == impl.io_in_uop_info_vstart(5)
assume spec.vl(1)      == impl.io_in_uop_info_vl(5)
#assume spec.vlmul(1)   == impl.io_in_uop_info_vlmul(5)
assume spec.vl(1) != 8'b0
assume spec.vl(1) != 8'h10

assume spec.vsew(1)    == impl.io_in_uop_info_vsew(5)
#assume spec.vsew(1) < 3'b100
assume spec.vsew(1) == 3'b0

assume -always impl.io_redirect_valid == 1'b0
assume -always impl.io_in_uop_info_vlmul == 3'h0
#assume -always impl.io_in_uop_sysUop_ctrl_rfWen == 1'b0
#assume -always impl.io_in_uop_sysUop_ctrl_fpWen == 1'b0
#assume -always impl.io_in_uop_sysUop_ctrl_vdWen == 1'b1

assume -always impl.io_in_mask_preg_idx == 8'h20
assume -always impl.io_in_vs1_preg_idx_0 == 8'h2b
assume -always impl.io_in_vs2_preg_idx_0 == 8'h2f
assume -always impl.io_in_old_vd_preg_idx_0 == 8'h2d

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h10))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h8))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b011) && (spec.vl(1) <= 8'h2))"

set vcompress_vm_active    "(spec.funct6(1) == 6'h17) && (spec.funct3(1) == 3'h02) && (spec.vstart(1)  == 1'b0) && (spec.Mask(1) == 1)"

lemma result_vcompress_vm    = ($vcompress_vm_active    && ($common_split_rule)) -> (spec.vd(1) == impl.io_out_wb_data(23))
lemma valid  = ($vcompress_vm_active    && ($common_split_rule)) -> (impl.io_out_wb_vld(23) == 1'b1)
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






