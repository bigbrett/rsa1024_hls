#ifndef WSRSA1024_HPP_
#define WSRSA1024_HPP_

#include "globals.hpp"
#include "rsaModExp.hpp"

typedef enum { ENCRYPT=0, DECRYPT=1, DECRYPTKEYINIT=2} RSAmode_t;

void wsrsa1024( memword_t privexp[NUM_MEMWORDS],  // private exponent
				RSAmode_t mode,
				uintRSA_t base,     // base (plain/cipher)text
				uintRSA_t publexp,  // public exponent
				uintRSA_t modulus,  // shared modulus
				uintRSA_t *result );

#endif
