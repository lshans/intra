#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include "pre.h"
#include "common.h"
void predict_one_para(short block_image[10][10], short resi[4][4], double **para, int mode)
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
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i - 1][j] + block_image[i - 2][j]) / 2;
				block_012pre[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 1: 
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i][j - 1] + block_image[i][j - 2]) / 2;
				block_012pre[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 2:
		//0,1,4,5,6模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i - 1][j - 1] + block_image[i - 1][j] + block_image[i][j - 1]) / 3;
				block_012pre[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 3:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2] + 2) / 4;
				block_pre3[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 4:
		//4模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + 2 * block_image[i - 1][j] + 2) / 4;
				block_pre4[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 5:
		//5模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i - 1][j] + block_image[i - 1][j - 1] + 1) / 2;
				block_pre5[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 6:
		//6模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i][j - 1] + block_image[i - 1][j - 1] + 1) / 2;
				block_pre6[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 7:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i - 1][j] + block_image[i - 1][j + 1] + 1) / 2;   
				block_pre7[i - 3][j - 3] = para[0][0] * u[1];
			}
		}
		break;
	case 8:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = (block_image[i][j - 1] + block_image[i + 1][j - 1] + 1) / 2;//可自己构造，与模式6关于x轴对称
				block_pre8[i - 3][j - 3] = para[0][0] * u[1];
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
void predict_two_para(short block_image[10][10], short resi[4][4], double **para, int mode)
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
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i - 1][j];
				u[2] = block_image[i - 2][j];
				block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 1:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 2];
				u[2] = block_image[i][j - 1];
				block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 2:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = (block_image[i - 1][j - 1] + 2 * block_image[i - 1][j] + block_image[i - 2][j]) / 4;
				block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 3:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + 2 * block_image[i - 1][j + 2]) / 5;
				block_pre3[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 4:
		//4模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];//可自己构造，与模式3对称
				u[2] = (2 * block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 5;
				block_pre4[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 5:
		//5模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				//u[1] = block_image[i][j - 1];
				//u[2] = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 4;  //自己构造
				u[1] = block_image[i - 1][j - 1];
				u[2] = block_image[i - 1][j];
				block_pre5[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 6:
		//6模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				block_pre6[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 7:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
// 				u[1] = block_image[i][j - 1];//可自己构造，与模式5对称
// 				u[2] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
				u[1] = block_image[i -1][j];
				u[2] = block_image[i - 1][j + 1];
				block_pre7[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
			}
		}
		break;
	case 8:
		for(int j = 3; j < BLOCKWIDTH + 3; ++j)
		{
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				u[1] = block_image[i + 1][j - 1];
				u[2] = block_image[i][j - 1];//可自己构造，与模式6关于x轴对称
				block_pre8[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2];
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
void predict_three_para(short block_image[10][10], short resi[4][4], double **para, int mode)
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
		case 0:
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					North = block_image[i - 1][j + 1];
					NW = block_image[i - 1][j];
					W  = block_image[i - 1][j - 1];
					block_012pre[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
				}
			}
			break;
		case 1:
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					North = block_image[i - 1][j - 1];
					NW = block_image[i][j - 1];
					W  = block_image[i + 1][j - 1];
					block_012pre[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
				}
			}
			break;
		case 2:
			//0,1,4,5,6模式可以用下面公式计算
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					North = block_image[i - 1][j];
					NW = block_image[i - 1][j - 1];
					W  = block_image[i][j - 1];
// 					block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
// 					resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差
					block_012pre[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre012[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];

					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 3:
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
// 					North = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
// 					NW = block_image[i - 1][j];
// 					W  = block_image[i][j - 1];
					W = block_image[i - 1][j];
					North = block_image[i - 1][j + 2];
					NW = block_image[i - 1][j + 1];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差
					block_pre3[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre3[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 4:
			//4模式可以用下面公式计算
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
// 					North = block_image[i - 1][j - 2];
// 					NW = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 4;
// 					W  = block_image[i][j - 1];
					North = block_image[i - 1][j];
					NW    =  block_image[i - 1][j - 1];
					W     =  block_image[i - 1][j - 2];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre4[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre4[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 5:
			//5模式可以用下面公式计算
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
// 					North = (block_image[i - 1][j - 2] + block_image[i - 1][j - 1]) / 2;
// 					NW = (block_image[i - 1][j - 1] + block_image[i - 1][j]) / 2;
// 					W  = block_image[i][j - 1];
					North = block_image[i - 2][j];
					NW    = block_image[i - 2][j - 1];
					W     = block_image[i - 2][j - 2];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差
					block_pre5[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre5[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 6:
			//6模式可以用下面公式计算
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
// 					North = (block_image[i - 1][j - 1] + block_image[i - 2][j - 1]) / 2;
// 					NW = block_image[i - 1][j];
// 					W = (block_image[i][j - 1] + block_image[i - 1][j - 1]) / 2;
					North = block_image[i - 1][j - 1];
					NW    = block_image[i - 1][j - 2];
					W     = block_image[i - 1][j - 3];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre6[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre6[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 7:
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
// 					North = (block_image[i - 1][j - 1] + block_image[i - 1][j]) / 2;
// 					NW = (block_image[i - 1][j] + block_image[i - 1][j + 1]) / 2;
// 					W = block_image[i][j - 1];
					North = block_image[i - 2][j + 2];
					NW    = block_image[i - 2][j + 1];
					W     = block_image[i - 2][j];
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre7[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre7[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 8:
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					North = (block_image[i][j - 1] + block_image[i - 1][j - 1]) / 2;
					NW = block_image[i - 1][j];
					W = (block_image[i + 1][j - 1] + block_image[i][j - 1]) / 2;
					//block_pre[i - 2][j - 2] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre[i - 2][j - 2];//预测残差

					block_pre8[i - 3][j - 3] = para[0][0] * W + para[1][0]* NW + para[2][0] * North;
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

/****************备份楔形滤波器的模式与角度对应关系!!!!!!!!!!!!!!!!!!!!******************************/
void predict_four_para(short block_image[10][10], short resi[4][4], double **para, int mode)
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
		case 1:   //水平  以负方向x轴为基准，逆时针旋转22.5度
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i][j - 1];
					u[2] = block_image[i][j - 2];
					u[3] = block_image[i][j - 3];
					u[4] = block_image[i - 1][j - 2];
					block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		case 2:  //22.5度
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i - 1][j];
					u[2] = block_image[i - 1][j + 1];
					u[3] = block_image[i - 1][j + 2];
					u[4] = block_image[i - 2][j + 2];
					block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		case 3:  //45度
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i - 1][j];
					u[2] = block_image[i - 1][j + 1];
					u[3] = block_image[i - 2][j + 1];
					u[4] = block_image[i - 2][j + 2];
					block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		case 4:   //67.5度
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i - 1][j];
					u[2] = block_image[i - 1][j + 1];
					u[3] = block_image[i - 2][j];
					u[4] = block_image[i - 2][j + 1];
					block_pre3[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		case 5:    //90度
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i - 1][j - 1];
					u[2] = block_image[i - 1][j];
					u[3] = block_image[i - 1][j + 1];
					u[4] = block_image[i - 2][j];
					block_pre4[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		case 6:    //112.5
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i - 1][j - 1];
					u[2] = block_image[i - 1][j];
					u[3] = block_image[i - 2][j - 1];
					u[4] = block_image[i - 2][j];  //自己构造
					block_pre5[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
					//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre5[i - 2][j - 2];//预测残差
					//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
					//resi_energy += energy_temp;
					//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
				}
			}
			break;
		case 7:   //134.5度
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i][j - 1];
					u[2] = block_image[i - 1][j - 1];
					u[3] = block_image[i - 1][j];
					u[4] = block_image[i - 2][j - 2];  //可自己构造
					block_pre6[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		case 8:   //
			for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
			{
				for(int j = 3; j < BLOCKWIDTH + 3; ++j)
				{
					u[1] = block_image[i][j - 1];
					u[2] = block_image[i - 1][j - 2];
					u[3] = block_image[i - 1][j - 1];
					u[4] = block_image[i - 1][j];  //可自己构造
					block_pre7[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				}
			}
			break;
		default:
			break;
	}
}
/****************备份楔形滤波器的模式与角度对应关系!!!!!!!!!!!!!!!!!!!!******************************/

/****************备份原来的模式与角度对应关系!!!!!!!!!!!!!!!!!!!!******************************
void predict_four_para(short block_image[10][10], short resi[4][4], double **para, int mode)
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
	case 0: 
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i - 1][j - 1];
				u[2] = block_image[i - 1][j];
				u[3] = block_image[i - 1][j + 1];
				u[4] = block_image[i - 1][j + 2];
				block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 1: 
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i - 2][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i][j - 1];
				u[4] = block_image[i + 1][j - 1];
				block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 2:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 2][j];
				block_012pre[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 3:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j];
				u[3] = block_image[i - 1][j + 1];
				u[4] = block_image[i - 1][j + 2];
				block_pre3[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 4:
		//4模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 1][j - 2];
				block_pre4[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 5:
		//5模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = (block_image[i - 2][j - 2] + block_image[i - 1][j - 2]) / 2;  //自己构造
				block_pre5[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre5[i - 2][j - 2];//预测残差
				//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
				//resi_energy += energy_temp;
				//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			}
		}
		break;
	case 6:
		//6模式可以用下面公式计算
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 1][j - 2];  //可自己构造
				block_pre6[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 7:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i][j - 1];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i - 1][j];
				u[4] = block_image[i - 1][j + 1];  //可自己构造
				block_pre7[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
			}
		}
		break;
	case 8:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i - 1][j];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i][j - 1];
				u[4] = block_image[i + 1][j - 1];  //可自己构造
				block_pre8[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
				//resi_temp[i - 2][j - 2] = block_image[i][j] - block_pre8[i - 2][j - 2];//预测残差
				//energy_temp = resi_temp[i - 2][j - 2] * resi_temp[i - 2][j - 2];
				//resi_energy += energy_temp;
				//printf("energe [%4d][%4d]  %I64d\n", i_row, i_col, energy_temp);
			}
		}
		break;
	case 9:
		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		{
			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			{
				u[1] = block_image[i - 1][j];
				u[2] = block_image[i - 1][j - 1];
				u[3] = block_image[i][j - 1];
				u[4] = block_image[i + 1][j - 1];  //可自己构造
				block_pre8[i - 3][j - 3] = para[0][0] * u[1] + para[1][0]* u[2] + para[2][0] * u[3] + para[3][0] * u[4];
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
****************备份原来的模式与角度对应关系!!!!!!!!!!!!!!!!!!!!******************************/