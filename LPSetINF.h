#ifndef LPSET_INTERFACE
#define LPSET_INTERFACE

// test current grey-current pair
typedef void (*FNP_TESTCUR)(double dbCurrent);
// test all grey-current pairs
typedef void (*FNP_TESTALL)(double* paGreyCurrent, 
							int nSize);
// stop test
typedef void (*FNP_STOPTEST)();
// ctrl card type: 0 - DAT2000; 1 - PCI2000; 2 - PCI3000
typedef UINT (*FNP_GETCARDTYPE)();
typedef BOOL (*LPLPSet) (double* paGreyCurrent, 
						 int nSize, 
						 FNP_TESTCUR pfnTestCur,
						 FNP_TESTALL pfnTestAll,
						 FNP_STOPTEST pfnStopTest,
						 FNP_GETCARDTYPE pfnGetCardType,
						 LPVOID lpReserved
						 );
typedef void (*LPInitLP)( double* paGreyCurrent, int nSize );
#endif