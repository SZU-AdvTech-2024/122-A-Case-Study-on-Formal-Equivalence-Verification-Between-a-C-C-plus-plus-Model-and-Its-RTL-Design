
source -v -c -e ../../scr/command_script_${CMODEL_NAME}.tcl
#========================================================
#    DPV EXECUATION
#========================================================

make

if {$HDPS_USED == "on"} {
   run_hdps
}

run_main

#view_trace -property check

exit


