#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <iostream>
#include "pre.h"
#include "common.h"
using namespace std;
//��ͼ�����б任��������任������
void DCT_Quanter(int16_t inputData[4][4], int16_t outputData[4][4]);

//#define FDEC_STRIDE 32
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

long long CalcBestResi(short block_image[8][8], int16_t block_resi[4][4], int16_t block_pre[4][4], int i_row, int i_col)
{
	int R[9];
	for (int i = 0; i < 9; ++i)
		R[i] = 0;

	int resi_temp[4][4];
	memset(resi_temp, 0, sizeof(resi_temp));
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//Ԥ��в�
			R[0] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//Ԥ��в�
			R[1] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//Ԥ��в�
			R[2] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre3[i][j];//Ԥ��в�
			R[3] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre4[i][j];//Ԥ��в�
			R[4] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre5[i][j];//Ԥ��в�
			R[5] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre6[i][j];//Ԥ��в�
			R[6] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre7[i][j];//Ԥ��в�
			R[7] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre8[i][j];//Ԥ��в�
			R[8] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	int modeMinRes = 0;
	int minRes = R[modeMinRes];
	for (int i = 0; i < 9; ++i)
	{
		if (R[i] < R[modeMinRes])
		{
			modeMinRes = i;
			minRes = R[i];
		}
	}

	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
	cout << i_row << " "<< i_col << " " << minRes << " " << modeMinRes << endl;

	count[modeMinRes] += 1;
	switch (modeMinRes)
	{
	case 0:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//Ԥ��в�
			}
		}
		break;

	case 1:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//Ԥ��в�
			}
		}
		break;

	case 2:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//Ԥ��в�

			}
		}
		break;

	case 3:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre3[i][j];				
				block_resi[i][j] = block_image[i][j] - block_pre3[i][j];//Ԥ��в�
			}
		}
		break;

	case 4:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre4[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre4[i][j];//Ԥ��в�

			}
		}
		break;

	case 5:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre5[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre5[i][j];//Ԥ��в�
			}
		}
		break;

	case 6:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre6[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre6[i][j];//Ԥ��в�
			}
		}
		break;

	case 7:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre7[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre7[i][j];//Ԥ��в�
			}
		}
		break;

	case 8:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre8[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre8[i][j];//Ԥ��в�
			}
		}
		break;
	default:
		break;
	}
	return minRes;
}
struct block CalcBestResi(short block_image[10][10], int16_t block_resi[4][4], int16_t block_pre[4][4], int i_row, int i_col, block blocktab[][256], short mode_block)
{
	int R[9];
	for (int i = 0; i < 9; ++i)
		R[i] = 0;

	int resi_temp[4][4];
	memset(resi_temp, 0, sizeof(resi_temp));
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//Ԥ��в�
			R[0] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//Ԥ��в�
			R[1] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//Ԥ��в�
			R[2] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre3[i][j];//Ԥ��в�
			R[3] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre4[i][j];//Ԥ��в�
			R[4] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre5[i][j];//Ԥ��в�
			R[5] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre6[i][j];//Ԥ��в�
			R[6] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre7[i][j];//Ԥ��в�
			R[7] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre8[i][j];//Ԥ��в�
			R[8] += resi_temp[i][j] * resi_temp[i][j];
		}
	}

	int modeMinRes = 0;
	int minRes = R[modeMinRes];
	for (int i = 0; i < 9; ++i)
	{
		if (R[i] < R[modeMinRes])
		{
			modeMinRes = i;
			minRes = R[i];
		}
	}

	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
	//cout << i_row << " "<< i_col << " " << minRes << " " << modeMinRes << endl;

	//count[modeMinRes] += 1;

	/***������ģʽ���ó�Ш���˲����õ��Ŀ鷽��***/
	//modeMinRes = mode_block;
	/***************************************/

	switch (modeMinRes)
	{
	case 0:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//Ԥ��в�
			}
		}
		break;

	case 1:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//Ԥ��в�
			}
		}
		break;

	case 2:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//Ԥ��в�

			}
		}
		break;

	case 3:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre3[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre3[i][j];//Ԥ��в�
			}
		}
		break;

	case 4:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre4[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre4[i][j];//Ԥ��в�

			}
		}
		break;

	case 5:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre5[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre5[i][j];//Ԥ��в�
			}
		}
		break;

	case 6:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre6[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre6[i][j];//Ԥ��в�
			}
		}
		break;

	case 7:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre7[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre7[i][j];//Ԥ��в�
			}
		}
		break;

	case 8:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre8[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre8[i][j];//Ԥ��в�
			}
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			blocktab[i_row][i_col].min_pre[i][j] = block_pre[i][j];				
			blocktab[i_row][i_col].min_resi[i][j] = block_image[i + 3][j + 3] - block_pre[i][j];//Ԥ��в�
		}
	}
	 blocktab[i_row][i_col].min_energy = minRes;
	 blocktab[i_row][i_col].modeMinRes = modeMinRes;
	 return blocktab[i_row][i_col];
}
// long long predict_4_parameter(short block_image[8][8], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col)
// {
// 	double **para = NULL;
// 	para = (double **)calloc(4,sizeof(double *));
// 	for(int i = 0; i < 4; i++)
// 	{
// 		para[i] = (double *)calloc(1,sizeof(double));
// 	}
// 
// 	int R[9];
// 	for (int mode = 0; mode < 9; ++mode)
// 		R[mode] = 0;
// 
// 	for(int r = 0; r < BLOCKHEIGHT + 4; r++)    
// 	{
// 		for(int c = 0; c < BLOCKWIDTH + 4; c++)
// 		{
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
// 				block_image[r][c] = 128;  
// 		}
// 	}
// 
// 	printf("start estimate\n");
// 	for(int mode = 0; mode < 9; ++mode)
// 	{
// 		estimate_four_para(block_image, para, mode);
// 		printf("finally para result is\n");
// 		for(int i = 0; i < 4; ++i)
// 		{
// 			for(int j = 0; j < 1; ++j)
// 			{
// 				printf("%f", para[i][j]); 
// 			}
// 			printf("\n");
// 		}
// 
// 		predict_four_para(block_image, block_resi, para, mode);
// 		//predict_two_para(Para2_image, resi, predicted, para, height, width);
// 		//predict_three_para(Para3_image, resi, predicted, para, height, width);
// 		//predict_four_para(Para4_image, resi, predicted, para, height, width);
// 	}
// 
// 	long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
// 	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
// 
// 	//int modeMinRes = 0;
// 	//int minRes = R[modeMinRes];
// 	//for (int i = 0; i < 11; ++i)
// 	//{
// 	//	if (R[i] < R[modeMinRes])
// 	//	{
// 	//		modeMinRes = i;
// 	//		minRes = R[i];
// 	//	}
// 	//}
// 	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
// 	//cout << minRes << endl;
// 
// 	//count[modeMinRes] += 1;
// 	//estimate(block_image, para, i_row, i_col, modeMinRes);
// 	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
// 
// 	//printf("simple block (%d, %d) end \n", i_row, i_col);
// 
// 	for(int i = 0; i < 4; ++i)
// 	{
// 		free(para[i]);
// 	}
// 	free(para);
// 	return energy_temp;
// }
// long long predict_3_parameter(short block_image[8][8], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col)
// {
// 
// 
// 	double **para = NULL;
// 	para = (double **)calloc(3,sizeof(double *));
// 	for(int i = 0; i < 3; i++)
// 	{
// 		para[i] = (double *)calloc(1,sizeof(double));
// 	}
// 
// 	int R[9];
// 	for (int mode = 0; mode < 9; ++mode)
// 		R[mode] = 0;
// 
// 	for(int r = 0; r < BLOCKHEIGHT + 4; r++)    
// 	{
// 		for(int c = 0; c < BLOCKWIDTH + 4; c++)
// 		{
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
// 				block_image[r][c] = 128;  
// 		}
// 	}
// 
// 	printf("start estimate\n");
// 	for(int mode = 0; mode < 9; ++mode)
// 	{
// 		estimate_three_para(block_image, para, mode);
// 		printf("finally para result is\n");
// 		for(int i = 0; i < 3; ++i)
// 		{
// 			for(int j = 0; j < 1; ++j)
// 			{
// 				printf("%f", para[i][j]); 
// 			}
// 			printf("\n");
// 		}
// 
// 		predict_three_para(block_image, block_resi, para, mode);
// 		//predict_two_para(Para2_image, resi, predicted, para, height, width);
// 		//predict_three_para(Para3_image, resi, predicted, para, height, width);
// 		//predict_four_para(Para4_image, resi, predicted, para, height, width);
// 	}
// 
// 	long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
// 	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
// 
// 	//int modeMinRes = 0;
// 	//int minRes = R[modeMinRes];
// 	//for (int i = 0; i < 11; ++i)
// 	//{
// 	//	if (R[i] < R[modeMinRes])
// 	//	{
// 	//		modeMinRes = i;
// 	//		minRes = R[i];
// 	//	}
// 	//}
// 	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
// 	//cout << minRes << endl;
// 
// 	//count[modeMinRes] += 1;
// 	//estimate(block_image, para, i_row, i_col, modeMinRes);
// 	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
// 
// 	//printf("simple block (%d, %d) end \n", i_row, i_col);
// 
// 	for(int i = 0; i < 3; ++i)
// 	{
// 		free(para[i]);
// 	}
// 	free(para);
// 	return energy_temp;
// }
// long long predict_2_parameter(short block_image[8][8], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col)
// {
// 
// 
// 	double **para = NULL;
// 	para = (double **)calloc(3,sizeof(double *));
// 	for(int i = 0; i < 3; i++)
// 	{
// 		para[i] = (double *)calloc(1,sizeof(double));
// 	}
// 
// 	int R[9];
// 	for (int mode = 0; mode < 9; ++mode)
// 		R[mode] = 0;
// 
// 	for(int r = 0; r < BLOCKHEIGHT + 4; r++)    
// 	{
// 		for(int c = 0; c < BLOCKWIDTH + 4; c++)
// 		{
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
// 				block_image[r][c] = 128;  
// 		}
// 	}
// 
// 	printf("start estimate\n");
// 	for(int mode = 0; mode < 9; ++mode)
// 	{
// 		estimate_two_para(block_image, para, mode);
// 		printf("finally para result is\n");
// 		for(int i = 0; i < 2; ++i)
// 		{
// 			for(int j = 0; j < 1; ++j)
// 			{
// 				printf("%f", para[i][j]); 
// 			}
// 			printf("\n");
// 		}
// 
// 		predict_two_para(block_image, block_resi, para, mode);
// 		//predict_two_para(Para2_image, resi, predicted, para, height, width);
// 		//predict_three_para(Para3_image, resi, predicted, para, height, width);
// 		//predict_four_para(Para4_image, resi, predicted, para, height, width);
// 	}
// 
// 	long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
// 	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
// 
// 	//int modeMinRes = 0;
// 	//int minRes = R[modeMinRes];
// 	//for (int i = 0; i < 11; ++i)
// 	//{
// 	//	if (R[i] < R[modeMinRes])
// 	//	{
// 	//		modeMinRes = i;
// 	//		minRes = R[i];
// 	//	}
// 	//}
// 	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
// 	//cout << minRes << endl;
// 
// 	//count[modeMinRes] += 1;
// 	//estimate(block_image, para, i_row, i_col, modeMinRes);
// 	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
// 
// 	//printf("simple block (%d, %d) end \n", i_row, i_col);
// 
// 	for(int i = 0; i < 3; ++i)
// 	{
// 		free(para[i]);
// 	}
// 	free(para);
// 	return energy_temp;
// }
// long long predict_1_parameter(short block_image[8][8], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col)
// {
// 
// 
// 	double **para = NULL;
// 	para = (double **)calloc(3,sizeof(double *));
// 	for(int i = 0; i < 3; i++)
// 	{
// 		para[i] = (double *)calloc(1,sizeof(double));
// 	}
// 
// 	int R[9];
// 	for (int mode = 0; mode < 9; ++mode)
// 		R[mode] = 0;
// 
// 	for(int r = 0; r < BLOCKHEIGHT + 4; r++)    
// 	{
// 		for(int c = 0; c < BLOCKWIDTH + 4; c++)
// 		{
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
// 				block_image[r][c] = 128;  
// 		}
// 	}
// 
// 	printf("start estimate\n");
// 	for(int mode = 0; mode < 9; ++mode)
// 	{
// 		estimate_one_para(block_image, para, mode);
// 		printf("finally para result is\n");
// 		for(int i = 0; i < 2; ++i)
// 		{
// 			for(int j = 0; j < 1; ++j)
// 			{
// 				printf("%f", para[i][j]); 
// 			}
// 			printf("\n");
// 		}
// 
// 		predict_one_para(block_image, block_resi, para, mode);
// 		//predict_two_para(Para2_image, resi, predicted, para, height, width);
// 		//predict_three_para(Para3_image, resi, predicted, para, height, width);
// 		//predict_four_para(Para4_image, resi, predicted, para, height, width);
// 	}
// 
// 	long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
// 	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
// 
// 	//int modeMinRes = 0;
// 	//int minRes = R[modeMinRes];
// 	//for (int i = 0; i < 11; ++i)
// 	//{
// 	//	if (R[i] < R[modeMinRes])
// 	//	{
// 	//		modeMinRes = i;
// 	//		minRes = R[i];
// 	//	}
// 	//}
// 	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
// 	//cout << minRes << endl;
// 
// 	//count[modeMinRes] += 1;
// 	//estimate(block_image, para, i_row, i_col, modeMinRes);
// 	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
// 
// 	//printf("simple block (%d, %d) end \n", i_row, i_col);
// 
// 	for(int i = 0; i < 3; ++i)
// 	{
// 		free(para[i]);
// 	}
// 	free(para);
// 	return energy_temp;
// }
block predict_4_parameter(short block_image[10][10], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256], int mode_block)
{
	double **para = NULL;
	para = (double **)calloc(10,sizeof(double *));
	for(int i = 0; i < 10; i++)
	{
		para[i] = (double *)calloc(1,sizeof(double));
	}
	double **para_9mode = NULL;
	para_9mode = (double **)calloc(10,sizeof(double *));
	for(int i = 0; i < 10; i++)
	{
		para_9mode[i] = (double *)calloc(10,sizeof(double));
	}
	int R[9];
	for (int mode = 0; mode < 9; ++mode)
		R[mode] = 0;

	for(int r = 0; r < BLOCKHEIGHT + 6; r++)    
	{
		for(int c = 0; c < BLOCKWIDTH + 6; c++)
		{
			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
				block_image[r][c] = 128;  
		}
	}

	//printf("predict_4_parameter start estimate\n");
	for(int mode = 0; mode < 10; ++mode)
	{
		estimate_four_para(block_image, para, mode);
		//printf("mode %d finally para result is\n", mode);
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 1; ++j)
				para_9mode[i][mode] = para[i][j];
		}

		predict_four_para(block_image, block_resi, para, mode);
		//predict_two_para(Para2_image, resi, predicted, para, height, width);
		//predict_three_para(Para3_image, resi, predicted, para, height, width);
		//predict_four_para(Para4_image, resi, predicted, para, height, width);
	}

	//cout << "\t\t\t\t";
	//printf("%10d%10d%10d%10d%10d%10d%10d%10d%10d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8);
	for(int i = 0; i < 4; ++i)
	{
		if (i == 0)
			cout << "4 predict param\t";
		else
			cout << "\t\t\t\t";
		for(int j = 0; j < 10; ++j)
			printf("%10.4f", para_9mode[i][j]); 
		printf("\n");
	}
	//printf("predict_4_parameter estimate and predict end\n");
	printf("\n");

	//long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
	block energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col, blocktab, mode_block);
	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);

	//int modeMinRes = 0;
	//int minRes = R[modeMinRes];
	//for (int i = 0; i < 11; ++i)
	//{
	//	if (R[i] < R[modeMinRes])
	//	{
	//		modeMinRes = i;
	//		minRes = R[i];
	//	}
	//}
	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
	//cout << minRes << endl;

	//count[modeMinRes] += 1;
	//estimate(block_image, para, i_row, i_col, modeMinRes);
	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);

	//printf("simple block (%d, %d) end \n", i_row, i_col);

	for(int i = 0; i < 10; ++i)
	{
		free(para[i]);
		free(para_9mode[i]);
	}
	free(para);
	free(para_9mode);
	return energy_temp;
}
//block predict_3_parameter(short block_image[10][10], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256])
//{
//	double **para = NULL;
//	double **para_9mode = NULL;
//	para = (double **)calloc(10,sizeof(double *));
//	for(int i = 0; i < 10; i++)
//	{
//		para[i] = (double *)calloc(1,sizeof(double));
//	}
//	para_9mode = (double **)calloc(10,sizeof(double *));
//	for(int i = 0; i < 10; i++)
//	{
//		para_9mode[i] = (double *)calloc(10,sizeof(double));
//	}
//	int R[9];
//	for (int mode = 0; mode < 9; ++mode)
//		R[mode] = 0;
//
//	for(int r = 0; r < BLOCKHEIGHT + 6; r++)    
//	{
//		for(int c = 0; c < BLOCKWIDTH + 6; c++)
//		{
//			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
//				block_image[r][c] = 128;  
//		}
//	}
//
//	//printf("predict_3_parameter start estimate\n");
//	for(int mode = 0; mode < 9; ++mode)
//	{
//		estimate_three_para(block_image, para, mode);
//		//printf("mode %d finally para result is\n", mode);
//		for(int i = 0; i < 3; ++i)
//		{
//			for(int j = 0; j < 1; ++j)
//				para_9mode[i][mode] = para[i][j]; 
//		}
//
//		predict_three_para(block_image, block_resi, para, mode);
//		//predict_two_para(Para2_image, resi, predicted, para, height, width);
//		//predict_three_para(Para3_image, resi, predicted, para, height, width);
//		//predict_four_para(Para4_image, resi, predicted, para, height, width);
//	}
//	//cout << "\t\t\t\t";
//	//printf("%10d%10d%10d%10d%10d%10d%10d%10d%10d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8);
//	for(int i = 0; i < 3; ++i)
//	{
//		if (i == 0)
//			cout << "3 predict param\t";
//		else
//			cout << "\t\t\t\t";
//		for(int j = 0; j < 9; ++j)
//			printf("%10.4f", para_9mode[i][j]); 
//		printf("\n");
//	}
//	//printf("predict_3_parameter estimate and predict end\n");
//	printf("\n");
//
//	//long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
//	block energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col, blocktab);
//	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
//
//	//int modeMinRes = 0;
//	//int minRes = R[modeMinRes];
//	//for (int i = 0; i < 11; ++i)
//	//{
//	//	if (R[i] < R[modeMinRes])
//	//	{
//	//		modeMinRes = i;
//	//		minRes = R[i];
//	//	}
//	//}
//	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
//	//cout << minRes << endl;
//
//	//count[modeMinRes] += 1;
//	//estimate(block_image, para, i_row, i_col, modeMinRes);
//	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
//
//	//printf("simple block (%d, %d) end \n", i_row, i_col);
//
//	for(int i = 0; i < 10; ++i)
//	{
//		free(para[i]);
//		free(para_9mode[i]);
//	}
//	free(para);
//	free(para_9mode);
//	return energy_temp;
//}
//block predict_2_parameter(short block_image[10][10], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256])
//{
//
//
//	double **para = NULL;
//	double **para_9mode = NULL;
//	para = (double **)calloc(10,sizeof(double *));
//	for(int i = 0; i < 10; i++)
//	{
//		para[i] = (double *)calloc(1,sizeof(double));
//	}
//	para_9mode = (double **)calloc(10,sizeof(double *));
//	for(int i = 0; i < 10; i++)
//	{
//		para_9mode[i] = (double *)calloc(10,sizeof(double));
//	}
//
//	int R[9];
//	for (int mode = 0; mode < 9; ++mode)
//		R[mode] = 0;
//
//	for(int r = 0; r < BLOCKHEIGHT + 6; r++)    
//	{
//		for(int c = 0; c < BLOCKWIDTH + 6; c++)
//		{
//			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
//				block_image[r][c] = 128;  
//		}
//	}
//
//	//printf("predict_2_parameter start estimate\n");
//	for(int mode = 0; mode < 9; ++mode)
//	{
//		estimate_two_para(block_image, para, mode);
//		//printf("mode %d finally para result is\n", mode);
//		for(int i = 0; i < 2; ++i)
//		{
//			for(int j = 0; j < 1; ++j)
//				para_9mode[i][mode] = para[i][j]; 
//		}
//
//		predict_two_para(block_image, block_resi, para, mode);
//		//predict_two_para(Para2_image, resi, predicted, para, height, width);
//		//predict_three_para(Para3_image, resi, predicted, para, height, width);
//		//predict_four_para(Para4_image, resi, predicted, para, height, width);
//	}
//	//cout << "\t\t\t\t";
//	//printf("%10d%10d%10d%10d%10d%10d%10d%10d%10d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8);
//	for(int i = 0; i < 2; ++i)
//	{
//		if (i == 0)
//			cout << "2 predict param\t";
//		else
//			cout << "\t\t\t\t";
//		for(int j = 0; j < 9; ++j)
//			printf("%10.4f", para_9mode[i][j]); 
//		printf("\n");
//	}
//	//printf("predict_2_parameter estimate and predict end\n");
//	printf("\n");
//
//	//long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
//	block energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col, blocktab);
//	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
//
//	//int modeMinRes = 0;
//	//int minRes = R[modeMinRes];
//	//for (int i = 0; i < 11; ++i)
//	//{
//	//	if (R[i] < R[modeMinRes])
//	//	{
//	//		modeMinRes = i;
//	//		minRes = R[i];
//	//	}
//	//}
//	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
//	//cout << minRes << endl;
//
//	//count[modeMinRes] += 1;
//	//estimate(block_image, para, i_row, i_col, modeMinRes);
//	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
//
//	//printf("simple block (%d, %d) end \n", i_row, i_col);
//
//	for(int i = 0; i < 10; ++i)
//	{
//		free(para[i]);
//		free(para_9mode[i]);
//	}
//	free(para);
//	free(para_9mode);
//	return energy_temp;
//}
//block predict_1_parameter(short block_image[10][10], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256])
//{
//	double **para = NULL;
//	para = (double **)calloc(10,sizeof(double *));
//	for(int i = 0; i < 10; i++)
//	{
//		para[i] = (double *)calloc(1,sizeof(double));
//	}
//	double **para_9mode = NULL;
//	para_9mode = (double **)calloc(10,sizeof(double *));
//	for(int i = 0; i < 10; i++)
//	{
//		para_9mode[i] = (double *)calloc(10,sizeof(double));
//	}
//
//	int R[9];
//	for (int mode = 0; mode < 9; ++mode)
//		R[mode] = 0;
//
//	for(int r = 0; r < BLOCKHEIGHT + 6; r++)    
//	{
//		for(int c = 0; c < BLOCKWIDTH + 6; c++)
//		{
//			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// ����ģʽ3��7,��ͼ�����ұ����к��±����е�ֵ��ֵ��128
//				block_image[r][c] = 128;
//		}
//	}
//
//	//printf("predict_1_parameter start estimate\n");
//	for(int mode = 0; mode < 8; ++mode)
//	{
//		estimate_one_para(block_image, para, mode);
//		//printf("mode %d finally para result is\n", mode);
//		for(int i = 0; i < 1; ++i)
//		{
//			for(int j = 0; j < 1; ++j)
//				para_9mode[i][mode] = para[i][j];  
//		}
//		predict_one_para(block_image, block_resi, para, mode);
//		//predict_two_para(Para2_image, resi, predicted, para, height, width);
//		//predict_three_para(Para3_image, resi, predicted, para, height, width);
//		//predict_four_para(Para4_image, resi, predicted, para, height, width);
//	}
//	//printf("%-10d%-10d%-10d%-10d%-10d%-10d%-10d%-10d%-10d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8);
//	for(int i = 0; i < 1; ++i)
//	{
//		if (i == 0)
//			cout << "1 predict param\t";
//		else
//			cout << "\t\t\t\t";
//		for(int j = 0; j < 9; ++j)
//			//printf("%f\t\t", para_9mode[i][j]);
//			printf("%10.4f", para_9mode[i][j]);
//		printf("\n");
//	}
//	//printf("predict_1_parameter estimate and predict end\n");
//	printf("\n");
//
//	//long long energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col);
//	block energy_temp = CalcBestResi(block_image, block_resi, block_pre, i_row, i_col, blocktab);
//	//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
//
//	//int modeMinRes = 0;
//	//int minRes = R[modeMinRes];
//	//for (int i = 0; i < 11; ++i)
//	//{
//	//	if (R[i] < R[modeMinRes])
//	//	{
//	//		modeMinRes = i;
//	//		minRes = R[i];
//	//	}
//	//}
//	//printf("BLOCK [%4d][%4d] %d %d\n",i_row, i_col, minRes, modeMinRes);
//	//cout << minRes << endl;
//
//	//count[modeMinRes] += 1;
//	//estimate(block_image, para, i_row, i_col, modeMinRes);
//	//predict_paraN1(block_image, resi, predicted, para, modeMinRes);
//
//	//printf("simple block (%d, %d) end \n", i_row, i_col);
//
//	for(int i = 0; i < 10; ++i)
//	{
//		free(para[i]);
//		free(para_9mode[i]);
//	}
//	free(para);
//	free(para_9mode);
//	return energy_temp;
//}
// �Ը���ͼ��鰴12��Ԥ��ģʽ����Ԥ�⣬ѡ����õ�һ�ֵõ��в�,����ȫͼ�Ĳв�����
long long predict(short **image_construct, short **resi, short **predicted, int height, int width, short **direction_block)
{
	// ͼ��в�顢�任�������ͼ��顢���ڴ�ռ����
	short block_resi[4][4] = {0};          	   //ÿһ��С��Ĳв�ֵ
	short outdataDct_Quant[4][4] = {0};		    //ÿһ��С��ı任�������ֵ
	short block_construct[4][4] = {0};		   //ÿһ��С����ؽ�ֵ
	short block_pre[4][4] = {0};			   //ÿһ��С���Ԥ��ֵ
	long long resi_energy = 0;				   // �����Ѵ������ۼƲв�����
	long long energy_3_parameter = 0;          //ÿһ��С��Ĳв�����ֵ
	long long energy_4_parameter = 0;          //ÿһ��С��Ĳв�����ֵ
	long long energy_1_parameter = 0;          //ÿһ��С��Ĳв�����ֵ
	long long energy_2_parameter = 0;          //ÿһ��С��Ĳв�����ֵ

	block parameter3;          //ÿһ��С��в����ݣ�Ԥ�����ݣ�����Ԥ��ģʽ���в�����
	block parameter4;          //ÿһ��С��в����ݣ�Ԥ�����ݣ�����Ԥ��ģʽ���в�����
	block parameter1;          //ÿһ��С��в����ݣ�Ԥ�����ݣ�����Ԥ��ģʽ���в�����
	block parameter2;          //ÿһ��С��в����ݣ�Ԥ�����ݣ�����Ԥ��ģʽ���в�����

	int  min_energy = 0;			        //���ֲ�ͬ���������Ŀ�в�������Сֵ
	int  para_label = 0;						//�������Ԥ�������õĲ�������
	short min_resi[4][4] = {0};					//�в�������Сʱ��Ӧ�Ĳв������
	short min_pre[4][4] = {0};					//�в�������Сʱ��Ӧ�Ŀ�Ԥ������
	int  modeMinRes = 0;                        //����Ԥ���Ӧ��ģʽ
	short direction_mode = 0;   //Ш���˲����õ��ķ���

	short resi_temp[4][4] = {0};                 //��в�����
	short smallimage[256][256][10][10] = {0};
	//short **smallimage = NULL;
	//smallimage  = (short **)calloc(10, sizeof(short *));
	//for(int i = 0; i < 10; i++)
	//{
	//	smallimage[i] = (short *)calloc(10,sizeof(short));
	//}
	//short **Para1_image = NULL;
	//Para1_image  = (short **)calloc(10, sizeof(short *));
	//for(int i = 0; i < 10; i++)
	//{
	//	Para1_image[i] = (short *)calloc(10,sizeof(short));
	//}
	//short **Para2_image = NULL;
	//Para2_image  = (short **)calloc(10, sizeof(short *));
	//for(int i = 0; i < 10; i++)
	//{
	//	Para2_image[i] = (short *)calloc(10,sizeof(short));
	//}
	//short **Para3_image = NULL;
	//Para3_image  = (short **)calloc(10, sizeof(short *));
	//for(int i = 0; i < 10; i++)
	//{
	//	Para3_image[i] = (short *)calloc(10,sizeof(short));
	//}
	//short **Para4_image = NULL;
	//Para4_image  = (short **)calloc(10, sizeof(short *));
	//for(int i = 0; i < 10; i++)
	//{
	//	Para4_image[i] = (short *)calloc(10,sizeof(short));
	//}
	short Para1_image[10][10] = {0};
	short Para2_image[10][10] = {0};
	short Para3_image[10][10] = {0};
	short Para4_image[10][10] = {0};



	// ������ͼ����� 8x8 ��ͼ��飨�߽���չ�󣩣���ֵ�������洢ÿһ��Ԥ��ģʽ����ͼ������� 
	for (int i_row = 0; i_row < ROWS; ++i_row)
	{
		for (int i_col = 0; i_col < COLS; ++i_col)
		{
			for (int i = 0; i < BLOCKHEIGHT + 6; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH + 6; ++j)
				{
					smallimage[i_row][i_col][i][j] = image_construct[i_row * BLOCKHEIGHT + i] [i_col * BLOCKWIDTH + j];
				}
			}
			for (int i = 0; i < BLOCKHEIGHT + 6; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH + 6; ++j)
				{
					Para1_image[i][j] = smallimage[i_row][i_col][i][j];
					Para2_image[i][j] = smallimage[i_row][i_col][i][j];
					Para3_image[i][j] = smallimage[i_row][i_col][i][j];
					Para4_image[i][j] = smallimage[i_row][i_col][i][j];
				}
			}
			// �ֱ���4��ģʽ��ͼ������Ԥ��

			// add function pointer

			//long long(*func_arr[])((short *)[8], (short *)[4], (short *)[4], int, int) = {predict_1_parameter, predict_2_parameter, predict_3_parameter, predict_4_parameter};
			printf("-----------��ָ���---------------------------------------------------------------\n"); 			
			printf("image block (%d %d) predict beginning\n", i_row, i_col); 
			//energy_1_parameter = predict_1_parameter(Para1_image, block_resi, block_pre, i_row, i_col);
			//energy_2_parameter = predict_2_parameter(Para2_image, block_resi, block_pre, i_row, i_col);
			//energy_3_parameter = predict_3_parameter(Para3_image, block_resi, block_pre, i_row, i_col);
			//energy_4_parameter = predict_4_parameter(Para4_image, block_resi, block_pre, i_row, i_col);
			//parameter4 = predict_4_parameter(Para4_image, block_resi, block_pre, i_row, i_col, blocktab);
			direction_mode = direction_block[i_row][i_col];
			parameter4 = predict_4_parameter(Para4_image, block_resi, block_pre, i_row, i_col, blocktab, direction_mode);
			/********************************************************************************
			cout << "\t\t\t\t";
			printf("%10d%10d%10d%10d%10d%10d%10d%10d%10d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8);
			parameter1 = predict_1_parameter(Para1_image, block_resi, block_pre, i_row, i_col, blocktab);
			min_energy = parameter1.min_energy;
			modeMinRes = parameter1.modeMinRes;
			para_label = 0;

			//����ǰ���Ԥ��ֵ���浽����ͼ���Ԥ�������
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					block_pre[i][j] = parameter1.min_pre[i][j];
				}
			}
			//����ǰ��Ĳв�ֵ���浽����ͼ��Ĳв������
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					block_resi[i][j] = parameter1.min_resi[i][j];
				}
			}

			parameter2 = predict_2_parameter(Para2_image, block_resi, block_pre, i_row, i_col, blocktab);
			if(parameter2.min_energy < min_energy)
			{
				//����ǰ���Ԥ��ֵ���浽����ͼ���Ԥ�������
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_pre[i][j] = parameter2.min_pre[i][j];
					}
				}
				//����ǰ��Ĳв�ֵ���浽����ͼ��Ĳв������
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_resi[i][j] = parameter2.min_resi[i][j];
					}
				}
				modeMinRes = parameter2.modeMinRes;
				min_energy = parameter2.min_energy;
				para_label = 1;
			}
			parameter3 = predict_3_parameter(Para3_image, block_resi, block_pre, i_row, i_col, blocktab);
			if(parameter3.min_energy < min_energy)
			{
				//����ǰ���Ԥ��ֵ���浽����ͼ���Ԥ�������
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_pre[i][j] = parameter3.min_pre[i][j];
					}
				}
				//����ǰ��Ĳв�ֵ���浽����ͼ��Ĳв������
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_resi[i][j] = parameter3.min_resi[i][j];
					}
				}
				modeMinRes = parameter3.modeMinRes;
				min_energy = parameter3.min_energy;
				para_label = 2;
			}
			parameter4 = predict_4_parameter(Para4_image, block_resi, block_pre, i_row, i_col, blocktab);
			if(parameter4.min_energy < min_energy)
			{
				//����ǰ���Ԥ��ֵ���浽����ͼ���Ԥ�������
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_pre[i][j] = parameter4.min_pre[i][j];
					}
				}
				//����ǰ��Ĳв�ֵ���浽����ͼ��Ĳв������
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_resi[i][j] = parameter4.min_resi[i][j];
					}
				}
				min_energy = parameter4.min_energy;
				modeMinRes = parameter4.modeMinRes;
				para_label = 3;
			}
			****************************************************************************************/
			/**************ͳ�Ʋ�ͬ���������£���ģʽΪ����Ԥ���ͼ�����******************/
			count_para_mode[0][parameter4.modeMinRes] += 1;
			cout << "[i_row][i_col]" << i_row << " " << i_col << endl;
			cout << "min_energy = " << parameter4.min_energy << " " << "modeMinRes = " << parameter4.modeMinRes <<endl;
			resi_energy += parameter4.min_energy; 
			/*************��ÿ���������Ԥ��ģʽ����д���ļ���***********************************/
			FILE *fout_mode = fopen("optimal_direction_mode.txt", "a+");
			assert(fout_mode);
			if(fout_mode != NULL){
				if((i_col + 1) % COLS == 0)
					fprintf(fout_mode, "\n");
				else
					fprintf(fout_mode, "%4d", parameter4.modeMinRes);
			}	
			fclose(fout_mode);
			/******************************************************************************/
			FILE *fout = fopen("block_resi4.txt", "a+");
			assert(fout);
			if(fout != NULL)
				fprintf(fout, "%4d\t %4d\n", parameter4.min_energy, parameter4.modeMinRes);
			fclose(fout);
			/************ÿ��ͼ��齫��ͬ����������Ԥ��Ľ�����бȽ�ȡ����***********
			**************ͳ�Ʋ�ͬ���������£���ģʽΪ����Ԥ���ͼ�����****
			count_para_mode[para_label][modeMinRes] += 1;
			para_num[para_label] += 1;
			cout << "[i_row][i_col]" << i_row << " " << i_col << endl;
			cout << "min_energy = " << min_energy << "; " << "para num = "<< para_label + 1 <<" ;"<< "modeMinRes = " << modeMinRes  <<endl;
			resi_energy += min_energy;
			
			FILE *fout = fopen("block_resi.txt", "a+");
			assert(fout);
			if(fout != NULL)
				fprintf(fout, "%4d\t (%d,%d)\n", min_energy, para_label + 1, modeMinRes);
			fclose(fout);
			**************/

			//�����ÿһ��Ԥ��ģʽ��Ԥ��в���Ƚϲв���������������͵�Ԥ��ģʽ�µĲв��,����ֵΪ�в�����
			//long long energy_temp = CalcBestResi(block_resi, block_pre, i_row, i_col);
			//resi_energy += energy_temp;
			
			//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_2_parameter);
			cout << "Accumulated energe = " << resi_energy << "\n" << endl;
			//����ǰ���Ԥ��ֵ���浽����ͼ���Ԥ�������
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	���ش��󣡣�����
					predicted[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_pre[i][j];
				}
			}
			//����ǰ��Ĳв�ֵ���浽����ͼ��Ĳв������
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	���ش��󣡣�����
					resi[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_resi[i][j];
				}
			}
			// �Եõ������ŵĲв����б任������������������任��
			/**************************************************************
	         * 3. transform and quant the residual of prediction
	         *************************************************************/
	  
			DCT_Quanter(block_resi, outdataDct_Quant);
			//DCT_Quanter��Ľ����Ԥ��ֵ������ӵõ��ؽ��飬�ؽ����ֵ������image_construct�е�ǰ���Ӧλ�ô������ǵ�ԭʼͼ��
			//Ϊ��һ��ѭ����ȡ��һ������ڽ��ؽ�����ֵ��׼��
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//block_construct[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j] + outdataDct_Quant[i_row][i_col][i][j];
					//image_construct[i_row][i_col][i + 1][j + 1] = block_construct[i_row][i_col][i][j]; ���ش��󣡣��м�״̬����Ҫ���������ֻ꣬��Ҫ����ֵ
					block_construct[i][j] = block_pre[i][j] + outdataDct_Quant[i][j];
					image_construct[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_construct[i][j];

				}
			}
		}
	}
	return resi_energy;
}

