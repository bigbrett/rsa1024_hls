# rsa1024_hls

# Overview
- Directives are under solution1
- Src contains the HLS source code

# Source code
- globals.hpp: macros, typedefs, and other global things
- interleaveModMult.cpp/.hpp: computes modular multiplication using the interleaved binary method. 
  - Computes X*Y mod M
- montMult.cpp/hpp: computes montgomery modular multiplication.  
  - Computes X*Y * R^(-1) mod M
- rsaModExp.cpp/hpp: computes modular multiplication using the square and multiply algorithm on intermediate montgomery residues
  - Computes M^(e) mod n
- rsa_tb.cpp: self-checking testbench
