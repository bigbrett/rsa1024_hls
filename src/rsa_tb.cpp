#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <bitset>
#include "globals.hpp"
#include "montMult.hpp"
#include "rsaModExp.hpp"
#include "interleaveModMult.hpp"
#include "wsrsa2048.hpp"


const uintRSA_t Mbar = uintRSA_t("CAC00639454E47A7CDCA9033E4AD317E95421D6998C6DEFE79AE2246321BD1AD60CDABE20BA4154D1202EA2635E55C326F443311D267D8B69F989823676264904DBF2C73CADAC30BE1AA3964E12E61C64CBB5FDE42FE3A02F21D4C959F2209E4A2F7E5D7C3EFF321AF6A4878E0374ACF095CC07EB77C7EC3AF932C988890548F",16);
const uintRSA_t xbar0 = uintRSA_t("26B27761777AC22768965E7FEA5F5D19407D40CA901EB0DAE04B0A1D20F260656B5975CF3BD74C61CC9D04C8865B68131515C8EFF0D9B2804604E5680409DEECC21AA023464E52F285CE4C86DE9286DAD0A3AD846439C27C2B130B2E2BA3407BC17B8B45439AA16449866345885B815057C7D69B8B503DB414637DA48C140AB7",16);


int main()
{

	// Input operands
	uintRSA_t priv_exp   = uintRSA_t("47b9cfde843176b88741d68cf096952e950813151058ce46f2b048791a26e507a1095793c12bae1e09d82213ad9326928cf7c2350acb19c98f19d32d577d666cd7bb8b2b5ba629d25ccf72a5ceb8a8da038906c84dcdb1fe677dffb2c029fd8926318eede1b58272af22bda5c5232be066839398e42f5352df58848adad11a1",16);
	uintRSA_t ciphertext = uintRSA_t("9344666f787b49b8bbe2d17775a142953866f673d0c3a8a0d1fdccd0c11643633004e9d29380a91d426101888d24b3af8eff955a1d2eb5e6ec426b636c4b402a8e6e40f30e129bb6c9f79ac6f3a9a5842d8e0fa83433d6ef3527afe6ada82c68579ef274c903c63ac3196c29125f618336d7ea1410faa600dfb338fac737caf0",16);
	uintRSA_t pub_exp    = uintRSA_t("10001",16);
	uintRSA_t modulus    = uintRSA_t("d94d889e88853dd89769a18015a0a2e6bf82bf356fe14f251fb4f5e2df0d9f9a94a68a30c428b39e3362fb3779a497eceaea37100f264d7fb9fb1a97fbf621133de55fdcb9b1ad0d7a31b379216d79252f5c527b9bc63d83d4ecf4d1d45cbf843e8474babc655e9bb6799cba77a47eafa838296474afc24beb9c825b73ebf549",16);
	uintRSA_t plaintext  = uintRSA_t("48656C6C6F2C20576F726C6421",16);

	// memory for input operands
	memword_t priv_mem[NUM_MEMWORDS];
	memword_t plaintext_mem[NUM_MEMWORDS];
	memword_t publexp_mem[NUM_MEMWORDS];
	memword_t modulus_mem[NUM_MEMWORDS];
	memword_t xbar_mem[NUM_MEMWORDS];
	memword_t Mbar_mem[NUM_MEMWORDS];
	memword_t ciphertext_mem[NUM_MEMWORDS];

	// memory for result
	memword_t enc_res[NUM_MEMWORDS]=0;
	uintRSA_t xbar_dbg;

	// copy input operands into memory
	for (int i=0; i<NUM_MEMWORDS; i++) {
		plaintext_mem[i] = plaintext.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		modulus_mem[i] = modulus.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		publexp_mem[i] = pub_exp.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		ciphertext_mem[i] = ciphertext.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		xbar_mem[i] = xbar0.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		Mbar_mem[i] = Mbar.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		cout << hex << priv_exp.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i) << ", ";
	}

//	cout << "uint32_t xbar[] = {";
//	for (int i=NUM_MEMWORDS-1; i>=0; i--) {
//		cout << hex << xbar_mem[i];
//		if (i!=0) cout << ", ";
//	}
//	cout << "};" << endl;
//	cout << "uint32_t Mbar[] = {";
//	for (int i=NUM_MEMWORDS-1; i>=0; i--) {
//		cout << hex << Mbar_mem[i];
//		if (i!=0) cout << ", ";
//	}
//	cout << "};" << endl;

	// Encrypt data using DUT
	wsrsa1024(plaintext_mem, publexp_mem, modulus_mem, Mbar_mem, xbar_mem, &xbar_dbg, enc_res);

	// check results against golden truth
	if (0 != memcmp(enc_res,ciphertext_mem,sizeof(enc_res)) )
	{
		cout << "ENCRYPT Failed !!!" << endl;
		return -1;
	}

	cout << "Test Passed !!!" << endl;
	return 0;
}

