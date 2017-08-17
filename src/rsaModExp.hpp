#ifndef RSAMODEXP_HPP_
#define RSAMODEXP_HPP_

#include "globals.hpp"
#include "montMult.hpp"
#include "interleaveModMult.hpp"


void rsaModExp(uintRSA_t M, uintRSA_t e, uintRSA_t n, uintRSA_t* out);


#endif
