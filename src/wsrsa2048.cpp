#include "wsrsa2048.hpp"

void wsrsa1024( memword_t privexp[NUM_MEMWORDS],    // BRAM holding private exponent
				RSAmode_t mode,     // mode: encrypt, decrypt, or load private exponent from BRAM
				uintRSA_t base,     // base (plain/cipher)text
				uintRSA_t publexp,  // public exponent
				uintRSA_t modulus,  // modulus
				uintRSA_t *result ) // result
{

	static uintRSA_t priv=0;

	switch(mode)
	{	
	// Load the private key (from BRAM) into local ap_uint<1024> variable.
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

	// Encrypts data using RSA modular exponentiation: result = base^(private exponent) % modulus
	// base is the plaintext, exponent is private exponent, modulus is the shared modulus
	case ENCRYPT:
		cout << "mode = ENCRYPT" << endl;
		rsaModExp(base,publexp,modulus,result);
		break;

	// Decrypts data using RSA modular exponentiation: result = base^(public exponent) % modulus
	// base is the ciphertext, exponent is private exponent, modulus is the shared modulus	case DECRYPT:
	case DECRYPT:
	default:
		cout << "mode = DECRYPT" << endl;
		rsaModExp(base,priv,modulus,result);
		break;

	}
}
