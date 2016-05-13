#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include "pre.h"
#include "common.h"
void predict_one_para(short block_image[8][8], short resi[4][4], double **para, int mode)
{
	// 图像残差块、变换量化后的图像块、的内存空间分配
	short block_resi[4][4] = {0};          	   //每一个小块的残差值
	short outdataDct_Quant[4][4] = {0};		  //每一个小块的变换量化后的值
	short block_construct[4][4] = {0};		 //每一个小块的重建值
	long long resi_energy = 0;					 // 所有已处理块的累计残差能量
	long long energy_temp = 0;                   //每一个小块的残差能量值
	short resi_temp[4][4] = {0};                 //块残差数据                 

	static short u[3] = {0};
	switch (mode)
	{
	case 0: 
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i - 1][j] + block_image[i - 2][j]) / 2;
				block_012pre[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 1: 
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i][j - 1] + block_image[i][j - 2]) / 2;
				block_012pre[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 2:
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i - 1][j - 1] + block_image[i - 1][j] + block_image[i][j - 1]) / 3;
				block_012pre[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 3:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2] + 2) / 4;
				block_pre3[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 4:
		//4模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + 2 * block_image[i - 1][j] + 2) / 4;
				block_pre4[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 5:
		//5模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i - 1][j] + block_image[i - 1][j - 1] + 1) / 2;
				block_pre5[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 6:
		//6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i][j - 1] + block_image[i - 1][j - 1] + 1) / 2;
				block_pre6[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 7:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i - 1][j] + block_image[i - 1][j + 1] + 1) / 2;   
				block_pre7[i - 2][j - 2] = para[0][0] * u[1];
			}
		}
		break;
	case 8:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i][j - 1] + block_image[i + 1][j - 1] + 1) / 2;//可自己构造，与模式6关于x轴对称
				block_pre8[i - 2][j - 2] = para[0][0] * u[1];
				//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre8[i - 2][j - 2];//预测残差
				//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
				//resi_energy += energy_temp;
				//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			}
		}
		break;
	default:
		break;
	}
}
void predict_two_para(short block_image[8][8], short resi[4][4], double **para, int mode)
{
	// 图像残差块、变换量化后的图像块、的内存空间分配
	short block_resi[4][4] = {0};          	   //每一个小块的残差值
	short outdataDct_Quant[4][4] = {0};		  //每一个小块的变换量化后的值
	short block_construct[4][4] = {0};		 //每一个小块的重建值
	long long resi_energy = 0;					 // 所有已处理块的累计残差能量
	long long energy_temp = 0;                   //每一个小块的残差能量值
	short resi_temp[4][4] = {0};                 //块残差数据                 

	static short u[3] = {0};
	switch (mode)
	{
	case 0: case 1: case 2:
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = (block_image[i - 1][j - 1] + 2 * block_image[i - 1][j] + block_image[i - 2][j]) / 4;
				block_012pre[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 3:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + 2 * block_image[i - 1][j + 2]) / 5;
				block_pre3[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 4:
		//4模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];//可自己构造，与模式3对称
				u[2] = (2 * block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 5;
				block_pre4[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 5:
		//5模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 4;  //自己构造
				block_pre5[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 6:
		//6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i][j - 1] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j - 2]) / 4;
				u[2] = block_image[i - 1][j];//可自己构造
				block_pre6[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 7:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];//可自己构造，与模式5对称
				u[2] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
				block_pre7[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 8:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = (block_image[i][j - 1] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
				u[2] = block_image[i - 1][j];//可自己构造，与模式6关于x轴对称
				block_pre8[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2];
				//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre8[i - 2][j - 2];//预测残差
				//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
				//resi_energy += energy_temp;
				//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			}
		}
		break;
	default:
		break;
	}
}
void predict_three_para(short block_image[8][8], short resi[4][4], double **para, int mode)
{
	// 图像残差块、变换量化后的图像块、的内存空间分配
	short block_resi[4][4] = {0};          	   //每一个小块的残差值
	short outdataDct_Quant[4][4] = {0};		  //每一个小块的变换量化后的值
	short block_construct[4][4] = {0};		 //每一个小块的重建值
	long long resi_energy = 0;					 // 所有已处理块的累计残差能量
	long long energy_temp = 0;                   //每一个小块的残差能量值
	short resi_temp[4][4] = {0};                 //块残差数据                 

	short North, NW, W;
	switch (mode)
	{
		case 0: case 1: case 2:
			//0,1,4,5,6模式可以用下面公式计算
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = block_image[i - 1][j];
					NW = block_image[i - 1][j - 1];
					W  = block_image[i][j - 1];
// 					block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
// 					resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差
					block_012pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre012[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];

					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 3:
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
					NW = block_image[i - 1][j];
					W  = block_image[i][j - 1];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差
					block_pre3[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre3[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 4:
			//4模式可以用下面公式计算
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = block_image[i - 1][j - 2];
					NW = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 4;
					W  = block_image[i][j - 1];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre4[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre4[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 5:
			//5模式可以用下面公式计算
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = (block_image[i - 1][j - 2] + block_image[i - 1][j - 1]) / 2;
					NW = (block_image[i - 1][j - 1] + block_image[i - 1][j]) / 2;
					W  = block_image[i][j - 1];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差
					block_pre5[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre5[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 6:
			//6模式可以用下面公式计算
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = (block_image[i - 1][j - 1] + block_image[i - 2][j - 1]) / 2;
					NW = block_image[i - 1][j];
					W = (block_image[i][j - 1] + block_image[i - 1][j - 1]) / 2;
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre6[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre6[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 7:
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = (block_image[i - 1][j - 1] + block_image[i - 1][j]) / 2;
					NW = (block_image[i - 1][j] + block_image[i - 1][j + 1]) / 2;
					W = block_image[i][j - 1];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre7[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre7[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 8:
			for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			{
				for(int j = 2; j < BLOCKWIDTH + 2; ++j)
				{
					North = (block_image[i][j - 1] + block_image[i - 1][j - 1]) / 2;
					NW = block_image[i - 1][j];
					W = (block_image[i + 1][j - 1] + block_image[i][j - 1]) / 2;
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre8[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre8[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		default:
			break;
    }
}
void predict_four_para(short block_image[8][8], short resi[4][4], double **para, int mode)
{
	// 图像残差块、变换量化后的图像块、的内存空间分配
	short block_resi[4][4] = {0};          	   //每一个小块的残差值
	short outdataDct_Quant[4][4] = {0};		  //每一个小块的变换量化后的值
	short block_construct[4][4] = {0};		 //每一个小块的重建值
	long long resi_energy = 0;					 // 所有已处理块的累计残差能量
	long long energy_temp = 0;                   //每一个小块的残差能量值
	short resi_temp[4][4] = {0};                 //块残差数据                 

	static short u[5] = {0};
	switch (mode)
	{
	case 0: case 1: case 2:
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 2][j];
				block_012pre[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 3:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j];
				u[3] = block_image[i - 1][j + 1];
				u[4] = block_image[i - 1][j + 2];
				block_pre3[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 4:
		//4模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 1][j - 2];
				block_pre4[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 5:
		//5模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = (block_image[i - 2][j - 2] + block_image[i - 1][j - 2]) / 2;  //自己构造
				block_pre5[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre5[i - 2][j - 2];//预测残差
				//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
				//resi_energy += energy_temp;
				//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			}
		}
		break;
	case 6:
		//6模式可以用下面公式计算
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 1][j - 2];  //可自己构造
				block_pre6[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 7:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 1][j + 1];  //可自己构造
				block_pre7[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 8:
		for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
		{
			for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			{
				u[1] = block_image[i - 1][j];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i][j - 1];
				u[4] = block_image[i + 1][j - 1];  //可自己构造
				block_pre8[i - 2][j - 2] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre8[i - 2][j - 2];//预测残差
				//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
				//resi_energy += energy_temp;
				//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			}
		}
		break;
	default:
		break;
	}
}
			
			//for(int i = 2; i < BLOCKHEIGHT + 2; ++i)
			//{
			//	for(int j = 2; j < BLOCKWIDTH + 2; ++j)
			//	{
			//		short North, NW, W;
			//		switch (mode)
			//		{
			//		case 0: case 1: case 2:
			//			North = image_mode[i_row][i_col][i - 1][j];
			//			NW = image_mode[i_row][i_col][i - 1][j - 1];
			//			W  = image_mode[i_row][i_col][i][j - 1];
			//			break;
			//		case 3:
			//			North = (image_mode[i_row][i_col][i - 1][j] + 2 * image_mode[i_row][i_col][i - 1][j + 1] + image_mode[i_row][i_col][i - 1][j + 2]) / 4;
			//			NW = image_mode[i_row][i_col][i - 1][j];
			//			W  = image_mode[i_row][i_col][i][j - 1];
			//			break;
			//		case 4:
			//			North = image_mode[i_row][i_col][i - 1][j - 2];
			//			NW = (image_mode[i_row][i_col][i - 1][j - 2] + 2 * image_mode[i_row][i_col][i - 1][j - 1] + image_mode[i_row][i_col][i - 1][j]) / 4;
			//			W  = image_mode[i_row][i_col][i][j - 1];
			//			break;	
			//		case 5:
			//			North = (image_mode[i_row][i_col][i - 1][j - 2] + image_mode[i_row][i_col][i - 1][j - 1]) / 2;
			//			NW = (image_mode[i_row][i_col][i - 1][j - 1] + image_mode[i_row][i_col][i - 1][j]) / 2;
			//			W  = image_mode[i_row][i_col][i][j - 1];
			//			break;	
			//		case 6:
			//			North = (image_mode[i_row][i_col][i - 1][j - 1] + image_mode[i_row][i_col][i - 2][j - 1]) / 2;
			//			NW = image_mode[i_row][i_col][i - 1][j];
			//			W = (image_mode[i_row][i_col][i][j - 1] + image_mode[i_row][i_col][i - 1][j - 1]) / 2;
			//			break;	
			//		case 7:
			//			North = (image_mode[i_row][i_col][i - 1][j - 1] + image_mode[i_row][i_col][i - 1][j]) / 2;
			//			NW = (image_mode[i_row][i_col][i - 1][j] + image_mode[i_row][i_col][i - 1][j + 1]) / 2;
			//			W = image_mode[i_row][i_col][i][j - 1];
			//			break;	
			//		case 8:
			//			North = (image_mode[i_row][i_col][i][j - 1] + image_mode[i_row][i_col][i - 1][j - 1]) / 2;
			//			NW = image_mode[i_row][i_col][i - 1][j];
			//			W = (image_mode[i_row][i_col][i + 1][j - 1] + image_mode[i_row][i_col][i][j - 1]) / 2;
			//			break;	
			//		default:
			//			break;
			//		}

			//		block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
			//		resi_temp[i - 2][j - 2] = image_mode[i_row][i_col][i][j] - block_pre[i - 2][j - 2];//预测残差
			//		energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
			//		resi_energy += energy_temp;
			//		printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			//	}
			//}
			//printf("Accumulated energe [%4d][%4d]  %I64d\n", i_row, i_col, resi_energy);
			////将当前块的预测值保存到整幅图像的预测矩阵中
			//for (int i = 0; i < BLOCKHEIGHT; ++i)
			//{
			//	for (int j = 0; j < BLOCKWIDTH; ++j)
			//	{
			//		//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	严重错误！！！！
			//		predicted[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_pre[i][j];
			//	}
			//}
			////将当前块的残差值保存到整幅图像的残差矩阵中
			//for (int i = 0; i < BLOCKHEIGHT; ++i)
			//{
			//	for (int j = 0; j < BLOCKWIDTH; ++j)
			//	{
			//		//  resi[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j];	严重错误！！！！
			//		resi[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = resi_temp[i][j];
			//	}
			//}
			//// 对得到的最优的残差块进行变换、量化、逆量化、逆变换，
			///**************************************************************
	  //       * 3. transform and quant the residual of prediction
	  //       *************************************************************/
	  //
			//DCT_Quanter(resi_temp, outdataDct_Quant);
			////DCT_Quanter后的结果与预测值进行相加得到重建块，重建块的值拷贝到image_construct中当前块对应位置处，覆盖掉原始图像，
			////为下一次循环读取下一个块的邻近重建像素值做准备
			//for (int i = 0; i < BLOCKHEIGHT; ++i)
			//{
			//	for (int j = 0; j < BLOCKWIDTH; ++j)
			//	{
			//		//block_construct[i_row][i_col][i][j] = block_resi[i_row][i_col][i][j] + outdataDct_Quant[i_row][i_col][i][j];
			//		//image_construct[i_row][i_col][i + 1][j + 1] = block_construct[i_row][i_col][i][j]; 严重错误！！中间状态不需要索引块坐标，只需要它的值
			//		block_construct[i][j] = resi_temp[i][j] + outdataDct_Quant[i][j];
			//		image_construct[i_row * BLOCKHEIGHT + i][i_col * BLOCKWIDTH + j] = block_construct[i][j];

			//	}
			//}
//}
