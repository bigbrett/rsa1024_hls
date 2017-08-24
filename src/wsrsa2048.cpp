#include "wsrsa2048.hpp"

void wsrsa1024( memword_t privexp[NUM_MEMWORDS],  // private exponent
				RSAmode_t mode,
				memword_t base_mem[NUM_MEMWORDS],
				memword_t publexp_mem[NUM_MEMWORDS],
				memword_t modulus_mem[NUM_MEMWORDS],
				memword_t result_mem[NUM_MEMWORDS],
				ap_uint<4> *mode_out)
{

	static uintRSA_t priv;
	static uintRSA_t base,publexp,modulus,result;
//	const memword_t golden[3*NUM_MEMWORDS] = {}

	switch(mode)
	{
	case DECRYPTKEYINIT:
		*mode_out = 2;
		cout << "mode = DECRYPTKEYINIT" << endl;
		for (int i=0; i<NUM_MEMWORDS; i++)
		{
			priv.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = privexp[i];
			if (i!=NUM_MEMWORDS-1)
				priv >>= MEMWORD_SIZE;
		}
//		*result = 0;
		break;

	case INIT:
		*mode_out = 3;
		for (int i=0; i<NUM_MEMWORDS; i++)
		{
			base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = base_mem[i];
			publexp.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = publexp_mem[i];
			modulus.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = modulus_mem[i];
			result_mem[i] = 0;
			if (i!=NUM_MEMWORDS-1)
			{
				base >>= MEMWORD_SIZE;
				publexp >>= MEMWORD_SIZE;
				modulus >>= MEMWORD_SIZE;
			}
		}
		break;


	case DECRYPT:
		*mode_out = 1;
		cout << "mode = DECRYPT" << endl;
		rsaModExp(base,priv,modulus,&result);
		for (int i=0; i<NUM_MEMWORDS; i++)
		{
			result_mem[i] = result.range(NUM_MEMWORDS-1,0);
			if (i!=NUM_MEMWORDS-1)
				result >>= MEMWORD_SIZE;
		}
		break;

	case ENCRYPT:
	default:
		*mode_out = 0;
		cout << "mode = ENCRYPT" << endl;
		rsaModExp(base,publexp,modulus,&result);
		for (int i=0; i<NUM_MEMWORDS; i++)
		{
			result_mem[i] = result.range(NUM_MEMWORDS-1,0);
			if (i!=NUM_MEMWORDS-1)
				result >>= MEMWORD_SIZE;
		}
		break;

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
