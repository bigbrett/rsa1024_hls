#include "wsrsa2048.hpp"

const ap_uint<NUM_BITS+1> r = ap_uint<NUM_BITS+1>("179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216",10);
const uintRSA_t Mbar = uintRSA_t("CAC00639454E47A7CDCA9033E4AD317E95421D6998C6DEFE79AE2246321BD1AD60CDABE20BA4154D1202EA2635E55C326F443311D267D8B69F989823676264904DBF2C73CADAC30BE1AA3964E12E61C64CBB5FDE42FE3A02F21D4C959F2209E4A2F7E5D7C3EFF321AF6A4878E0374ACF095CC07EB77C7EC3AF932C988890548F",16);
const uintRSA_t xbar0 = uintRSA_t("26B27761777AC22768965E7FEA5F5D19407D40CA901EB0DAE04B0A1D20F260656B5975CF3BD74C61CC9D04C8865B68131515C8EFF0D9B2804604E5680409DEECC21AA023464E52F285CE4C86DE9286DAD0A3AD846439C27C2B130B2E2BA3407BC17B8B45439AA16449866345885B815057C7D69B8B503DB414637DA48C140AB7",16);

void wsrsa1024(
		memword_t base_mem[NUM_MEMWORDS],
		memword_t publexp_mem[NUM_MEMWORDS],
		memword_t modulus_mem[NUM_MEMWORDS],
		memword_t result_mem[NUM_MEMWORDS])
{
#pragma HLS ALLOCATION instances=montMult limit=1 function
	// initial residues
//	uintRSA_t Mbar,xbar;

	uintRSA_t base,publexp,modulus,result=0;

	// Copy in the inputs from AXI memory
	for (int i=0; i<NUM_MEMWORDS; i++)
	{
		base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = base_mem[i];
		publexp.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = publexp_mem[i];
		modulus.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = modulus_mem[i];

		if (i!=NUM_MEMWORDS-1)
		{
			base >>= MEMWORD_SIZE;
			publexp >>= MEMWORD_SIZE;
			modulus >>= MEMWORD_SIZE;
		}
	}

//	result = modulus.range(31,0) + base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE);
//	cout << hex << result << endl;
	// perform modular exponentiation
//	rsaModExp(base,publexp,modulus,&result);


	// Compute Montgomery residue of message with respect to the radix (modulo n)
//	interleaveModMult(r,ap_uint<1025>(base),modulus,&Mbar);
//    cout << hex << "Mbar = " << Mbar << endl;
	// initialize xbar to montgomery residue of 1 with respect to the radix (modulo n)
//	interleaveModMult(r,uintRSA_t(1),modulus,&xbar);
//	cout << hex << "xbar = " << xbar << endl;
	// compute montgomery modular exponentiation using square and multiply algorithm
	int i;
	uintRSA_t xbar = xbar0;
	for (i=NUM_BITS-1; i>=0; i--)
	{
		montMult(xbar,xbar,modulus,&xbar); // square
		if (publexp.test(i))
			montMult(Mbar,xbar,modulus,&xbar); // multiply
	}
	// undo montgomery transform
	montMult(xbar,1,modulus,&result);

	// copy outputs to AXI memory
	for (int i=0; i<NUM_MEMWORDS; i++)
	{
		result_mem[i] = result.range(NUM_MEMWORDS-1,0);

		if (i!=NUM_MEMWORDS-1)
			result >>= MEMWORD_SIZE;
	}

}



//void wsrsa1024( memword_t privexp[NUM_MEMWORDS],  // private exponent
//				RSAmode_t mode,
//				uintRSA_t base,     // base (plain/cipher)text
//				uintRSA_t publexp,  // public exponent
//				uintRSA_t modulus,  // shared modulus
//				memword_t result_mem[NUM_MEMWORDS],
//				ap_uint<4> *mode_out)
//{
//
//	static uintRSA_t priv;
//	uintRSA_t result=0;
//
//	switch(mode)
//	{
//	case DECRYPTKEYINIT:
//		cout << "mode = DECRYPTKEYINIT" << endl;
//		for (int i=0; i<NUM_MEMWORDS; i++)
//		{
//			priv.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = privexp[i];
//			if (i!=NUM_MEMWORDS-1)
//				priv >>= MEMWORD_SIZE;
//		}
////		*result = 0;
//		*mode_out = 2;
//		break;
//
//	case DECRYPT:
//		cout << "mode = DECRYPT" << endl;
//		rsaModExp(base,priv,modulus,&result);
//		for (int i=0; i<NUM_MEMWORDS; i++)
//		{
//			result_mem[i] = result.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE);
//			if (i!=NUM_MEMWORDS-1)
//				result >>= MEMWORD_SIZE;
//		}
//		*mode_out = 1;
//		break;
//
//	case ENCRYPT:
//	default:
//		cout << "mode = ENCRYPT" << endl;
//		rsaModExp(base,publexp,modulus,&result);
//		for (int i=0; i<NUM_MEMWORDS; i++)
//		{
//			result_mem[i] = result.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE);
//			if (i!=NUM_MEMWORDS-1)
//				result >>= MEMWORD_SIZE;
//		}
//		*mode_out = 0;
//		break;
//
//	}
//}
