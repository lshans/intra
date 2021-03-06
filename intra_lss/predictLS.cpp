#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <iostream>
#include "pre.h"
#include "common.h"
using namespace std;
//对图像块进行变换量化、逆变换逆量化
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
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//预测残差
			R[0] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//预测残差
			R[1] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//预测残差
			R[2] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre3[i][j];//预测残差
			R[3] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre4[i][j];//预测残差
			R[4] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre5[i][j];//预测残差
			R[5] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre6[i][j];//预测残差
			R[6] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre7[i][j];//预测残差
			R[7] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 2][j + 2] - block_pre8[i][j];//预测残差
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
				block_resi[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//预测残差
			}
		}
		break;

	case 1:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//预测残差
			}
		}
		break;

	case 2:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_012pre[i][j];//预测残差

			}
		}
		break;

	case 3:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre3[i][j];				
				block_resi[i][j] = block_image[i][j] - block_pre3[i][j];//预测残差
			}
		}
		break;

	case 4:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre4[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre4[i][j];//预测残差

			}
		}
		break;

	case 5:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre5[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre5[i][j];//预测残差
			}
		}
		break;

	case 6:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre6[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre6[i][j];//预测残差
			}
		}
		break;

	case 7:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre7[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre7[i][j];//预测残差
			}
		}
		break;

	case 8:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre8[i][j];				
				block_resi[i][j] = block_image[i + 2][j + 2] - block_pre8[i][j];//预测残差
			}
		}
		break;
	default:
		break;
	}
	return minRes;
}
struct block CalcBestResi(short block_image[10][10], int16_t block_resi[4][4], int16_t block_pre[4][4], int i_row, int i_col, block blocktab[][COLS], int mode_block)
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
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//预测残差
			R[0] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//预测残差
			R[1] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//预测残差
			R[2] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre3[i][j];//预测残差
			R[3] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre4[i][j];//预测残差
			R[4] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre5[i][j];//预测残差
			R[5] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre6[i][j];//预测残差
			R[6] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre7[i][j];//预测残差
			R[7] += resi_temp[i][j] * resi_temp[i][j];
		}
	}
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			resi_temp[i][j] = block_image[i + 3][j + 3] - block_pre8[i][j];//预测残差
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

	/***将最优模式设置成楔形滤波器得到的块方向***/
	modeMinRes = mode_block;
	/***************************************/

	switch (modeMinRes)
	{
	case 0:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//预测残差
			}
		}
		break;

	case 1:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];
				block_resi[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//预测残差
			}
		}
		break;

	case 2:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_012pre[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_012pre[i][j];//预测残差

			}
		}
		break;

	case 3:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre3[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre3[i][j];//预测残差
			}
		}
		break;

	case 4:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre4[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre4[i][j];//预测残差

			}
		}
		break;

	case 5:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre5[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre5[i][j];//预测残差
			}
		}
		break;

	case 6:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre6[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre6[i][j];//预测残差
			}
		}
		break;

	case 7:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre7[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre7[i][j];//预测残差
			}
		}
		break;

	case 8:
		for (int i = 0; i < BLOCKHEIGHT; ++i)
		{
			for (int j = 0; j < BLOCKWIDTH; ++j)
			{
				block_pre[i][j] = block_pre8[i][j];				
				block_resi[i][j] = block_image[i + 3][j + 3] - block_pre8[i][j];//预测残差
			}
		}
		break;
	default:
		break;
	}
	int minRes_final = 0;
	for (int i = 0; i < BLOCKHEIGHT; ++i)
	{
		for (int j = 0; j < BLOCKWIDTH; ++j)
		{
			blocktab[i_row][i_col].min_pre[i][j] = block_pre[i][j];				
			blocktab[i_row][i_col].min_resi[i][j] = block_image[i + 3][j + 3] - block_pre[i][j];//预测残差
			minRes_final += (blocktab[i_row][i_col].min_resi[i][j] * blocktab[i_row][i_col].min_resi[i][j]);
		}
	}

	 blocktab[i_row][i_col].min_energy = minRes_final;
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
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
// 			if(r > BLOCKHEIGHT + 1  || (r > 2 && c > BLOCKWIDTH + 1))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
block predict_4_parameter(short block_image[10][10], short block_resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][COLS], int mode_block)
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
			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
				block_image[r][c] = 128;  
		}
	}

	//printf("predict_4_parameter start estimate\n");
	for(int mode = 1; mode < 9; ++mode)
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
//			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
//			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
//			if(r > BLOCKHEIGHT + 2  || (r > 2 && c > BLOCKWIDTH + 2))// 考虑模式3、7,将图像块的右边两列和下边两行的值赋值成128
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
// 对各个图像块按12种预测模式进行预测，选出最好的一种得到残差,返回全图的残差能量
long long predict(short **image_construct, short **resi, short **predicted, int height, int width, int **direction_block)
{
	// 图像残差块、变换量化后的图像块、的内存空间分配
	short block_resi[4][4] = {0};          	   //每一个小块的残差值
	short outdataDct_Quant[4][4] = {0};		    //每一个小块的变换量化后的值
	short block_construct[4][4] = {0};		   //每一个小块的重建值
	short block_pre[4][4] = {0};			   //每一个小块的预测值
	long long resi_energy = 0;				   // 所有已处理块的累计残差能量
	long long energy_3_parameter = 0;          //每一个小块的残差能量值
	long long energy_4_parameter = 0;          //每一个小块的残差能量值
	long long energy_1_parameter = 0;          //每一个小块的残差能量值
	long long energy_2_parameter = 0;          //每一个小块的残差能量值

	block parameter3;          //每一个小块残差数据，预测数据，最优预测模式，残差能量
	block parameter4;          //每一个小块残差数据，预测数据，最优预测模式，残差能量
	block parameter1;          //每一个小块残差数据，预测数据，最优预测模式，残差能量
	block parameter2;          //每一个小块残差数据，预测数据，最优预测模式，残差能量

	int  min_energy = 0;			        //四种不同参数个数的块残差能量最小值
	int  para_label = 0;						//标记最优预测所采用的参数个数
	short min_resi[4][4] = {0};					//残差能量最小时对应的残差块数据
	short min_pre[4][4] = {0};					//残差能量最小时对应的块预测数据
	int  modeMinRes = 0;                        //最优预测对应的模式
	int direction_mode = 0;   //楔形滤波器得到的方向

	short resi_temp[4][4] = {0};                 //块残差数据
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



	// 将输入图像读成 8x8 的图像块（边界扩展后），赋值给用来存储每一种预测模式输入图像的数组 
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
			// 分别按照4种模式对图像块进行预测

			// add function pointer

			//long long(*func_arr[])((short *)[8], (short *)[4], (short *)[4], int, int) = {predict_1_parameter, predict_2_parameter, predict_3_parameter, predict_4_parameter};
			printf("-----------块分隔符---------------------------------------------------------------\n"); 			
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

			//将当前块的预测值保存到整幅图像的预测矩阵中
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					block_pre[i][j] = parameter1.min_pre[i][j];
				}
			}
			//将当前块的残差值保存到整幅图像的残差矩阵中
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
				//将当前块的预测值保存到整幅图像的预测矩阵中
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_pre[i][j] = parameter2.min_pre[i][j];
					}
				}
				//将当前块的残差值保存到整幅图像的残差矩阵中
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
				//将当前块的预测值保存到整幅图像的预测矩阵中
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_pre[i][j] = parameter3.min_pre[i][j];
					}
				}
				//将当前块的残差值保存到整幅图像的残差矩阵中
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
				//将当前块的预测值保存到整幅图像的预测矩阵中
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						block_pre[i][j] = parameter4.min_pre[i][j];
					}
				}
				//将当前块的残差值保存到整幅图像的残差矩阵中
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
			/**************统计不同参数个数下，各模式为最优预测的图像块数******************/
			count_para_mode[0][parameter4.modeMinRes] += 1;
			cout << "[i_row][i_col]" << i_row << " " << i_col << endl;
			cout << "min_energy = " << parameter4.min_energy << " " << "modeMinRes = " << parameter4.modeMinRes <<endl;
			resi_energy += parameter4.min_energy; 
			/*************将每个块的最优预测模式方向写到文件中***********************************/
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
			/************每个图像块将不同参数个数下预测的结果进行比较取最优***********
			**************统计不同参数个数下，各模式为最优预测的图像块数****
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

			//计算对每一种预测模式的预测残差，并比较残差能量，将能量最低的预测模式下的残差保留,返回值为残差能量
			//long long energy_temp = CalcBestResi(block_resi, block_pre, i_row, i_col);
			//resi_energy += energy_temp;
			
			//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_2_parameter);
			cout << "Accumulated energe = " << resi_energy << "\n" << endl;
			//将当前块的预测值保存到整幅图像的预测矩阵中
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	严重错误！！！！
					predicted[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_pre[i][j];
				}
			}
			//将当前块的残差值保存到整幅图像的残差矩阵中
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	严重错误！！！！
					resi[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_resi[i][j];
				}
			}
			// 对得到的最优的残差块进行变换、量化、逆量化、逆变换，
			/**************************************************************
	         * 3. transform and quant the residual of prediction
	         *************************************************************/
	  
			DCT_Quanter(block_resi, outdataDct_Quant);
			//DCT_Quanter后的结果与预测值进行相加得到重建块，重建块的值拷贝到image_construct中当前块对应位置处，覆盖掉原始图像，
			//为下一次循环读取下一个块的邻近重建像素值做准备
			for (int i = 0; i < BLOCKHEIGHT; ++i)
			{
				for (int j = 0; j < BLOCKWIDTH; ++j)
				{
					//block_construct[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j] + outdataDct_Quant[i_row][i_col][i][j];
					//image_construct[i_row][i_col][i + 1][j + 1] = block_construct[i_row][i_col][i][j]; 严重错误！！中间状态不需要索引块坐标，只需要它的值
					block_construct[i][j] = block_pre[i][j] + outdataDct_Quant[i][j];
					image_construct[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_construct[i][j];

				}
			}
		}
	}
	return resi_energy;
}

