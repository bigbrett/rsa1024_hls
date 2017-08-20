#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdint.h>
#include <cstdlib>

#include "hls_stream.h"
#include "ap_utils.h"

#define AP_INT_MAX_W 4096 // must be defined before ap_int.h include
#include "ap_int.h"

using namespace hls;
using namespace std;

#define NUM_BITS 1024
#define MEMWORD_SIZE 32
#define NUM_MEMWORDS 32

// arbitrary precision for RSA
typedef ap_uint<NUM_BITS> uintRSA_t; // 1024 bit unsigned integer
typedef ap_uint<MEMWORD_SIZE> memword_t; // 32 bits





#endif // GLOBALS_HPP_
