#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <bitset>
#include "globals.hpp"
#include "montMult.hpp"
#include "rsaModExp.hpp"
#include "interleaveModMult.hpp"
#include "wsrsa2048.hpp"


static void printReverse(uintRSA_t in)
{
	cout << "{" << hex << in.range(7,0);
	for (int i=1; i<128; i++)
	{
		unsigned char foo = in.range(8*i+7,8*i);
		printf(",0x%02X",(unsigned char)in.range(8*i+7,8*i));
	}
	cout << "};" << endl;
}


static void printWords(uintRSA_t in,bool endian)
{

	if (endian)
	{
		cout << "{";
		for (int i=0; i<32; i++)
		{
			//		cout<< hex << in.range(32*i+31,32*i) << endl;
			ap_uint<32> foo = in.range(32*i+31,32*i);
			printf("0x%02X%02X%02X%02X",(unsigned char)foo.range(7,0),
					(unsigned char)foo.range(15,8),
					(unsigned char)foo.range(23,16),
					(unsigned char)foo.range(31,24));
			if (i!= 31)
				cout << ", ";
		}
		cout << "};" << endl;
	}
	else
	{
		cout << "{";
		for (int i=0; i<32; i++)
		{
			ap_uint<32> foo = in.range(32*i+31,32*i);
			printf("0x%02X%02X%02X%02X",(unsigned char)foo.range(31,24),
					(unsigned char)foo.range(23,16),
					(unsigned char)foo.range(15,8),
					(unsigned char)foo.range(7,0));
			if (i!= 31)
				cout << ", ";
		}
		cout << "};"<<endl;
	}
}


int main()
{
	uintRSA_t returnval = 0;

#define sel 2

#if sel==0
	uintRSA_t ciphertext =    uintRSA_t("00be5416af9696937b7234421f7256f78dba8001c80a5fdecdb4ed761f2b7f955946ec920399f23ce9627f66286239d3f20e7a46df185946c6c8482e227b9ce172dd518202381706ed0f91b53c5436f233dec27e8cb46c4478f0398d2c254021a7c21596b30f77e9886e2fd2a081cadd3faf83c86bfdd6e9daad12559f8d2747",16);
	uintRSA_t pub_exp =    uintRSA_t("6f1e6ab386677cdc86a18f24f42073b328847724fbbd293eee9cdec29ac4dfe953a4256d7e6b9abee426db3b4ddc367a9fcf68ff168a7000d3a7fa8b9d9064ef4f271865045925660fab620fad0aeb58f946e33bdff6968f4c29ac62bd08cf53cb8be2116f2c339465a64fd02517f2bafca72c9f3ca5bbf96b24c1345eb936d1",16);
	uintRSA_t modulus =    uintRSA_t("b4d92132b03210f62e52129ae31ef25e03c2dd734a7235efd36bad80c28885f3a9ee1ab626c30072bb3fd9906bf89a259ffd9d5fd75f87a30d75178b9579b257b5dca13ca7546866ad9f2db0072d59335fb128b7295412dd5c43df2c4f2d2f9c1d59d2bb444e6dac1d9cef27190a97aae7030c5c004c5aea3cf99afe89b86d6d",16);
	uintRSA_t Mbar = uintRSA_t("9A9D95D8EE88E38C18FF90DCDDFA8D8B59E8E3457F635660241E4B0CB01AD15CFDB7727BE260BA7254001D0D1B0DF4335927FE9332B9409A3B3D8F6DA56DE4ED030A9DAF7364871E5E46A01E174D36BEF53BB2C823A3301027168A23E67F5ABE4F7E1C3B2D75862C822D1B26593402E8835719CA67428A1F4020F14379EBB84D",16);
	uintRSA_t xbar = uintRSA_t("4B26DECD4FCDEF09D1ADED651CE10DA1FC3D228CB58DCA102C94527F3D777A0C5611E549D93CFF8D44C0266F940765DA600262A028A0785CF28AE8746A864DA84A235EC358AB97995260D24FF8D2A6CCA04ED748D6ABED22A3BC20D3B0D2D063E2A62D44BBB19253E26310D8E6F5685518FCF3A3FFB3A515C306650176479293",16);
	int plaintext = 29;

	if (NUM_BITS != 1024)
	{
		cerr << "Incorrect bitwidth specified" << endl;
		return 1;
	}
#elif sel==1
	uintRSA_t priv_exp = uintRSA_t("10020cf19dd87c0750321f7f94b6bc04c2ad8228638fda6029620311b9dc42646e6fb56f07f12ca257383ff9f17149ea729d220cda74cd796720767e7812499f56843f23a46addd14a444f0d4c342c97f8ad89c65697f2d283616cfd271ffac7d39ac101033352d1a2686b71a6e4970c1094afacd694f64babfb440f46b33671",16);
	uintRSA_t ciphertext =    uintRSA_t("99edc729fcacffd12791c6fca9bbf0a2ef94eb30306279d1247525e5d91c99dd9ec25d8ccd5f4d32a4deb7ad86927ec4b905857705842384ebc45ac25f571e4fbdc070222e8dbf47cc471aac7d52d490016e1a97f5d9c585f8898b1655cdfa0adc322ee2851ed7146529fe74f71bb75b3469f8b5d1947b5548c352640a0a0ce0",16);
	uintRSA_t pub_exp =    uintRSA_t("10001",16);
	uintRSA_t modulus =    uintRSA_t("00f54aef851ae186b8b9e0422e94759960d3c88129392d112de4f4b6e6bca9864bd379eabc62e48f9f81432b49684806ed6ab249bd975a512cfcb32eec1a245fa232b42a382b010c855cfbdc865f44782664dffb6e4a544fe44056944d748173c1839e4db166dc96b8840056273691eb0e1a1c8f9447251e3c93f9c5443e07b761",16);
	uintRSA_t Mbar = uintRSA_t("5D1C86B1D4F24E5B418017A358DB29D8BEF600BAE0CA63524BE7E578122D7BF3C12165670A28BFAF7CFA8F8E7B4AB2FF06A838DF3E83F8CE21E5CADD3B0FE3AE91C4C902666969657A35E3C698C4A73B438996C44C9703531C59D2551F34AC41CEA7BFE8E8BDCB655743790F87ACF18EA68BE78F0F9A06FD31FF4841DBC4F25",16);
	uintRSA_t xbar = uintRSA_t("AB5107AE51E7947461FBDD16B8A669F2C377ED6C6D2EED21B0B4919435679B42C8615439D1B70607EBCD4B697B7F912954DB64268A5AED3034CD113E5DBA05DCD4BD5C7D4FEF37AA3042379A0BB87D99B200491B5ABB01BBFA96BB28B7E8C3E7C61B24E992369477BFFA9D8C96E14F1E5E3706BB8DAE1C36C063ABBC1F8489F",16);
	uintRSA_t plaintext = 29;

	if (NUM_BITS != 1024)
	{
		cerr << "Incorrect bitwidth specified" << endl;
		return 1;
	}
#elif sel==2
	uintRSA_t priv_exp = uintRSA_t("47b9cfde843176b88741d68cf096952e950813151058ce46f2b048791a26e507a1095793c12bae1e09d82213ad9326928cf7c2350acb19c98f19d32d577d666cd7bb8b2b5ba629d25ccf72a5ceb8a8da038906c84dcdb1fe677dffb2c029fd8926318eede1b58272af22bda5c5232be066839398e42f5352df58848adad11a1",16);
	uintRSA_t ciphertext =    uintRSA_t("9344666f787b49b8bbe2d17775a142953866f673d0c3a8a0d1fdccd0c11643633004e9d29380a91d426101888d24b3af8eff955a1d2eb5e6ec426b636c4b402a8e6e40f30e129bb6c9f79ac6f3a9a5842d8e0fa83433d6ef3527afe6ada82c68579ef274c903c63ac3196c29125f618336d7ea1410faa600dfb338fac737caf0",16);
	uintRSA_t pub_exp =    uintRSA_t("10001",16);
	uintRSA_t modulus =    uintRSA_t("d94d889e88853dd89769a18015a0a2e6bf82bf356fe14f251fb4f5e2df0d9f9a94a68a30c428b39e3362fb3779a497eceaea37100f264d7fb9fb1a97fbf621133de55fdcb9b1ad0d7a31b379216d79252f5c527b9bc63d83d4ecf4d1d45cbf843e8474babc655e9bb6799cba77a47eafa838296474afc24beb9c825b73ebf549",16);
	uintRSA_t plaintext = uintRSA_t("48656C6C6F2C20576F726C6421",16);
	if (NUM_BITS != 1024)
	{
		cerr << "Incorrect bitwidth specified" << endl;
		return 1;
	}
#elif sel==3
	uintRSA_t ciphertext   = 2790;
	uintRSA_t pub_exp   = 2753;
	uintRSA_t modulus   = 3233;
	uintRSA_t Mbar = 1330;
	uintRSA_t xbar = 1785;
	int plaintext = 65;

	if (NUM_BITS != 1024)
	{
		cerr << "Incorrect bitwidth specified" << endl;
		//			return 1;
	}
#elif sel == 4
	uintRSA_t ciphertext   = 1570;
	uintRSA_t pub_exp   = 1019;
	uintRSA_t modulus   = 3337;
	uintRSA_t Mbar = 1966;
	uintRSA_t xbar = 2216;
	int plaintext = 688;

	if (NUM_BITS != 1024)
	{
		cerr << "Incorrect bitwidth specified" << endl;
		return 1;
	}
#endif
//
//	cout << "uint32_t publexp_arr[]  = ";printWords(pub_exp,false);
//	cout << "uint32_t modulus_arr[] = ";printWords(modulus,false);
//	cout << "uint32_t ciphertext_golden_ans[] = ";printWords(ciphertext,false);
//	cout << "uint32_t plaintext_golden_ans[] = ";printWords(plaintext,false);
//	cout << "ENDIAN SWAP" << endl;
//	cout << "uint32_t publexp_arr[]  = ";printWords(pub_exp,true);
//	cout << "uint32_t modulus_arr[] = ";printWords(modulus,true);
//	cout << "uint32_t ciphertext_golden_ans[] = ";printWords(ciphertext,true);
//	cout << "uint32_t plaintext_golden_ans[] = ";printWords(plaintext,true);


	memword_t priv_mem[NUM_MEMWORDS];
	memword_t plaintext_mem[NUM_MEMWORDS];
	memword_t publexp_mem[NUM_MEMWORDS];
	memword_t modulus_mem[NUM_MEMWORDS];
	memword_t ciphertext_mem[NUM_MEMWORDS];

	for (int i=0; i<NUM_MEMWORDS; i++) {
		priv_mem[i] = priv_exp.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		plaintext_mem[i] = plaintext.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		modulus_mem[i] = modulus.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		publexp_mem[i] = pub_exp.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
		ciphertext_mem[i] = ciphertext.range(MEMWORD_SIZE*i+(MEMWORD_SIZE-1),MEMWORD_SIZE*i);
	}

	memword_t enc_res[NUM_MEMWORDS]=0;
	memword_t dec_res[NUM_MEMWORDS]=0;
	ap_uint<4> mode_out;
	ap_uint<5> iter;
	ap_uint<3> gf;

	RSAmode_t mode = INIT;
	wsrsa1024(priv_mem,mode,plaintext_mem,publexp_mem,modulus_mem,enc_res,    &gf, &iter, &mode_out);

	mode = ENCRYPT;
	wsrsa1024(priv_mem,mode,plaintext_mem,publexp_mem,modulus_mem,enc_res,    &gf, &iter, &mode_out);
	cout << "enc result = " << hex << enc_res << endl;
	if (0 != memcmp(enc_res,ciphertext_mem,sizeof(enc_res)) )
	{
		cout << "ENCRYPT Failed !!!" << endl;
		return -1;
	}

	mode = DECRYPTKEYINIT;
	wsrsa1024(priv_mem,mode,ciphertext_mem,publexp_mem,modulus_mem,dec_res,    &gf, &iter, &mode_out);
	mode = INIT;
	wsrsa1024(priv_mem,mode,ciphertext_mem,publexp_mem,modulus_mem,dec_res,&gf, &iter, &mode_out);

	mode = DECRYPT;
	wsrsa1024(priv_mem,mode,ciphertext_mem,publexp_mem,modulus_mem,dec_res, &gf, &iter, &mode_out);
	cout << "dec result = " << hex << dec_res << endl;
	if (0 != memcmp(dec_res,plaintext_mem,sizeof(enc_res)) )
	{
		cout << "DECRYPT Failed !!!" << endl;
		return -1;
	}


	cout << "Test Passed !!!" << endl;
	return 0;
}

