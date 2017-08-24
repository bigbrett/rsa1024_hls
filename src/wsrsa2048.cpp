#include "wsrsa2048.hpp"

void wsrsa1024( memword_t privexp[NUM_MEMWORDS],  // private exponent
				RSAmode_t mode,
				memword_t base_mem[NUM_MEMWORDS],
				memword_t publexp_mem[NUM_MEMWORDS],
				memword_t modulus_mem[NUM_MEMWORDS],
				memword_t result_mem[NUM_MEMWORDS],
                ap_uint<3> *golden_flag,
                ap_uint<5> *iter,
				ap_uint<4> *mode_out)
{
    // DEBUG
    ap_uint<3> gf = 0;
    *iter = 0;
	static uintRSA_t priv;
	static uintRSA_t base,publexp,modulus,result;
	const memword_t golden[3*NUM_MEMWORDS] = {0x726C6421, 0x2C20576F, 0x656C6C6F, 0x00000048, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, // END BASE
                                              0x00010001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                              0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, // END PUBEXP
                                              0x73EBF549, 0xEB9C825B, 0x74AFC24B, 0xA8382964, 0x77A47EAF, 0xB6799CBA, 0xBC655E9B, 0x3E8474BA,
                                              0xD45CBF84, 0xD4ECF4D1, 0x9BC63D83, 0x2F5C527B, 0x216D7925, 0x7A31B379, 0xB9B1AD0D, 0x3DE55FDC,
                                              0xFBF62113, 0xB9FB1A97, 0x0F264D7F, 0xEAEA3710, 0x79A497EC, 0x3362FB37, 0xC428B39E, 0x94A68A30,
                                              0xDF0D9F9A, 0x1FB4F5E2, 0x6FE14F25, 0xBF82BF35, 0x15A0A2E6, 0x9769A180, 0x88853DD8, 0xD94D889E};// END MODULUS



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
            *iter = i;
			base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = base_mem[i];
            publexp.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = publexp_mem[i];
			modulus.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = modulus_mem[i];

            // Check each value against golden by anding it with complement
			unsigned int foo = memword_t(base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE)) & memword_t(~golden[i]); cout << foo;
			gf[0] = (memword_t(base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE)) & memword_t(~golden[i])) ? 1 : 0;       cout << hex << "base["<<i<<"]="<<base.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) << "  gold="<<golden[i]<<endl;
			gf[1] = (publexp.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) & ~golden[i+32]) ? 1 : 0; cout << hex << "expo["<<i<<"]="<<publexp.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE)<<"  gold="<<golden[i+32]<<endl;;
			gf[2] = (modulus.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) & ~golden[i+64]) ? 1 : 0; cout << hex << "modu["<<i<<"]="<<modulus.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE)<<"  gold="<<golden[i+64]<<endl;;
			if (gf > 0)
			{
				cout << "FUCK********************************" << endl;
			}
			*golden_flag = gf;

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
