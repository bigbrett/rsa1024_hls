#include "montMult.hpp"
#define PRINT_ON false

//void montMult(uint1024_t X, uint1024_t Y, uint1024_t M, uint1024_t* outData){
void montMult(uintRSA_t X0, uintRSA_t Y0, uintRSA_t M0, uintRSA_t* outData)
{
#pragma HLS ALLOCATION instances=mul limit=256 operation

	/* IMPT BUGFIX: INTERMEDIATE WIDTHS MUST BE GREATER
	 * THAN OR EQUAL TO NUM_BITS+1 OR OVERFLOW CAUSES
	 * THE INTEGER LIBRARY TO SCREW UP SIGN EXTENTIONS */
	ap_uint<NUM_BITS+2> S = 0;
	ap_uint<NUM_BITS+2> X = X0, Y = Y0, M=M0;

	int i;
	for (i=0; i<NUM_BITS; i++)
	{
		//if(PRINT_ON){cout << "i = "<<dec<<i<<endl;}
		if (X.test(i)) {
			S += Y;       //if(PRINT_ON){cout<<" S+=("<<X[i]<<")*Y-->"<<endl;cout<<"    "<<hex<<S<<endl;}
		}
		if (S.test(0)) {
			S += M;       //if(PRINT_ON){cout<<" S += M-->"<<endl<<"    "<<hex<<S<<endl;}
		}
		S = S >> 1;       //if(PRINT_ON){cout<<" S >> 1-->"<<endl<<"    "<<hex<<S<<endl<<endl;}
	}

	//if(PRINT_ON){cout << "LOOP END" << endl << " S = " << S << endl;}
	if (S >= M)
	{
		S -= M;
		//if(PRINT_ON){cout << "***SUBTRACTED MODULUS" << endl << "FINAL MONTMULT = " << hex << S << endl;}
	}
	*outData = S.range(NUM_BITS-1,0);
}

