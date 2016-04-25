#include "pre.h"

double **para = NULL;				// 估计的预测参数
double **pre = NULL;				// 预测后的图像数据

para = (double **)calloc(3,sizeof(double *));
for(int i = 0; i < 3; i++)
	para[i] = (double *)calloc(1,sizeof(double));


DCT = (double **)calloc(BLOCKHEIGHT - 4, sizeof(double *));
for(int i = 0; i < height; i++)
	DCT[i] = (double *)calloc(BLOCKWIDTH - 4,sizeof(double));


TDCT = (double **)calloc(BLOCKHEIGHT - 4, sizeof(double *));
for(int i = 0; i < height; i++)
	TDCT[i] = (double *)calloc(BLOCKWIDTH - 4,sizeof(double));

ODST = (double **)calloc(BLOCKHEIGHT - 4, sizeof(double *));
for(int i = 0; i < height; i++)
	ODST[i] = (double *)calloc(BLOCKWIDTH - 4,sizeof(double));

f  = (double **)calloc(BLOCKHEIGHT - 4, sizeof(double *));
for(int i = 0; i < height; i++)
	f[i] = (double *)calloc(BLOCKWIDTH - 4,sizeof(double));

F  = (double **)calloc(BLOCKHEIGHT - 4, sizeof(double *));
for(int i = 0; i < height; i++)
	F[i] = (double *)calloc(BLOCKWIDTH - 4,sizeof(double));

temp  = (double **)calloc(BLOCKHEIGHT - 4, sizeof(double *));
for(int i = 0; i < height; i++)
	temp[i] = (double *)calloc(BLOCKWIDTH - 4,sizeof(double));


void DCT_Quanter(int16_t inputData[4][4], int16_t outputData[4][4]);

//#define FDEC_STRIDE 32
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
const int FDEC_STRIDE = 9 * 8;	// 8x8 block

// estimate prediction param
// img:		input img value
// return:	 estimated prediction param value
void estimate(short **img, double **para, int width, int height);

// ?????
// mat transposition
// A:		input of 1d array
// m:		rows
// n:		cols
// return:	output
double  *MatrixInver(double   *A,int   m,int   n);

// ?????
// calculate determinant of a mat
// A;		input of 1d array
// m:		rows
// n:		cols;
double  Surplus(double   A[],int   m,int   n);

//inline void PREDICT_4x4_DC(uint8_t *src, short v)    //本函数对4x4块的每行像素赋同样的值
//{
//	*(uint32_t *) &src[0 * FDEC_STRIDE] = //第一句既表明p 指向的是32 位无符号整型变量，也表明其指向的是src（像素起始位置）正因为是32 位所以才有后面一次能给4 个像素赋值
//	*(uint32_t *) &src[1 * FDEC_STRIDE] =
//	*(uint32_t *) &src[2 * FDEC_STRIDE] =
//	*(uint32_t *) &src[3 * FDEC_STRIDE] = v;
//}
//inline void PREDICT_4x4_LOAD_LEFT(uint32_t *src)   //该定义表示依次取出当前宏块左边的4个像素
//{
//	const int l0 = src[-1 + 0 * FDEC_STRIDE];
//	const int l1 = src[-1 + 1 * FDEC_STRIDE];
//	const int l2 = src[-1 + 2 * FDEC_STRIDE];
//	UNUSED const int l3 = src[-1 + 3 * FDEC_STRIDE];
//}
//
//inline void PREDICT_4x4_LOAD_TOP(uint32_t *src)    //该定义表示依次取出当前宏块上边的4个像素
//{
//	const int t0 = src[-1 + 0 * FDEC_STRIDE];
//	const int t1 = src[-1 + 1 * FDEC_STRIDE];
//	const int t2 = src[-1 + 2 * FDEC_STRIDE];
//	UNUSED const int t3 = src[-1 + 3 * FDEC_STRIDE];
//}
//inline void PREDICT_4x4_LOAD_TOP_RIGHT(uint32_t *src)    //该定义表示依次取出当前宏块右上方的4个像素(E/F/G/H)
//{
//	const int t4 = src[4 - 1 * FDEC_STRIDE];
//	const int t5 = src[5 - 1 * FDEC_STRIDE];
//	const int t6 = src[6 - 1 * FDEC_STRIDE];
//	UNUSED const int t7 = src[7 - 1 * FDEC_STRIDE];
//}

// 本函数对4x4块的每行像素赋同样的值128
static void predict_4x4_dc_128(uint8_t *src)
{
	PREDICT_4x4_DC(0x80808080);
}

// 用宏块左方四个像素（I、J、K、L）的均值对所有像素覆盖
static void predict_4x4_dc_left(uint8_t *src)
{
	uint32_t dc = ((src[-1 + 0 * FDEC_STRIDE] + src[-1 + 1 * FDEC_STRIDE] +
	src[-1 + 2 * FDEC_STRIDE] + src[-1 + 3 * FDEC_STRIDE] + 2) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// 用宏块上方四个像素（A、B、C、D）的均值对所有像素覆盖
static void predict_4x4_dc_top(uint8_t *src)
{
	uint32_t dc = ((src[0 - FDEC_STRIDE] + src[1 - FDEC_STRIDE] +
		src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 2) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// 用宏块左边和上方八个像素（I、J、K、L、A、B、C、D）的均值对所有像素覆盖
static void predict_4x4_dc(uint8_t *src)
{
	uint32_t dc = (( src[-1 + 0 * FDEC_STRIDE] + src[-1 + 1 * FDEC_STRIDE] +
		src[-1 + 2 * FDEC_STRIDE] + src[-1 + 3 * FDEC_STRIDE] +
		src[0 - FDEC_STRIDE] + src[1 - FDEC_STRIDE] +
		src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 4 ) >> 3) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// 本函数用每行左边的像素对行进行覆盖（一行的所有像素值相同）
static void predict_4x4_h(uint8_t *src)    
{
	*(uint32_t *) &src[0 * FDEC_STRIDE] = src[0 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[1 * FDEC_STRIDE] = src[1 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[2 * FDEC_STRIDE] = src[2 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[3 * FDEC_STRIDE] = src[3 * FDEC_STRIDE] * 0x01010101;
}

// 本函数用每列上边的像素对列进行覆盖
static void predict_4x4_v(uint8_t *src)    
{
	uint32_t top = *((uint32_t *) &src[-FDEC_STRIDE]);//取出当前块上方4个像素点的值
	PREDICT_4x4_DC(top);                                 //每行均用取出的值进行覆盖
}

// 45度右上至左下覆盖预测 t0代表A  t1代表B t2代表C t3代表D
static void predict_4x4_ddl(uint8_t *src) 
{
	PREDICT_4x4_LOAD_TOP;
	PREDICT_4x4_LOAD_TOP_RIGHT
	src[0 * FDEC_STRIDE + 0] = (t0 + 2 * t1 + t2 + 2) >> 2;  //(A + 2B + C + 2)/4  +2表示四舍五入

	src[0 * FDEC_STRIDE + 1] = src[1 * FDEC_STRIDE + 0] = (t1 + 2 * t2 + t3 + 2) >> 2;
    src[0*FDEC_STRIDE+2] =
	src[1*FDEC_STRIDE+1] =
	src[2*FDEC_STRIDE+0] = ( t2 + 2*t3 + t4 + 2 ) >> 2;      //-------c=f=i=（C+2D+E+2）/4

	src[0*FDEC_STRIDE+3] =
	src[1*FDEC_STRIDE+2] =
	src[2*FDEC_STRIDE+1] =
	src[3*FDEC_STRIDE+0] = ( t3 + 2*t4 + t5 + 2 ) >> 2;  //-------d=g=j=m=（D+2E+F+2）/4

	src[1*FDEC_STRIDE+3] =
	src[2*FDEC_STRIDE+2] =
	src[3*FDEC_STRIDE+1] = ( t4 + 2*t5 + t6 + 2 ) >> 2;   //------h=k=n=（E+2F+G+2）/4

	src[2*FDEC_STRIDE+3] =
	src[3*FDEC_STRIDE+2] = ( t5 + 2*t6 + t7 + 2 ) >> 2;   //------i=o=（F+2G+H+2）/4

	src[3*FDEC_STRIDE+3] = ( t6 + 3*t7 + 2 ) >> 2;        //-----------p=（G+3H+2）/4
}

// 45°左上至右下覆盖预测
static void predict_4x4_ddr(uint8_t *src)     
{
	const int lt = src[-1-FDEC_STRIDE];     // -----lt=M--------------I、J、K、L---------------A、B、C、D
	PREDICT_4x4_LOAD_LEFT
	PREDICT_4x4_LOAD_TOP           
	src[0*FDEC_STRIDE+0] =
	src[1*FDEC_STRIDE+1] =
	src[2*FDEC_STRIDE+2] =
	src[3*FDEC_STRIDE+3] = ( t0 + 2 * lt + l0 + 2 ) >> 2;//a=f=k=p=（A+2M+I+2）/4

	src[0*FDEC_STRIDE+1] =
	src[1*FDEC_STRIDE+2] =
	src[2*FDEC_STRIDE+3] = ( lt + 2 * t0 + t1 + 2 ) >> 2;//b=g=l=（M+2A+B+2）/4

	src[0*FDEC_STRIDE+2] =
	src[1*FDEC_STRIDE+3] = ( t0 + 2 * t1 + t2 + 2 ) >> 2;//c=h=（A+2B+C+2）/4

	src[0*FDEC_STRIDE+3] = ( t1 + 2 * t2 + t3 + 2 ) >> 2;//d=（B+2C+D+2）/4

	src[1*FDEC_STRIDE+0] =
	src[2*FDEC_STRIDE+1] =
	src[3*FDEC_STRIDE+2] = ( lt + 2 * l0 + l1 + 2 ) >> 2;//e=j=o=（M+2I+J+2）/4

	src[2*FDEC_STRIDE+0] =
	src[3*FDEC_STRIDE+1] = ( l0 + 2 * l1 + l2 + 2 ) >> 2;//i=n=（I+2J+K+2）/4

	src[3*FDEC_STRIDE+0] = ( l1 + 2 * l2 + l3 + 2 ) >> 2;//m=（J+2K+L+2）/4
}

//与 y夹角 26.6°左上至右下覆盖预测 （没用到 L）
static void predict_4x4_vr(uint8_t *src)     
{
	const int lt = src[-1-FDEC_STRIDE];//-----M
	PREDICT_4x4_LOAD_LEFT              //-------------I、J、K、L
	PREDICT_4x4_LOAD_TOP               //--------------A、B、C、D
	src[0*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+1]= ( lt + t0 + 1 ) >> 1; //---a=j=（M+A+1）/2

	src[0*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+2]= ( t0 + t1 + 1 ) >> 1; //---e=k=（A+B+1）/2

	src[0*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+3]= ( t1 + t2 + 1 ) >> 1;  //---c=i=（B+C+1）/2

	src[0*FDEC_STRIDE+3]= ( t2 + t3 + 1 ) >> 1;  //---d=（C+D+1）/2
	src[1*FDEC_STRIDE+0]=
	src[3*FDEC_STRIDE+1]= ( l0 + 2 * lt + t0 + 2 ) >> 2; //---e=n=（I+2M+A+2）/4

	src[1*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+2]= ( lt + 2 * t0 + t1 + 2 ) >> 2; //---f=o=（M+2A+B+2）/4

	src[1*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+3]= ( t0 + 2 * t1 + t2 + 2) >> 2; //---g=p=（A+2B+C+2）/4

	src[1*FDEC_STRIDE+3]= ( t1 + 2 * t2 + t3 + 2 ) >> 2; //---h=（B+2C+D+2）/4

	src[2*FDEC_STRIDE+0]= ( lt + 2 * l0 + l1 + 2 ) >> 2; //---i=（M+2I+J+2）/4

	src[3*FDEC_STRIDE+0]= ( l0 + 2 * l1 + l2 + 2 ) >> 2; //---m=（I+2J+K+2）/4
}

 //与 x夹角 26.6°左上至右下覆盖预测（没用到D）
static void predict_4x4_hd(uint8_t *src)  
{
	const int lt= src[-1-1*FDEC_STRIDE];   //-----M
	PREDICT_4x4_LOAD_LEFT                   //-------------I、J、K、L
	PREDICT_4x4_LOAD_TOP                   //--------------A、B、C、D
	src[0*FDEC_STRIDE+0]=
	src[1*FDEC_STRIDE+2]= ( lt + l0 + 1 ) >> 1; //---a=g=（M+I+1）/2

	src[0*FDEC_STRIDE+1]=
	src[1*FDEC_STRIDE+3]= ( l0 + 2 * lt + t0 + 2 ) >> 2;  //---b=h=（I+2M+A+2）/4

	src[0*FDEC_STRIDE+2]= ( lt + 2 * t0 + t1 + 2 ) >> 2;  //---c=（M+2A+B+2）/4

	src[0*FDEC_STRIDE+3]= ( t0 + 2 * t1 + t2 + 2 ) >> 2;  //---d=（A+2B+C+2）/4

	src[1*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+2]= ( l0 + l1 + 1 ) >> 1;           //---e=k=（I+J+1）/2

	src[1*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+3]= ( lt + 2 * l0 + l1 + 2 ) >> 2;  //---f=l=（M+2I+J+2）/4

	src[2*FDEC_STRIDE+0]=
	src[3*FDEC_STRIDE+2]= ( l1 + l2+ 1 ) >> 1;            //---i=o=（J+K+1）/2

	src[2*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+3]= ( l0 + 2 * l1 + l2 + 2 ) >> 2;  //---j=p=（I+2J+K+2）/4
	src[3*FDEC_STRIDE+0]= ( l2 + l3 + 1 ) >> 1;            //---m=（K+L+1）/2
	src[3*FDEC_STRIDE+1]= ( l1 + 2 * l2 + l3 + 2 ) >> 2;   //---n=（J+2K+L+2）/4
}

// 与y夹角 26.6°右上至左下覆盖预测（没用到H）
static void predict_4x4_vl(uint8_t *src)     
{
	PREDICT_4x4_LOAD_TOP                 //--------------A、B、C、D
	PREDICT_4x4_LOAD_TOP_RIGHT          //----E、F、G、H
	src[0*FDEC_STRIDE+0]= ( t0 + t1 + 1 ) >> 1;           //---a=（A+B+1）/2

	src[0*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+0]= ( t1 + t2 + 1 ) >> 1;           //---b=i=（B+C+1）/2

	src[0*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+1]= ( t2 + t3 + 1 ) >> 1;            //---c=j=（C+D+1）/2

	src[0*FDEC_STRIDE+3]=
	src[2*FDEC_STRIDE+2]= ( t3 + t4 + 1 ) >> 1;           //---d=k=（D+E+1）/2

	src[2*FDEC_STRIDE+3]= ( t4 + t5 + 1 ) >> 1;           //---l=（E+F+1）/2

	src[1*FDEC_STRIDE+0]= ( t0 + 2 * t1 + t2 + 2 ) >> 2;  //---e=（A+2B+C+2）/4

	src[1*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+0]= ( t1 + 2 * t2 + t3 + 2 ) >> 2;  //---f=m=（B+2C+D+2）/4

	src[1*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+1]= ( t2 + 2 * t3 + t4 + 2 ) >> 2;  //---g=n=（C+2D+E+2）/4

	src[1*FDEC_STRIDE+3]=
	src[3*FDEC_STRIDE+2]= ( t3 + 2 * t4 + t5 + 2 ) >> 2;  //---h=o=（D+2E+F+2）/4

	src[3*FDEC_STRIDE+3]= ( t4 + 2 * t5 + t6 + 2 ) >> 2;  //---p=（E+2F+G+2）/4
}

//与 x夹角 26.6°左下至右上覆盖预测
static void predict_4x4_hu(uint8_t *src)    
{
	PREDICT_4x4_LOAD_LEFT                       //-------------I、J、K、L
	src[0*FDEC_STRIDE+0]= ( l0 + l1 + 1 ) >> 1; //-a=（I+J+1）/2
	src[0*FDEC_STRIDE+1]= ( l0 + 2 * l1 + l2 + 2 ) >> 2;  //---b=（I+2J+K+2）/4

	src[0*FDEC_STRIDE+2]=
	src[1*FDEC_STRIDE+0]= ( l1 + l2 + 1 ) >> 1;    //---c=e=（J+K+1）/2

	src[0*FDEC_STRIDE+3]=
	src[1*FDEC_STRIDE+1]= ( l1 + 2*l2 + l3 + 2 ) >> 2;  //---d=f=（J+2K+L+2）/4

	src[1*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+0]= ( l2 + l3 + 1 ) >> 1;       //---g=i=（K+L+1）/2

	src[1*FDEC_STRIDE+3]=
	src[2*FDEC_STRIDE+1]= ( l2 + 2 * l3 + l3 + 2 ) >> 2;  //---h=j=（K+3L+2）/4

	src[2*FDEC_STRIDE+3]=
	src[3*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+3]= l3;                              //---k=l=m=n=o=p=L
}

int CalcBestResi(int16_t block_resi[4][4], int i_row, int i_col)
{
	unsigned int R[11];
	for (int i = 0; i < 10; ++i)
		R[i] = 0;

	uint8_t resi_temp[4][4];
	memset(temp, 0, 4 * 4);
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_left_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[0] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_top_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[1] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[2] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - h_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[3] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - v_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[4] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddl_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[5] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddr_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[6] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vr_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[7] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hd_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[8] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vl_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[9] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hu_image[i_row][i_col][i + 1][j + 1];//预测残差
			R[10] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	int modeMinRes = 0;
	int minRes = R[modeMinRes];
	for (int i = 0; i < 11; ++i)
	{
		if (R[i] < R[modeMinRes])
		{
			modeMinRes = i;
			minRes = R[i];
		}
	}
	switch (modeMinRes)
	{
	case 0:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_left_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 1:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_top_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 2:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 3:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - h_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 4:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - v_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 5:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddl_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 6:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddr_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 7:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vr_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 8:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hd_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 9:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vl_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}
		break;

	case 10:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hu_image[i_row][i_col][i + 1][j + 1];//预测残差
			}
		}\
		break;
	default:
		break;
	}
	return minRes;

}

// 对各个图像块按12种预测模式进行预测，选出最好的一种得到残差,返回全图的残差能量
short predict_LS(uint8_t image_construct[1028][1028], short resi[1024][1024], int height, int width)
{
	// 图像残差块、变换量化后的图像块、的内存空间分配
	int16_t block_resi[4][4] = {0};
	int16_t outdataDct_Quant[4][4] = {0};
	int16_t block_construct[4][4] = {0};
	int16_t resi_energy = 0;
	

	for (int i_row = 0; i_row < ROWS; ++i_row)
	{
		for (int i_col = 0; i_col < COLS; ++i_col)
		{
			// 将输入图像读成 5x9 的图像块（边界扩展后），赋值给用来存储每一种预测模式输入图像的数组
			// 
			for (int i = 0; i < BLOCKHEIGHT + 4; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH + 4; ++j)
				{
					smallimage[i_row][i_col][i][j] = image_construct[i_row * BLOCKHEIGHT + i] [i_col * BLOCKWIDTH + j];
					dc_left_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					dc_top_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					dc_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					h_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					v_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					ddl_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					ddr_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					vr_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					hd_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					vl_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					hu_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
				}
			}
			// 分别按照11种模式对图像块进行预测
			predict_4x4_dc_left((uint8_t *)(&dc_left_image[i_row][i_col][1][1]));
			predict_4x4_dc_top((uint8_t *)(&dc_top_image[i_row][i_col][1][1]));
			predict_4x4_dc((uint8_t *)(&dc_image[i_row][i_col][1][1]));
			predict_4x4_h((uint8_t *)(&h_image[i_row][i_col][1][1]));
			predict_4x4_v((uint8_t *)(&v_image[i_row][i_col][1][1]));
			predict_4x4_ddl((uint8_t *)(&ddl_image[i_row][i_col][1][1]));
			predict_4x4_ddr((uint8_t *)(&ddr_image[i_row][i_col][1][1]));
			predict_4x4_vr((uint8_t *)(&vr_image[i_row][i_col][1][1]));
			predict_4x4_hd((uint8_t *)(&hd_image[i_row][i_col][1][1]));
			predict_4x4_hl((uint8_t *)(&hl_image[i_row][i_col][1][1]));
			predict_4x4_hu((uint8_t *)(&hu_image[i_row][i_col][1][1]));

			//计算对每一种预测模式的预测残差，并比较残差能量，将能量最低的预测模式下的残差保留,返回值为残差能量
			CalcBestResi(block_resi, i_row, i_col);
			//将当前块的残差值保存到整幅图像的残差矩阵中
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	严重错误！！！！
					resi[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_resi[i_row][i_col][i][j];
				}
			}
			// 对得到的最优的残差块进行变换、量化、逆量化、逆变换，
			resi_energy += DCT_Quanter(block_resi, outdataDct_Quant);
			//DCT_Quanter后的结果与预测值进行相加得到重建块，重建块的值拷贝到image_construct中当前块对应位置处，覆盖掉原始图像，
			//为下一次循环读取下一个块的邻近重建像素值做准备
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//block_construct[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j] + outdataDct_Quant[i_row][i_col][i][j];
					//image_construct[i_row][i_col][i + 1][j + 1] = block_construct[i_row][i_col][i][j]; 严重错误！！中间状态不需要索引块坐标，只需要它的值
					block_construct[i][j] = block_resi[i][j] + outdataDct_Quant[i][j];
					image_construct[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_construct[i][j];

				}
			}
		}
	}
	return resi_energy;
}

// multiply two mat
// D:		input 1st mat, H1 x K1
// f:		input 2st mat, K1 x W1
// temp:	otuput mat, H1 x W1
void Mult(double **mat1, double **mat2,double **resMat, int H1, int W1, int K1)	//DCT正变换  H1 * K1 | K1 * W1
{
	double sum = 0;
	for(int i = 0; i < H1; i++)
		for(int j = 0; j < W1; j++)
		{
			sum = 0;
			for(int k = 0;k < K1; k++)
				sum += mat1[i][k] * mat2[k][j];
			resMat[i][j] = sum;
		}
}

// TO-DO: 修改边界扩展至4行4列，模式3与4采用不同的估计方式
// img_padding 左边和上边填128，row和col分别从1开始索引img_padding
//int mode_cal_u(double **img_padding, int row, int col, int mode)
//{
//	static double u[2][2];
//	switch (mode)
//	{
//	case 0, 1, 2:
//		u[0][1] = img_padding[row - 1][col];	// 上边
//		u[1][0] = img_padding[row][col - 1];	// 左边
//		u[0][0] = img_padding[row - 1][col - 1];// 左上角
//		break;
//	case 3:
//		u[0][1] = img_padding[row - 1][col];	// 上边
//		u[1][0] = (img_padding[row][col - 1] + 2 * img_padding[row + 1][col - 1] + img_padding[row + 2][col - 1]) / 4;
//		u[0][0] = img_padding[row][col - 1];// 左上角
//		break;
//	case 4:
//		u[0][1] = img_padding[row - 1][col];	// 上边
//		u[1][0] = (img_padding[row - 2][col - 1] + 2 * img_padding[row - 1][col - 1] + img_padding[row][col - 1]) / 4;
//		u[0][0] = img_padding[row - 2][col - 1];// 左上角
//		break;	
//	case 5:
//		u[0][1] = img_padding[row - 1][col];	// 上边
//		u[1][0] = (img_padding[row - 1][col - 1] + img_padding[row][col - 1]) / 2;
//		u[0][0] = (img_padding[row - 2][col - 1] + img_padding[row - 1][col - 1]) / 2;
//		break;	
//	case 6:
//		u[0][1] = (img_padding[row - 1][col] + img_padding[row - 1][col - 1]) / 2;
//		u[1][0] = img_padding[row][col - 1];
//		u[0][0] = (img_padding[row - 1][col - 1] + img_padding[row - 1][col - 2]) / 2;
//		break;	
//	case 7:
//		u[0][1] = img_padding[row - 1][col];	// 上边
//		u[1][0] = (img_padding[row][col - 1] + img_padding[row + 1][col - 1]) / 2;
//		u[0][0] = (img_padding[row - 1][col - 1] + img_padding[row][col - 1]) / 2;
//		break;	
//	case 8:
//		u[0][1] = (img_padding[row - 1][col + 1] + img_padding[row - 1][col]) / 2;
//		u[1][0] = img_padding[row][col - 1];
//		u[0][0] = (img_padding[row - 1][col] + img_padding[row - 1][col - 1]) / 2;
//
//		break;	
//	default:
//		break;
//	}
//	
//}


void AccumulateParaAB(double **paraA, double **paraB, short **matrix_in_offset, int x, int y, int mode)
{   //需要调整多个块累加的程序。。。。??
	static double u[2][2];	// u[y][x] equals to u(x, y)
	switch (mode)
	{
	case 0: case 1: case 2:
		u[1][0] = matrix_in_offset[y][x - 1];
		u[0][1] = matrix_in_offset[y - 1][x];
		u[0][0] = matrix_in_offset[y - 1][x - 1];
		break;
	case 3:
		u[1][0] = matrix_in_offset[y][x - 1];
		u[0][1] = (matrix_in_offset[y - 1][x] + 2 * matrix_in_offset[y - 1][x + 1] + matrix_in_offset[y - 1][x + 2]) / 4;
		u[0][0] = matrix_in_offset[y - 1][x];
		break;
	case 4:
		u[1][0] = matrix_in_offset[y][x - 1];
		u[0][1] = (matrix_in_offset[y - 1][x - 2] + 2 * matrix_in_offset[y - 1][x - 1] + matrix_in_offset[y - 1][x]) / 4;
		u[0][0] = matrix_in_offset[y - 1][x - 2];
		break;	
	case 5:
		u[1][0] = matrix_in_offset[y][x - 1];
		u[0][1] = (matrix_in_offset[y - 1][x - 1] + matrix_in_offset[y - 1][x]) / 2;
		u[0][0] = (matrix_in_offset[y - 1][x - 2] + matrix_in_offset[y - 1][x - 1]) / 2;
		break;	
	case 6:
		u[1][0] = (matrix_in_offset[y][x - 1] + matrix_in_offset[y - 1][x - 1]) / 2;
		u[0][1] = matrix_in_offset[y - 1][x];
		u[0][0] = (matrix_in_offset[y - 1][x - 1] + matrix_in_offset[y - 2][x - 1]) / 2;
		break;	
	case 7:
		u[1][0] = matrix_in_offset[y][x - 1];
		u[0][1] = (matrix_in_offset[y - 1][x] + matrix_in_offset[y - 1][x + 1]) / 2;
		u[0][0] = (matrix_in_offset[y - 1][x - 1] + matrix_in_offset[y - 1][x]) / 2;
		break;	
	case 8:
		u[1][0] = (matrix_in_offset[y + 1][x - 1] + matrix_in_offset[y][x - 1]) / 2;
		u[0][1] = matrix_in_offset[y - 1][x];
		u[0][0] = (matrix_in_offset[y][x - 1] + matrix_in_offset[y - 1][x - 1]) / 2;
		break;	
	default:
		break;
	}
	u[1][1] = matrix_in_offset[y][x];

	paraA[0][0] += u[1][0] * u[1][0];
	paraA[0][1] += u[1][0] * u[0][0];
	paraA[0][2] += u[1][0] * u[0][1];
	paraA[1][0] += u[0][0] * u[1][0];
	paraA[1][1] += u[0][0] * u[0][0];
	paraA[1][2] += u[0][0] * u[0][1];
	paraA[2][0] += u[0][1] * u[1][0];
	paraA[2][1] += u[0][1] * u[0][0];
	paraA[2][2] += u[0][1] * u[0][1];

	paraB[0][0] += u[1][1] * u[1][0];
	paraB[1][0] += u[1][1] * u[0][0];
	paraB[2][0] += u[1][1] * u[0][1];
}


/*矩阵转置*/ 
double *MatrixInver(double A[], int m, int n)   
{ 
	double *B = (double *)malloc(m * n * sizeof(double)); 

	for(int i = 0;i <n;i++) 
		for(int j = 0;j < m;j++) 
			B[i * m + j] = A[j * n + i];         
	return B; 
} 
/*求矩阵行列式*/
double Surplus(double A[],int m,int n)   
{         
	int   i,j,k,p,r; 
	double   X,temp=1,temp1=1,s=0,s1=0; 

	if(n==2) 
	{ 
		for(i=0;i <m;i++) 
			for(j=0;j <n;j++) 
				if((i+j)%2)   temp1*=A[i*n+j]; 
				else   temp*=A[i*n+j]; 
				X=temp-temp1; 
	} 
	else 
	{ 
		for(k=0;k <n;k++) 
		{ 
			for(i=0,j=k;i <m,j <n;i++,j++) 
				temp*=A[i*n+j]; 
			if(m-i) 
			{ 
				for(p=m-i,r=m-1;p> 0;p--,r--) 
					temp*=A[r*n+p-1]; 
			} 
			s+=temp; 
			temp=1; 
		} 

		for(k=n-1;k >= 0;k--) 
		{ 
			for(i=0,j=k;i <m,j >= 0;i++,j--) 
				temp1*=A[i*n+j]; 
			if(m-i) 
			{for(p=m-1,r=i;r < m;p--,r++) 
			temp1*=A[r*n+p];} 
			s1+=temp1; 
			temp1=1; 
		}                 
		X=s-s1; 
	} 
	return   X; 
} 


// TO-DO verify whether is correct
/*矩阵求逆****A为一维数组*/ 
double *MatrixOpp(double A[],int   m,int   n)  
{ 
	double *SP = NULL;
	double *AB = NULL;
	double *B = NULL;
	double X;
	double *C = NULL; 
	SP=(double *)malloc(m * n * sizeof(double)); 
	AB=(double *)malloc(m * n * sizeof(double)); 
	B = (double *)malloc(m * n * sizeof(double)); 

	X = Surplus(A,m,n); 
	X = 1.00 / X; 

	for(int i = 0;i < m;i++) 
		for(int j = 0;j < n;j++) 
		{ 
			for(int k = 0;k < m * n; k++) 
				B[k]=A[k]; 

			{ 
				for(int x=0;x <n;x++) 
					B[i*n+x]=0; 
				for(int y=0;y <m;y++) 
					B[m*y+j]=0; 
				B[i*n+j]=1; 
				SP[i*n+j]=Surplus(B,m,n); 
				AB[i*n+j]=X*SP[i*n+j]; 
			} 
		} 
		C=MatrixInver(AB, m, n); 

		return   C; 
} 

void estimate(short **img, double **para, int width, int height)
{
	double **paraA = NULL;
	double **paraB = NULL;
	double **result = NULL;
	
	paraA  = (double **)calloc(3, sizeof(double *));
	for(int i = 0; i < 3; i++)
	{
		paraA[i] = (double *)calloc(3,sizeof(double));
	}

	result  = (double **)calloc(3, sizeof(double *));
	for(int i = 0; i < 3; i++)
	{
		result[i] = (double *)calloc(3,sizeof(double));
	}

	paraB  = (double **)calloc(3, sizeof(double *));
	for(int i = 0; i < 3; i++)
	{
		paraB[i] = (double *)calloc(1,sizeof(double));
	}

	para = (double **)calloc(3,sizeof(double *));
	for(int i = 0; i < 3; i++)
	{
		para[i] = (double *)calloc(1,sizeof(double));
	}
	
	// 扩充原始块，用128填充上下各四行，左右各四列
	/*short **matrix_in = NULL;
	matrix_in = (short **)calloc(height + 4,sizeof(short *));
	for(int i = 0; i < height + 4; i++)
		matrix_in[i] = (short *)calloc(width + 4,sizeof(short));
	memset(matrix_in, 128, sizeof(short));
	short **matrix_in_offset = (short **)&matrix_in[2][2];
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			matrix_in_offset[i][j] = img[i][j];*/

	 

	// TO-DO: for_each mode = 0~8;
	int mode = 0;
	//判断模式，根据不同模式给左边，左上，上边元素进行赋值，再调用下面的计算公式
	switch (mode)
	{
	case 0: case 1: case 4: case 5: case 6:
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 0; i < height; ++i)
			for(int j = 0; j < width; ++j)
				AccumulateParaAB(paraA, paraB, matrix_in_offset, j, i, mode);
		break;
	case 3: case 7:
		//3,7模式可以用下面公式计算
		for(int i = height - 1; i >= 0; --i)
			for(int j = width - 1; j >= 0; --j)
				AccumulateParaAB(paraA, paraB, matrix_in_offset, j, i, mode);
		break;
	case 8:
		//8模式可以用下面公式计算
		for(int i = height - 1; i >= 0 ; --i)
			for(int j = 0; j < width; ++j)
				AccumulateParaAB(paraA, paraB, matrix_in_offset, j, i, mode);
		break;
	default:
		break;
	}

	for (int i = 0; i < height + 4; ++i)
		free(matrix_in[i]);
	free(matrix_in);


	// 矩阵求逆l
	double *res_temp = MatrixOpp((double *)paraA, 3, 3); 
	
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			result[i][j] = res_temp[i * 3 + j];
	//????
	free(res_temp);
	{
		FILE *fout = fopen("res.txt", "w");
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
				fprintf(fout, "%6.2f, ", result[i][j]);
			fprintf(fout, "\n");
		}
		fclose(fout);
	}

	//矩阵result 与paraB相乘
	Mult(result, paraB, para, 3, 1, 3);
	for (int i = 0; i < 3; ++i)
	{
		free(result[i]);
		free(paraA[i]);
		free(paraB[i]);
	}
	free(result);
	free(paraA);
	free(paraB);


	
}

// use estimated prediction param to predict
// img:		input img value
// pre:		output of prediction value
// resi:	output of residual
void  predict(short **img, double **pre, double **resi, int height, int width)
{
	short  N, W, NW;
	//     short NE, SW;
	short  **imgConstructed = NULL;
	double paramter_4x4_ML[4][9] = {
		{0.42, 0.88, 0.57, 0.31, 0.25, 0.08, 0.71, 0.24, 0.33},
		{-0.38, -0.42, -0.36, -0.22, 0.50, 0.23, 0.11, 0.27, -0.35},
		{0.94, 0.51, 0.70, 0.24, 0.36, 0.74, 0.27, 0.74, 0.48},
		{0, 0, 0, 0.60, 0, 0, 0,0.24, 0.46}
	};
	double paramter_4x4_MSE[4][9] = {
		{0.51, 0.96, 0.65, 0.26, 0.20, -0.09, 0.64, 0.30, 0.31},
		{-0.49, -0.40, -0.41, -0.10, 0.58, 0.46, 0.33, -0.26, -0.25},
		{0.97, 0.44, 0.63, 0.05, 0.25, 0.63, 0.07, 0.66, 0.38},
		{0, 0, 0, 0.76, 0, 0, 0, 0.29, 0.53}
	};

	

	for(int i = 1; i < height + 1; i++)
	{
		for(int j = 1; j < width + 1; j++)
		{
			N = tb[i-1][j];
			W = tb[i][j-1];
			NW = tb[i-1][j-1];
			// NE = tb[i+1][j-1];
			// SW = tb[i-1][j+1];
			// pre[i-1][j-1] = intra_est(N, W, NW);//利用左上，左面，上面的像素进行帧内预测
			/*switch (mode){
			//case 0: 
			//		short pre = paramter_4x4_MSE[0][0] * W + paramter_4x4_MSE[1][0] * NW + paramter_4x4_MSE[2][0] * N;
			//		break;
			//case 1: 
			//		short pre = paramter_4x4_MSE[0][1] * W + paramter_4x4_MSE[1][1] * NW + paramter_4x4_MSE[2][1] * N;
			//		break;
			//case 2: 
			//		short pre = paramter_4x4_MSE[0][2] * W + paramter_4x4_MSE[1][2] * NW + paramter_4x4_MSE[2][2] * N;
			//		break;
			//case 3:  
			//		NE = tb[i+1][j-1];
			//		short pre = paramter_4x4_MSE[0][3] * W + paramter_4x4_MSE[1][3] * NW + paramter_4x4_MSE[2][3] * N + paramter_4x4_MSE[3][3] * NE;
			//		break;
			//case 4:
			//		short pre = paramter_4x4_MSE[0][4] * W + paramter_4x4_MSE[1][4] * NW + paramter_4x4_MSE[2][4] * N;
			//		break;
			//case 5:
			//		short pre = paramter_4x4_MSE[0][5] * W + paramter_4x4_MSE[1][5] * NW + paramter_4x4_MSE[2][5] * N;
			//		break;
			//case 6:
			//		short pre = paramter_4x4_MSE[0][6] * W + paramter_4x4_MSE[1][6] * NW + paramter_4x4_MSE[2][6] * N;
			//		break;
			//case 7:
			//		NE = tb[i+1][j-1];
			//		short pre = paramter_4x4_MSE[0][7] * W + paramter_4x4_MSE[1][7] * NW + paramter_4x4_MSE[2][7] * N + paramter_4x4_MSE[3][7] * NE;
			//		break;
			//			  
			//case 8: 
			//		SW = tb[i-1][j+1];
			//		short pre = paramter_4x4_MSE[0][8] * W + paramter_4x4_MSE[1][8] * NW + paramter_4x4_MSE[2][8] * N + paramter_4x4_MSE[3][8] * SW;
			//		break;
			//default:
			//		printf("please input proper mode \n");
			//		break;
			//}
			*/
			// mode 0
			pre[i-1][j-1] = paramter_4x4_MSE[0][0] * W + paramter_4x4_MSE[1][0] * NW + paramter_4x4_MSE[2][0] * N;

			// mode 1
			//short pre = paramter_4x4_MSE[0][1] * W + paramter_4x4_MSE[1][1] * NW + paramter_4x4_MSE[2][1] * N;

			// mode 2
			//short pre = paramter_4x4_MSE[0][2] * W + paramter_4x4_MSE[1][2] * NW + paramter_4x4_MSE[2][2] * N;
			//
			// mode 3
			//NE = tb[i+1][j-1];
			//short pre = paramter_4x4_MSE[0][3] * W + paramter_4x4_MSE[1][3] * NW + paramter_4x4_MSE[2][3] * N + paramter_4x4_MSE[3][3] * NE;
			//
			// mode 4
			//short pre = paramter_4x4_MSE[0][4] * W + paramter_4x4_MSE[1][4] * NW + paramter_4x4_MSE[2][4] * N;
			//
			// mode 5
			//short pre = paramter_4x4_MSE[0][5] * W + paramter_4x4_MSE[1][5] * NW + paramter_4x4_MSE[2][5] * N;
			//
			// mode 6
			//short pre = paramter_4x4_MSE[0][6] * W + paramter_4x4_MSE[1][6] * NW + paramter_4x4_MSE[2][6] * N;
			//
			// mode 7
			//NE = tb[i+1][j-1];
			//short pre = paramter_4x4_MSE[0][7] * W + paramter_4x4_MSE[1][7] * NW + paramter_4x4_MSE[2][7] * N + paramter_4x4_MSE[3][7] * NE;

			// mode 8
			//SW = tb[i-1][j+1];
			//short pre = paramter_4x4_MSE[0][8] * W + paramter_4x4_MSE[1][8] * NW + paramter_4x4_MSE[2][8] * N + paramter_4x4_MSE[3][8] * SW;


			resi[i-1][j-1] = tb[i][j] - pre[i-1][j-1];//预测残差
		}
	}
}

/**************************************************************
	 * 1. estimate prediction param of 9 modes for block of 4 x 4 and 4 modes for block of 16 x 16
	 *    by Markov prediction method
	 *************************************************************/
	//估计用于预测的邻近像素权值
	//estimate(img, para, width, height);

	/**************************************************************
	 * 2. use estimated prediction param to predict
	 *************************************************************/
	//predict(img, pre, resi, height, width);
