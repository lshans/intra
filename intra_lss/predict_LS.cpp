#include "pre.h"
void DCT_Quanter(int16_t inputData[4][4], int16_t outputData[4][4]);

//#define FDEC_STRIDE 32
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
const int FDEC_STRIDE = 9 * 8;	// 8x8 block

//inline void PREDICT_4x4_DC(uint8_t *src, short v)    //��������4x4���ÿ�����ظ�ͬ����ֵ
//{
//	*(uint32_t *) &src[0 * FDEC_STRIDE] = //��һ��ȱ���p ָ�����32 λ�޷������ͱ�����Ҳ������ָ�����src��������ʼλ�ã�����Ϊ��32 λ���Բ��к���һ���ܸ�4 �����ظ�ֵ
//	*(uint32_t *) &src[1 * FDEC_STRIDE] =
//	*(uint32_t *) &src[2 * FDEC_STRIDE] =
//	*(uint32_t *) &src[3 * FDEC_STRIDE] = v;
//}
//inline void PREDICT_4x4_LOAD_LEFT(uint32_t *src)   //�ö����ʾ����ȡ����ǰ�����ߵ�4������
//{
//	const int l0 = src[-1 + 0 * FDEC_STRIDE];
//	const int l1 = src[-1 + 1 * FDEC_STRIDE];
//	const int l2 = src[-1 + 2 * FDEC_STRIDE];
//	UNUSED const int l3 = src[-1 + 3 * FDEC_STRIDE];
//}
//
//inline void PREDICT_4x4_LOAD_TOP(uint32_t *src)    //�ö����ʾ����ȡ����ǰ����ϱߵ�4������
//{
//	const int t0 = src[-1 + 0 * FDEC_STRIDE];
//	const int t1 = src[-1 + 1 * FDEC_STRIDE];
//	const int t2 = src[-1 + 2 * FDEC_STRIDE];
//	UNUSED const int t3 = src[-1 + 3 * FDEC_STRIDE];
//}
//inline void PREDICT_4x4_LOAD_TOP_RIGHT(uint32_t *src)    //�ö����ʾ����ȡ����ǰ������Ϸ���4������(E/F/G/H)
//{
//	const int t4 = src[4 - 1 * FDEC_STRIDE];
//	const int t5 = src[5 - 1 * FDEC_STRIDE];
//	const int t6 = src[6 - 1 * FDEC_STRIDE];
//	UNUSED const int t7 = src[7 - 1 * FDEC_STRIDE];
//}

// ��������4x4���ÿ�����ظ�ͬ����ֵ128
static void predict_4x4_dc_128(uint8_t *src)
{
	PREDICT_4x4_DC(0x80808080);
}

// �ú�����ĸ����أ�I��J��K��L���ľ�ֵ���������ظ���
static void predict_4x4_dc_left(uint8_t *src)
{
	uint32_t dc = ((src[-1 + 0 * FDEC_STRIDE] + src[-1 + 1 * FDEC_STRIDE] +
	src[-1 + 2 * FDEC_STRIDE] + src[-1 + 3 * FDEC_STRIDE] + 2) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// �ú���Ϸ��ĸ����أ�A��B��C��D���ľ�ֵ���������ظ���
static void predict_4x4_dc_top(uint8_t *src)
{
	uint32_t dc = ((src[0 - FDEC_STRIDE] + src[1 - FDEC_STRIDE] +
		src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 2) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// �ú����ߺ��Ϸ��˸����أ�I��J��K��L��A��B��C��D���ľ�ֵ���������ظ���
static void predict_4x4_dc(uint8_t *src)
{
	uint32_t dc = (( src[-1 + 0 * FDEC_STRIDE] + src[-1 + 1 * FDEC_STRIDE] +
		src[-1 + 2 * FDEC_STRIDE] + src[-1 + 3 * FDEC_STRIDE] +
		src[0 - FDEC_STRIDE] + src[1 - FDEC_STRIDE] +
		src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 4 ) >> 3) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// ��������ÿ����ߵ����ض��н��и��ǣ�һ�е���������ֵ��ͬ��
static void predict_4x4_h(uint8_t *src)    
{
	*(uint32_t *) &src[0 * FDEC_STRIDE] = src[0 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[1 * FDEC_STRIDE] = src[1 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[2 * FDEC_STRIDE] = src[2 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[3 * FDEC_STRIDE] = src[3 * FDEC_STRIDE] * 0x01010101;
}

// ��������ÿ���ϱߵ����ض��н��и���
static void predict_4x4_v(uint8_t *src)    
{
	uint32_t top = *((uint32_t *) &src[-FDEC_STRIDE]);//ȡ����ǰ���Ϸ�4�����ص��ֵ
	PREDICT_4x4_DC(top);                                 //ÿ�о���ȡ����ֵ���и���
}

// 45�����������¸���Ԥ�� t0����A  t1����B t2����C t3����D
static void predict_4x4_ddl(uint8_t *src) 
{
	PREDICT_4x4_LOAD_TOP;
	PREDICT_4x4_LOAD_TOP_RIGHT
	src[0 * FDEC_STRIDE + 0] = (t0 + 2 * t1 + t2 + 2) >> 2;  //(A + 2B + C + 2)/4  +2��ʾ��������

	src[0 * FDEC_STRIDE + 1] = src[1 * FDEC_STRIDE + 0] = (t1 + 2 * t2 + t3 + 2) >> 2;
    src[0*FDEC_STRIDE+2] =
	src[1*FDEC_STRIDE+1] =
	src[2*FDEC_STRIDE+0] = ( t2 + 2*t3 + t4 + 2 ) >> 2;      //-------c=f=i=��C+2D+E+2��/4

	src[0*FDEC_STRIDE+3] =
	src[1*FDEC_STRIDE+2] =
	src[2*FDEC_STRIDE+1] =
	src[3*FDEC_STRIDE+0] = ( t3 + 2*t4 + t5 + 2 ) >> 2;  //-------d=g=j=m=��D+2E+F+2��/4

	src[1*FDEC_STRIDE+3] =
	src[2*FDEC_STRIDE+2] =
	src[3*FDEC_STRIDE+1] = ( t4 + 2*t5 + t6 + 2 ) >> 2;   //------h=k=n=��E+2F+G+2��/4

	src[2*FDEC_STRIDE+3] =
	src[3*FDEC_STRIDE+2] = ( t5 + 2*t6 + t7 + 2 ) >> 2;   //------i=o=��F+2G+H+2��/4

	src[3*FDEC_STRIDE+3] = ( t6 + 3*t7 + 2 ) >> 2;        //-----------p=��G+3H+2��/4
}

// 45�����������¸���Ԥ��
static void predict_4x4_ddr(uint8_t *src)     
{
	const int lt = src[-1-FDEC_STRIDE];     // -----lt=M--------------I��J��K��L---------------A��B��C��D
	PREDICT_4x4_LOAD_LEFT
	PREDICT_4x4_LOAD_TOP           
	src[0*FDEC_STRIDE+0] =
	src[1*FDEC_STRIDE+1] =
	src[2*FDEC_STRIDE+2] =
	src[3*FDEC_STRIDE+3] = ( t0 + 2 * lt + l0 + 2 ) >> 2;//a=f=k=p=��A+2M+I+2��/4

	src[0*FDEC_STRIDE+1] =
	src[1*FDEC_STRIDE+2] =
	src[2*FDEC_STRIDE+3] = ( lt + 2 * t0 + t1 + 2 ) >> 2;//b=g=l=��M+2A+B+2��/4

	src[0*FDEC_STRIDE+2] =
	src[1*FDEC_STRIDE+3] = ( t0 + 2 * t1 + t2 + 2 ) >> 2;//c=h=��A+2B+C+2��/4

	src[0*FDEC_STRIDE+3] = ( t1 + 2 * t2 + t3 + 2 ) >> 2;//d=��B+2C+D+2��/4

	src[1*FDEC_STRIDE+0] =
	src[2*FDEC_STRIDE+1] =
	src[3*FDEC_STRIDE+2] = ( lt + 2 * l0 + l1 + 2 ) >> 2;//e=j=o=��M+2I+J+2��/4

	src[2*FDEC_STRIDE+0] =
	src[3*FDEC_STRIDE+1] = ( l0 + 2 * l1 + l2 + 2 ) >> 2;//i=n=��I+2J+K+2��/4

	src[3*FDEC_STRIDE+0] = ( l1 + 2 * l2 + l3 + 2 ) >> 2;//m=��J+2K+L+2��/4
}

//�� y�н� 26.6�����������¸���Ԥ�� ��û�õ� L��
static void predict_4x4_vr(uint8_t *src)     
{
	const int lt = src[-1-FDEC_STRIDE];//-----M
	PREDICT_4x4_LOAD_LEFT              //-------------I��J��K��L
	PREDICT_4x4_LOAD_TOP               //--------------A��B��C��D
	src[0*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+1]= ( lt + t0 + 1 ) >> 1; //---a=j=��M+A+1��/2

	src[0*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+2]= ( t0 + t1 + 1 ) >> 1; //---e=k=��A+B+1��/2

	src[0*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+3]= ( t1 + t2 + 1 ) >> 1;  //---c=i=��B+C+1��/2

	src[0*FDEC_STRIDE+3]= ( t2 + t3 + 1 ) >> 1;  //---d=��C+D+1��/2
	src[1*FDEC_STRIDE+0]=
	src[3*FDEC_STRIDE+1]= ( l0 + 2 * lt + t0 + 2 ) >> 2; //---e=n=��I+2M+A+2��/4

	src[1*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+2]= ( lt + 2 * t0 + t1 + 2 ) >> 2; //---f=o=��M+2A+B+2��/4

	src[1*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+3]= ( t0 + 2 * t1 + t2 + 2) >> 2; //---g=p=��A+2B+C+2��/4

	src[1*FDEC_STRIDE+3]= ( t1 + 2 * t2 + t3 + 2 ) >> 2; //---h=��B+2C+D+2��/4

	src[2*FDEC_STRIDE+0]= ( lt + 2 * l0 + l1 + 2 ) >> 2; //---i=��M+2I+J+2��/4

	src[3*FDEC_STRIDE+0]= ( l0 + 2 * l1 + l2 + 2 ) >> 2; //---m=��I+2J+K+2��/4
}

 //�� x�н� 26.6�����������¸���Ԥ�⣨û�õ�D��
static void predict_4x4_hd(uint8_t *src)  
{
	const int lt= src[-1-1*FDEC_STRIDE];   //-----M
	PREDICT_4x4_LOAD_LEFT                   //-------------I��J��K��L
	PREDICT_4x4_LOAD_TOP                   //--------------A��B��C��D
	src[0*FDEC_STRIDE+0]=
	src[1*FDEC_STRIDE+2]= ( lt + l0 + 1 ) >> 1; //---a=g=��M+I+1��/2

	src[0*FDEC_STRIDE+1]=
	src[1*FDEC_STRIDE+3]= ( l0 + 2 * lt + t0 + 2 ) >> 2;  //---b=h=��I+2M+A+2��/4

	src[0*FDEC_STRIDE+2]= ( lt + 2 * t0 + t1 + 2 ) >> 2;  //---c=��M+2A+B+2��/4

	src[0*FDEC_STRIDE+3]= ( t0 + 2 * t1 + t2 + 2 ) >> 2;  //---d=��A+2B+C+2��/4

	src[1*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+2]= ( l0 + l1 + 1 ) >> 1;           //---e=k=��I+J+1��/2

	src[1*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+3]= ( lt + 2 * l0 + l1 + 2 ) >> 2;  //---f=l=��M+2I+J+2��/4

	src[2*FDEC_STRIDE+0]=
	src[3*FDEC_STRIDE+2]= ( l1 + l2+ 1 ) >> 1;            //---i=o=��J+K+1��/2

	src[2*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+3]= ( l0 + 2 * l1 + l2 + 2 ) >> 2;  //---j=p=��I+2J+K+2��/4
	src[3*FDEC_STRIDE+0]= ( l2 + l3 + 1 ) >> 1;            //---m=��K+L+1��/2
	src[3*FDEC_STRIDE+1]= ( l1 + 2 * l2 + l3 + 2 ) >> 2;   //---n=��J+2K+L+2��/4
}

// ��y�н� 26.6�����������¸���Ԥ�⣨û�õ�H��
static void predict_4x4_vl(uint8_t *src)     
{
	PREDICT_4x4_LOAD_TOP                 //--------------A��B��C��D
	PREDICT_4x4_LOAD_TOP_RIGHT          //----E��F��G��H
	src[0*FDEC_STRIDE+0]= ( t0 + t1 + 1 ) >> 1;           //---a=��A+B+1��/2

	src[0*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+0]= ( t1 + t2 + 1 ) >> 1;           //---b=i=��B+C+1��/2

	src[0*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+1]= ( t2 + t3 + 1 ) >> 1;            //---c=j=��C+D+1��/2

	src[0*FDEC_STRIDE+3]=
	src[2*FDEC_STRIDE+2]= ( t3 + t4 + 1 ) >> 1;           //---d=k=��D+E+1��/2

	src[2*FDEC_STRIDE+3]= ( t4 + t5 + 1 ) >> 1;           //---l=��E+F+1��/2

	src[1*FDEC_STRIDE+0]= ( t0 + 2 * t1 + t2 + 2 ) >> 2;  //---e=��A+2B+C+2��/4

	src[1*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+0]= ( t1 + 2 * t2 + t3 + 2 ) >> 2;  //---f=m=��B+2C+D+2��/4

	src[1*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+1]= ( t2 + 2 * t3 + t4 + 2 ) >> 2;  //---g=n=��C+2D+E+2��/4

	src[1*FDEC_STRIDE+3]=
	src[3*FDEC_STRIDE+2]= ( t3 + 2 * t4 + t5 + 2 ) >> 2;  //---h=o=��D+2E+F+2��/4

	src[3*FDEC_STRIDE+3]= ( t4 + 2 * t5 + t6 + 2 ) >> 2;  //---p=��E+2F+G+2��/4
}

//�� x�н� 26.6�����������ϸ���Ԥ��
static void predict_4x4_hu(uint8_t *src)    
{
	PREDICT_4x4_LOAD_LEFT                       //-------------I��J��K��L
	src[0*FDEC_STRIDE+0]= ( l0 + l1 + 1 ) >> 1; //-a=��I+J+1��/2
	src[0*FDEC_STRIDE+1]= ( l0 + 2 * l1 + l2 + 2 ) >> 2;  //---b=��I+2J+K+2��/4

	src[0*FDEC_STRIDE+2]=
	src[1*FDEC_STRIDE+0]= ( l1 + l2 + 1 ) >> 1;    //---c=e=��J+K+1��/2

	src[0*FDEC_STRIDE+3]=
	src[1*FDEC_STRIDE+1]= ( l1 + 2*l2 + l3 + 2 ) >> 2;  //---d=f=��J+2K+L+2��/4

	src[1*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+0]= ( l2 + l3 + 1 ) >> 1;       //---g=i=��K+L+1��/2

	src[1*FDEC_STRIDE+3]=
	src[2*FDEC_STRIDE+1]= ( l2 + 2 * l3 + l3 + 2 ) >> 2;  //---h=j=��K+3L+2��/4

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
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_left_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[0] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_top_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[1] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[2] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - h_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[3] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - v_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[4] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddl_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[5] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddr_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[6] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vr_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[7] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hd_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[8] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vl_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			R[9] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hu_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
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
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_left_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 1:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_top_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 2:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - dc_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 3:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - h_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 4:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - v_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 5:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddl_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 6:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - ddr_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 7:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vr_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 8:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hd_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 9:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - vl_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}
		break;

	case 10:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_resi[i][j] = smallimage[i_row][i_col][i + 1][j + 1] - hu_image[i_row][i_col][i + 1][j + 1];//Ԥ��в�
			}
		}\
		break;
	default:
		break;
	}
	return minRes;

}

// �Ը���ͼ��鰴12��Ԥ��ģʽ����Ԥ�⣬ѡ����õ�һ�ֵõ��в�,����ȫͼ�Ĳв�����
short predict(uint8_t image_construct[1025][1029], short resi[1024][1024], int height, int width)
{
	// ͼ��в�顢�任�������ͼ��顢���ڴ�ռ����
	int16_t block_resi[4][4] = {0};
	int16_t outdataDct_Quant[4][4] = {0};
	int16_t block_construct[4][4] = {0};
	int16_t resi_energy = 0;
	

	for (int i_row = 0; i_row < ROWS; ++i_row)
	{
		for (int i_col = 0; i_col < COLS; ++i_col)
		{
			// ������ͼ����� 5x9 ��ͼ��飨�߽���չ�󣩣���ֵ�������洢ÿһ��Ԥ��ģʽ����ͼ�������
			// 
			for (int i = 0; i < BLOCKHEIGHT + 1; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH + 5; ++j)
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
			// �ֱ���11��ģʽ��ͼ������Ԥ��
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

			//�����ÿһ��Ԥ��ģʽ��Ԥ��в���Ƚϲв���������������͵�Ԥ��ģʽ�µĲв��,����ֵΪ�в�����
			CalcBestResi(block_resi, i_row, i_col);
			//����ǰ��Ĳв�ֵ���浽����ͼ��Ĳв������
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	���ش��󣡣�����
					resi[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_resi[i_row][i_col][i][j];
				}
			}
			// �Եõ������ŵĲв����б任������������������任��
			resi_energy += DCT_Quanter(block_resi, outdataDct_Quant);
			//DCT_Quanter��Ľ����Ԥ��ֵ������ӵõ��ؽ��飬�ؽ����ֵ������image_construct�е�ǰ���Ӧλ�ô������ǵ�ԭʼͼ��
			//Ϊ��һ��ѭ����ȡ��һ������ڽ��ؽ�����ֵ��׼��
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//block_construct[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j] + outdataDct_Quant[i_row][i_col][i][j];
					//image_construct[i_row][i_col][i + 1][j + 1] = block_construct[i_row][i_col][i][j]; ���ش��󣡣��м�״̬����Ҫ���������ֻ꣬��Ҫ����ֵ
					block_construct[i][j] = block_resi[i][j] + outdataDct_Quant[i][j];
					image_construct[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_construct[i][j];

				}
			}
		}
	}
	return resi_energy;
}

