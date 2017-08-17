#include "interleaveModMult.hpp"

void interleaveModMult(ap_uint<NUM_BITS+1> X, ap_uint<NUM_BITS+1> Y, ap_uint<NUM_BITS> M, uintRSA_t* Pout)
{
	ap_uint<NUM_BITS+4> P=0;

	int i;
	for (i=NUM_BITS; i>=0; i--)
	{
		P = P << 1;
		if (X.test(i))
			P += Y;
		if (P>=M)
			P -= M;
		if (P>=M)
			P -= M;
	}
	*Pout = P.range(NUM_BITS-1,0);
}
