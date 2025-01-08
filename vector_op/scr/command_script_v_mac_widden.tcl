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
              ../../../common/cmodel/hector_wrapper/v_mac_widden.cpp
         
        compile_design spec
}

#========================================================
#    compile_impl
#========================================================

proc compile_impl {} {
        create_design -name impl -top bosc_VMacExu -clock clock
        #create_design -name impl -top VMacExu -clock clock

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
assume spec.Widden(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_isWidden(5)
assume spec.Mask(1)    == impl.auto_issue_in_issue_bits_uop_vctrl_vm(5)
assume spec.vstart(1)  == impl.io_vstart(5)
assume spec.vxrm(1)    == impl.io_vcsr[2:1](5)
assume spec.vma(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vma(5)
assume spec.vta(1)     == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vta(5)

assume spec.funct6(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct6(5)
assume spec.funct3(1)  == impl.auto_issue_in_issue_bits_uop_vctrl_funct3(5)

assume spec.vl(1)      == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vl(5)
assume spec.vsew(1)    == impl.auto_issue_in_issue_bits_uop_vCsrInfo_vsew(5)
assume spec.vsew(1) < 3'b100

assume impl.auto_issue_in_issue_bits_uop_ctrl_fuType(5) == 5'h10

assume -always impl.redirectIn_valid == 1'b0

assume -always impl.auto_issue_in_issue_bits_uop_uopIdx == 8'b0
assume -always impl.auto_issue_in_issue_bits_uop_uopNum == 9'b1

assume -always impl.auto_issue_in_issue_bits_uop_vctrl_eewType_1 == 3'b011

#Mapping for -------------------------------------------------------LMUL <= 1
set common_split_rule "1'b0"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h8))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))"
set common_split_rule "$common_split_rule || ((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h2))"

set sew8_split_rule  "((spec.vsew(1) == 3'b000) && (spec.vl(1) <= 8'h8))"
set sew16_split_rule "((spec.vsew(1) == 3'b001) && (spec.vl(1) <= 8'h4))"
set sew32_split_rule "((spec.vsew(1) == 3'b010) && (spec.vl(1) <= 8'h2))"

set vwmul_vv_active     "(spec.funct6(1) == 6'h3B) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b1)"
set vwmul_vx_active     "(spec.funct6(1) == 6'h3B) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"
set vwmulsu_vv_active   "(spec.funct6(1) == 6'h3A) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b1)"
set vwmulsu_vx_active   "(spec.funct6(1) == 6'h3A) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"
set vwmulu_vv_active    "(spec.funct6(1) == 6'h38) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b1)"
set vwmulu_vx_active    "(spec.funct6(1) == 6'h38) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"

set vwmacc_vv_active       "(spec.funct6(1) == 6'h3D) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b1)"
set vwmacc_vx_active       "(spec.funct6(1) == 6'h3D) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"
set vwmaccsu_vv_active     "(spec.funct6(1) == 6'h3F) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b1)"
set vwmaccsu_vx_active     "(spec.funct6(1) == 6'h3F) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"
set vwmaccu_vv_active      "(spec.funct6(1) == 6'h3C) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b1)"
set vwmaccu_vx_active      "(spec.funct6(1) == 6'h3C) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"
set vwmaccus_vx_active     "(spec.funct6(1) == 6'h3E) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b1)"

#set vmulhu_vx_active   "(spec.funct6(1) == 6'h24) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vmul_vv_active     "(spec.funct6(1) == 6'h25) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vmul_vx_active     "(spec.funct6(1) == 6'h25) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vmulhsu_vv_active  "(spec.funct6(1) == 6'h26) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vmulhsu_vx_active  "(spec.funct6(1) == 6'h26) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vmulh_vv_active    "(spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vmulh_vx_active    "(spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vsmul_vv_active    "(spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h00) && (spec.Widden(1) == 1'b0)"
#set vsmul_vx_active    "(spec.funct6(1) == 6'h27) && (spec.funct3(1) == 3'h04) && (spec.Widden(1) == 1'b0)"
#set vmadd_vv_active    "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vmadd_vx_active    "(spec.funct6(1) == 6'h29) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vnmsub_vv_active   "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vnmsub_vx_active   "(spec.funct6(1) == 6'h2B) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vmacc_vv_active    "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vmacc_vx_active    "(spec.funct6(1) == 6'h2D) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"
#set vnmsac_vv_active   "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h02) && (spec.Widden(1) == 1'b0)"
#set vnmsac_vx_active   "(spec.funct6(1) == 6'h2F) && (spec.funct3(1) == 3'h06) && (spec.Widden(1) == 1'b0)"

lemma result_vwmul_vv_sew8       = ($vwmul_vv_active      && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmul_vx_sew8       = ($vwmul_vx_active      && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmulsu_vv_sew8     = ($vwmulsu_vv_active    && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmulsu_vx_sew8     = ($vwmulsu_vx_active    && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmulu_vv_sew8      = ($vwmulu_vv_active     && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmulu_vx_sew8      = ($vwmulu_vx_active     && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))

lemma result_vwmacc_vv_sew8       = ($vwmacc_vv_active      && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmacc_vx_sew8       = ($vwmacc_vx_active      && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmaccsu_vv_sew8     = ($vwmaccsu_vv_active    && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmaccsu_vx_sew8     = ($vwmaccsu_vx_active    && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmaccu_vv_sew8      = ($vwmaccu_vv_active     && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmaccu_vx_sew8      = ($vwmaccu_vx_active     && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
lemma result_vwmaccus_vx_sew8     = ($vwmaccus_vx_active    && ($sew8_split_rule))  -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))

#lemma result_vwmul_vv_sew16      = ($vwmul_vv_active      && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmul_vx_sew16      = ($vwmul_vx_active      && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulsu_vv_sew16    = ($vwmulsu_vv_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulsu_vx_sew16    = ($vwmulsu_vx_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulu_vv_sew16     = ($vwmulu_vv_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulu_vx_sew16     = ($vwmulu_vx_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))

#lemma result_vwmul_vv_sew32      = ($vwmul_vv_active      && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmul_vx_sew32      = ($vwmul_vx_active      && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulsu_vv_sew32    = ($vwmulsu_vv_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulsu_vx_sew32    = ($vwmulsu_vx_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulu_vv_sew32     = ($vwmulu_vv_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vwmulu_vx_sew32     = ($vwmulu_vx_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))









# 2op 2 pipeline
#lemma result_vmulhu_vv    = ($vmulhu_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhu_vx    = ($vmulhu_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vv      = ($vmul_vv_active      && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vx      = ($vmul_vx_active      && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vv   = ($vmulhsu_vv_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vx   = ($vmulhsu_vx_active   && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vv     = ($vmulh_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vx     = ($vmulh_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vv     = ($vsmul_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vx     = ($vsmul_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vv     = ($vmadd_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vx     = ($vmadd_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vv    = ($vnmsub_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vx    = ($vnmsub_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vv     = ($vmacc_vv_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vx     = ($vmacc_vx_active     && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vv    = ($vnmsac_vv_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vx    = ($vnmsac_vx_active    && ($common_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma vxsat_vsmul_vv      = ($vsmul_vv_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#lemma vxsat_vsmul_vx      = ($vsmul_vx_active     && ($common_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))

#lemma result_vmulhu_vv_sew8    = ($vmulhu_vv_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhu_vx_sew8    = ($vmulhu_vx_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vv_sew8      = ($vmul_vv_active      && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vx_sew8      = ($vmul_vx_active      && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vv_sew8   = ($vmulhsu_vv_active   && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vx_sew8   = ($vmulhsu_vx_active   && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vv_sew8     = ($vmulh_vv_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vx_sew8     = ($vmulh_vx_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vv_sew8     = ($vsmul_vv_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vx_sew8     = ($vsmul_vx_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vv_sew8     = ($vmadd_vv_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vx_sew8     = ($vmadd_vx_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vv_sew8    = ($vnmsub_vv_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vx_sew8    = ($vnmsub_vx_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vv_sew8     = ($vmacc_vv_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vx_sew8     = ($vmacc_vx_active     && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vv_sew8    = ($vnmsac_vv_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vx_sew8    = ($vnmsac_vx_active    && ($sew8_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma vxsat_vsmul_vv_sew8      = ($vsmul_vv_active     && ($sew8_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#lemma vxsat_vsmul_vx_sew8      = ($vsmul_vx_active     && ($sew8_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#
#lemma result_vmulhu_vv_sew16    = ($vmulhu_vv_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhu_vx_sew16    = ($vmulhu_vx_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vv_sew16      = ($vmul_vv_active      && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vx_sew16      = ($vmul_vx_active      && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vv_sew16   = ($vmulhsu_vv_active   && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vx_sew16   = ($vmulhsu_vx_active   && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vv_sew16     = ($vmulh_vv_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vx_sew16     = ($vmulh_vx_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vv_sew16     = ($vsmul_vv_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vx_sew16     = ($vsmul_vx_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vv_sew16     = ($vmadd_vv_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vx_sew16     = ($vmadd_vx_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vv_sew16    = ($vnmsub_vv_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vx_sew16    = ($vnmsub_vx_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vv_sew16     = ($vmacc_vv_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vx_sew16     = ($vmacc_vx_active     && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vv_sew16    = ($vnmsac_vv_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vx_sew16    = ($vnmsac_vx_active    && ($sew16_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma vxsat_vsmul_vv_sew16      = ($vsmul_vv_active     && ($sew16_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#lemma vxsat_vsmul_vx_sew16      = ($vsmul_vx_active     && ($sew16_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#
#lemma result_vmulhu_vv_sew32    = ($vmulhu_vv_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhu_vx_sew32    = ($vmulhu_vx_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vv_sew32      = ($vmul_vv_active      && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vx_sew32      = ($vmul_vx_active      && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vv_sew32   = ($vmulhsu_vv_active   && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vx_sew32   = ($vmulhsu_vx_active   && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vv_sew32     = ($vmulh_vv_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vx_sew32     = ($vmulh_vx_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vv_sew32     = ($vsmul_vv_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vx_sew32     = ($vsmul_vx_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vv_sew32     = ($vmadd_vv_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vx_sew32     = ($vmadd_vx_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vv_sew32    = ($vnmsub_vv_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vx_sew32    = ($vnmsub_vx_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vv_sew32     = ($vmacc_vv_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vx_sew32     = ($vmacc_vx_active     && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vv_sew32    = ($vnmsac_vv_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vx_sew32    = ($vnmsac_vx_active    && ($sew32_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma vxsat_vsmul_vv_sew32      = ($vsmul_vv_active     && ($sew32_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#lemma vxsat_vsmul_vx_sew32      = ($vsmul_vx_active     && ($sew32_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#
#lemma result_vmulhu_vv_sew64    = ($vmulhu_vv_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhu_vx_sew64    = ($vmulhu_vx_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vv_sew64      = ($vmul_vv_active      && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmul_vx_sew64      = ($vmul_vx_active      && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vv_sew64   = ($vmulhsu_vv_active   && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulhsu_vx_sew64   = ($vmulhsu_vx_active   && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vv_sew64     = ($vmulh_vv_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmulh_vx_sew64     = ($vmulh_vx_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vv_sew64     = ($vsmul_vv_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vsmul_vx_sew64     = ($vsmul_vx_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vv_sew64     = ($vmadd_vv_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmadd_vx_sew64     = ($vmadd_vx_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vv_sew64    = ($vnmsub_vv_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsub_vx_sew64    = ($vnmsub_vx_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vv_sew64     = ($vmacc_vv_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vmacc_vx_sew64     = ($vmacc_vx_active     && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vv_sew64    = ($vnmsac_vv_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma result_vnmsac_vx_sew64    = ($vnmsac_vx_active    && ($sew64_split_rule)) -> (spec.vd(1) == impl.auto_writeback_out_bits_data(9))
#lemma vxsat_vsmul_vv_sew64      = ($vsmul_vv_active     && ($sew64_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))
#lemma vxsat_vsmul_vx_sew64      = ($vsmul_vx_active     && ($sew64_split_rule)) -> (spec.vxsat(1) == impl.auto_writeback_out_bits_vxsat(9))

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






