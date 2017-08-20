#include "wsrsa2048.hpp"

void wsrsa1024( memword_t privexp[NUM_MEMWORDS],  // private exponent
				RSAmode_t mode,
				uintRSA_t base,     // base (plain/cipher)text
				uintRSA_t publexp,  // public exponent
				uintRSA_t modulus,  // shared modulus
				uintRSA_t *result )
{

	static uintRSA_t priv=0;

	switch(mode)
	{
	case DECRYPTKEYINIT:
		cout << "mode = DECRYPTKEYINIT" << endl;
		for (int i=0; i<NUM_MEMWORDS; i++)
		{
			priv.range(NUM_BITS-1,(NUM_BITS)-MEMWORD_SIZE) = privexp[i];
			if (i!=NUM_MEMWORDS-1)
				priv >>= MEMWORD_SIZE;
		}
		*result = 0;
		break;

	case ENCRYPT:
		cout << "mode = ENCRYPT" << endl;
		rsaModExp(base,publexp,modulus,result);
		break;

	case DECRYPT:
	default:
		cout << "mode = DECRYPT" << endl;
		rsaModExp(base,priv,modulus,result);
		break;

	}
}
