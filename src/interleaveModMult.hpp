#ifndef INTERLEAVEMODMULT_HPP_
#define INTERLEAVEMODMULT_HPP_

#include "globals.hpp"

void interleaveModMult(ap_uint<NUM_BITS+1> X, ap_uint<NUM_BITS+1> Y, ap_uint<NUM_BITS> M, uintRSA_t* out);



#endif
