############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project testrsa
set_top wsrsa1024
add_files testrsa/src/globals.hpp
add_files testrsa/src/interleaveModMult.cpp
add_files testrsa/src/interleaveModMult.hpp
add_files testrsa/src/montMult.cpp
add_files testrsa/src/montMult.hpp
add_files testrsa/src/rsaModExp.cpp
add_files testrsa/src/rsaModExp.hpp
add_files testrsa/src/wsrsa2048.cpp
add_files testrsa/src/wsrsa2048.hpp
add_files -tb testrsa/src/rsa_tb.cpp
open_solution "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
source "./testrsa/solution1/directives.tcl"
csim_design -clean -compiler gcc
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog
