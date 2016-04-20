//在整数算法中| Zij | = ( | Wij | * MF+ f ) >> qbits，并且sign(Zij)=sign(Wij )
//对于帧内块 f=2^qbits/3,对于帧间块,f=2^qbits/6，其中qbits=15+floor(QP/6)


void quant4x4_c(int16_t* data, const int32_t Qp, int32_t is_intra)
{
	const int32_t qbits    = 15 + Qp / 6;
	const int32_t mf_index = Qp % 6;
	int32_t i;
	const int32_t f = (1 << qbits) / (is_intra ? 3 : 6);
	for(i = 0 ; i < 16 ; i ++)
	{
		if (data[i] > 0)  //sign(Zij)=sign(Wij )
			data[i] = (data[i] * quant[mf_index][i] + f) >> qbits;
		else
			data[i] = -((-(data[i] * quant[mf_index][i]) + f) >> qbits);
	}
}

static  int def_quant4_mf[6][4][4] =
{
	{ { 13107, 8066, 13107, 8066 }, { 8066, 5243, 8066, 5243 },
	{ 13107, 8066, 13107, 8066 }, { 8066, 5243, 8066, 5243 } },
	{ { 11916, 7490, 11916, 7490 }, { 7490, 4660, 7490, 4660 },
	{ 11916, 7490, 11916, 7490 }, { 7490, 4660, 7490, 4660 } },
	{ { 10082, 6554, 10082, 6554 }, { 6554, 4194, 6554, 4194 },
	{ 10082, 6554, 10082, 6554 }, { 6554, 4194, 6554, 4194 } },
	{ {  9362, 5825,  9362, 5825 }, { 5825, 3647, 5825, 3647 },
	{  9362, 5825,  9362, 5825 }, { 5825, 3647, 5825, 3647 } },
	{ {  8192, 5243,  8192, 5243 }, { 5243, 3355, 5243, 3355 },
	{  8192, 5243,  8192, 5243 }, { 5243, 3355, 5243, 3355 } },
	{ {  7282, 4559,  7282, 4559 }, { 4559, 2893, 4559, 2893 },
	{  7282, 4559,  7282, 4559 }, { 4559, 2893, 4559, 2893 } }
};
static int dequant_mf[6][4][4] =
{
	{ {10, 13, 10, 13}, {13, 16, 13, 16}, {10, 13, 10, 13}, {13, 16, 13, 16} },
	{ {11, 14, 11, 14}, {14, 18, 14, 18}, {11, 14, 11, 14}, {14, 18, 14, 18} },
	{ {13, 16, 13, 16}, {16, 20, 16, 20}, {13, 16, 13, 16}, {16, 20, 16, 20} },
	{ {14, 18, 14, 18}, {18, 23, 18, 23}, {14, 18, 14, 18}, {18, 23, 18, 23} },
	{ {16, 20, 16, 20}, {20, 25, 20, 25}, {16, 20, 16, 20}, {20, 25, 20, 25} },
	{ {18, 23, 18, 23}, {23, 29, 23, 29}, {18, 23, 18, 23}, {23, 29, 23, 29} }
};
#define QUANT_ONE( coef, mf ) \
{ \
	if( (coef) > 0 ) \
	(coef) = ( f + (coef) * (mf) ) >> i_qbits; \
	else \
	(coef) = - ( ( f - (coef) * (mf) ) >> i_qbits ); \
}
static void quant_4x4_core( int16_t dct[4][4], int quant_mf[4][4], int i_qbits, int f )
{
	int i;
	for( i = 0; i < 16; i++ )
		QUANT_ONE( dct[0][i], quant_mf[0][i] );
}
static void quant_4x4(int16_t dct[4][4], int quant_mf[6][4][4], int i_qscale, int b_intra )
{
	const int i_qbits = 15 + i_qscale / 6;
	const int i_mf = i_qscale % 6;
	const int f = ( 1 << i_qbits ) / ( b_intra ? 3 : 6 );
	quant_4x4_core( dct, quant_mf[i_mf], i_qbits, f );
}
#define DEQUANT_SHL( x ) \
	dct[y][x] = ( dct[y][x] * dequant_mf[i_mf][y][x] ) << i_qbits

#define DEQUANT_SHR( x ) \
	dct[y][x] = ( dct[y][x] * dequant_mf[i_mf][y][x] + f ) >> (-i_qbits)

static void dequant_4x4( int16_t dct[4][4], int dequant_mf[6][4][4], int i_qp )
{
	const int i_mf = i_qp%6;
	const int i_qbits = i_qp/6;
	//const int i_qbits = i_qp/6 - 4;
	int y;

	if( i_qbits >= 0 )
	{
		for( y = 0; y < 4; y++ )
		{
			DEQUANT_SHL( 0 );
			DEQUANT_SHL( 1 );
			DEQUANT_SHL( 2 );
			DEQUANT_SHL( 3 );
		}
	}
	else
	{
		const int f = 1 << (-i_qbits-1);
		for( y = 0; y < 4; y++ )
		{
			DEQUANT_SHR( 0 );
			DEQUANT_SHR( 1 );
			DEQUANT_SHR( 2 );
			DEQUANT_SHR( 3 );
		}
	}
}

