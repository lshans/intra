#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <iomanip>
#include <iostream>
#include "pre.h"
#include "common.h"
#include <cassert>

const int N = 3;
using namespace std; 

// multiply two mat
// D:		input 1st mat, H1 x K1
// f:		input 2st mat, K1 x W1
// temp:	otuput mat, H1 x W1
void Mult(double **mat1, double **mat2,double **resMat, int H1, int W1, int K1)	//DCT正变换  H1 * K1 | K1 * W1
{
	double sum = 0;
	for(int i = 0; i < H1; i++)
	{
		for(int j = 0; j < W1; j++)
		{
			sum = 0;
			for(int k = 0;k < K1; k++)
				sum += mat1[i][k] * mat2[k][j];
			resMat[i][j] = sum;
			//printf("%f", resMat[i][j]); 
		}
	}
}

//------------------------------------------------------------------  
//功能: 采用部分主元的高斯消去法求方阵A的逆矩阵B  
//入口参数: 输入方阵，输出方阵，方阵阶数  
//返回值: true or false  
//-------------------------------------------------------------------  
bool Gauss(double **A, double **B, int n)  
{  
	int i, j, k;  
	double max, temp; 
	double **t = NULL;
	t = (double **)calloc(10, sizeof(double *));
	for(int i = 0; i < 10; i++)
		t[i] = (double *)calloc(10,sizeof(double));
	//将A矩阵存放在临时矩阵t[n][n]中
	for (i = 0; i < n; i++)  
	{  
		for (j = 0; j < n; j++)  
		{  
			t[i][j] = A[i][j];  
		}  
	}  
	//初始化B矩阵为单位阵  
	for (i = 0; i < n; i++)  
	{  
		for (j = 0; j < n; j++)  
		{  
			B[i][j] = (i == j) ? (double)1 : 0;  
		}  
	}  
	for (i = 0; i < n; i++)  
	{  
		//寻找主元  
		max = t[i][i];  
		k = i;  
		for (j = i + 1; j < n; j++)  
		{  
			if (fabs(t[j][i]) > fabs(max))  
			{  
				max = t[j][i];  
				k = j;  
			}  
		}  
		//如果主元所在行不是第i行，进行行交换  
		if (k != i)  
		{  
			for (j = 0; j < n; j++)  
			{  
				temp = t[i][j];  
				t[i][j] = t[k][j];  
				t[k][j] = temp;  
				//B伴随交换  
				temp = B[i][j];  
				B[i][j] = B[k][j];  
				B[k][j] = temp;  
			}  
		}  
		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵  
		if (t[i][i] == 0)  
		{  
			cout << "There is no inverse matrix!";  
			return false;  
		}  
		//消去A的第i列除去i行以外的各行元素  
		temp = t[i][i];  
		for (j = 0; j < n; j++)  
		{  
			t[i][j] = t[i][j] / temp;        //主对角线上的元素变为1  
			B[i][j] = B[i][j] / temp;        //伴随计算  
		}  
		for (j = 0; j < n; j++)        //第0行->第n行  
		{  
			if (j != i)                //不是第i行  
			{  
				temp = t[j][i];  
				for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素  
				{  
					t[j][k] = t[j][k] - t[i][k] * temp;  
					B[j][k] = B[j][k] - B[i][k] * temp;  
				}  
			}  
		}  
	} 
	for(int i = 0; i < 10; i++)  
		free(t[i]);  
	free(t);
	return true;  
} 
void AccumulateParaAB_one_para(double **paraA, double **paraB, short block_image[10][10], int i, int j, int mode)
{   //需要调整多个块累加的程序。。。。??
	static short u[2] = {0};	// u[y][x] equals to u(x, y) y equals i, x equals j
	switch (mode)
	{
	case 0: 
		u[1] = (block_image[i - 1][j] + block_image[i - 2][j]) / 2;  //相当于H.264的模式0（用每列上方的像素对列进行覆盖）
		break;
	case 1: 
		u[1] = (block_image[i][j - 1] + block_image[i][j - 2]) / 2;  //相当于H.264的模式1（用每行左方的像素对行进行覆盖）
		break;
	case 2:
		u[1] = (block_image[i - 1][j - 1] + block_image[i - 1][j] + block_image[i][j - 1]) / 3;
		break;
	case 3:
		u[1] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2] + 2) / 4;
		break;
	case 4:
		u[1] = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + 2 * block_image[i - 1][j] + 2) / 4;
		break;	
	case 5:
		u[1] = (block_image[i - 1][j] + block_image[i - 1][j - 1] + 1) / 2;
		break;	
	case 6:
		u[1] = (block_image[i][j - 1] + block_image[i - 1][j - 1] + 1) / 2;
		break;	
	case 7:
		u[1] = (block_image[i - 1][j] + block_image[i - 1][j + 1] + 1) / 2;   
		break;	
	case 8:
		u[1] = (block_image[i][j - 1] + block_image[i + 1][j - 1] + 1) / 2;
		break;	
	default:
		break;
	}
	u[0] = block_image[i][j];

	paraA[0][0] += u[1] * u[1];
	paraB[0][0] += u[0] * u[1];
	
}
void estimate_one_para(short block_image[10][10], double **para, int mode)
{

	double **paraA = NULL;
	double **paraB = NULL;
	double **result = NULL;
	short pixAverage = 0;              //当前块像素的平均值
	short pixSum = 0;              //像素的总和

	paraA  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		paraA[i] = (double *)calloc(4,sizeof(double));
	}

	result  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		result[i] = (double *)calloc(4,sizeof(double));
	}

	paraB  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		paraB[i] = (double *)calloc(1,sizeof(double));
	}

	for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			AccumulateParaAB_one_para(paraA, paraB, block_image, i, j, mode);



	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraA[0][0] = 1;
	//		paraA[0][1] = 1;
	//		paraA[0][2] = 2;
	//		paraA[1][0] = -1;
	//		paraA[1][1] = 2;
	//		paraA[1][2] = 0;
	//		paraA[2][0] = 1;
	//		paraA[2][1] = 1;
	//		paraA[2][2] = 3;
	//	}
	//}
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraB[0][0] = 1;
	//		paraB[1][0] = 0;
	//		paraB[2][0] = 0;
	//	}
	//}

	if (Gauss(paraA, result, 1))  
	{  
		//printf("Matrix A inve is\n");
		//for(int i = 0; i < 1; ++i)
		//{
		//	for(int j = 0; j < 1; ++j)
		//	{
		//		printf("%.15lf ", result[i][j]); 
		//	}
		//	printf("\n");
		//}
	} 
	// 矩阵求逆l
	//double *res_temp = MatrixOpp((double *)paraA, 3, 3); 
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		printf("%d\n", paraA[i][j]);
	//	}
	//}

	//long long a = getA(paraA, 3);  
	//if(a==0)  
	//{  
	//	printf("can not transform!\n");  
	//}  
	//else  
	//{  
	//	getAStart(paraA, 3, result);  
	//	for(int i=0;i<3;i++)  
	//	{  
	//		for(int j=0;j<3;j++)  
	//		{  
	//			printf("%.3lf ",(double)result[i][j]/a);  
	//		}  
	//		printf("\n");  
	//	}  
	//}


	//for(int i = 0; i < 9; i++)
	//{
	//		printf("%lf\n", res_temp[i]);
	//
	//}
	//
	//
	//for (int i = 0; i < 3; ++i)
	//	for (int j = 0; j < 3; ++j)
	//		result[i][j] = res_temp[i * 3 + j];
	////????
	//free(res_temp);
	//
	//{
	//	FILE *fout = fopen("res.txt", "w");
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//			fprintf(fout, "%6.2f, ", result[i][j]);
	//		fprintf(fout, "\n");
	//	}
	//	fclose(fout);
	//}


	//矩阵result 与paraB相乘
	Mult(result, paraB, para, 1, 1, 1);


	//for (int i = 0; i < height + 4; ++i)
	//	free(matrix_in[i]);
	//free(matrix_in);

	for (int i = 0; i < 4; ++i)
	{
		free(result[i]);
		free(paraA[i]);
		free(paraB[i]);
	}
	free(result);
	free(paraA);
	free(paraB);
}
void AccumulateParaAB_two_para(double **paraA, double **paraB, short block_image[10][10], int i, int j, int mode)
{   //需要调整多个块累加的程序。。。。??
	static short u[3] = {0};	// u[y][x] equals to u(x, y) y equals i, x equals j
	switch (mode)
	{
	case 0: 
		u[1] = block_image[i - 1][j];
		u[2] = block_image[i - 2][j];
	case 1:
		u[1] = block_image[i][j - 2];
		u[2] = block_image[i][j - 1];
	case 2:
		u[1] = block_image[i][j - 1];
		u[2] = (block_image[i - 1][j - 1] + 2 * block_image[i - 1][j] + block_image[i - 2][j]) / 4;
		break;
	case 3:
		u[1] = block_image[i][j - 1];
		u[2] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + 2 * block_image[i - 1][j + 2]) / 5;
		break;
	case 4:
		u[1] = block_image[i][j - 1];//可自己构造，与模式3对称
		u[2] = (2 * block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 5;
		break;	
	case 5:
		//u[1] = block_image[i][j - 1];
		//u[2] = (block_image[i - 1][j - 2] + 2 * block_image[i - 1][j - 1] + block_image[i - 1][j]) / 4;  //自己构造
		u[1] = block_image[i - 1][j - 1];
		u[2] = block_image[i - 1][j];
		break;	
	case 6:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j - 1];//可自己构造
		break;	
	case 7:
		//u[1] = block_image[i][j - 1];//可自己构造，与模式5对称
		//u[2] = (block_image[i - 1][j] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
		u[1] = block_image[i -1][j];
		u[2] = block_image[i - 1][j + 1];
		break;	
	case 8:
// 		u[1] = (block_image[i][j - 1] + 2 * block_image[i - 1][j + 1] + block_image[i - 1][j + 2]) / 4;
// 		u[2] = block_image[i - 1][j];//可自己构造，与模式6关于x轴对称。有问题？？？？
		u[1] = block_image[i + 1][j - 1];
		u[2] = block_image[i][j - 1];
		break;	
	default:
		break;
	}
	u[0] = block_image[i][j];

	paraA[0][0] += u[1] * u[1];
	paraA[0][1] += u[1] * u[2];
	paraA[1][0] += u[1] * u[2];
	paraA[1][1] += u[2] * u[2];

	paraB[0][0] += u[0] * u[1];
	paraB[1][0] += u[0] * u[2];
}
void estimate_two_para(short block_image[10][10], double **para, int mode)
{

	double **paraA = NULL;
	double **paraB = NULL;
	double **result = NULL;
	short pixAverage = 0;              //当前块像素的平均值
	short pixSum = 0;              //像素的总和

	paraA  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		paraA[i] = (double *)calloc(4,sizeof(double));
	}

	result  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		result[i] = (double *)calloc(4,sizeof(double));
	}

	paraB  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		paraB[i] = (double *)calloc(1,sizeof(double));
	}

	for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			AccumulateParaAB_two_para(paraA, paraB, block_image, i, j, mode);

	//printf("para end \n");


	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraA[0][0] = 1;
	//		paraA[0][1] = 1;
	//		paraA[0][2] = 2;
	//		paraA[1][0] = -1;
	//		paraA[1][1] = 2;
	//		paraA[1][2] = 0;
	//		paraA[2][0] = 1;
	//		paraA[2][1] = 1;
	//		paraA[2][2] = 3;
	//	}
	//}
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraB[0][0] = 1;
	//		paraB[1][0] = 0;
	//		paraB[2][0] = 0;
	//	}
	//}

	if (Gauss(paraA, result, 2))  
	{  
		//printf("Matrix A inve is\n");
		//for(int i = 0; i < 2; ++i)
		//{
		//	for(int j = 0; j < 2; ++j)
		//	{
		//		printf("%.15lf ", result[i][j]); 
		//	}
		//	printf("\n");
		//}
	} 
	// 矩阵求逆l
	//double *res_temp = MatrixOpp((double *)paraA, 3, 3); 
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		printf("%d\n", paraA[i][j]);
	//	}
	//}

	//long long a = getA(paraA, 3);  
	//if(a==0)  
	//{  
	//	printf("can not transform!\n");  
	//}  
	//else  
	//{  
	//	getAStart(paraA, 3, result);  
	//	for(int i=0;i<3;i++)  
	//	{  
	//		for(int j=0;j<3;j++)  
	//		{  
	//			printf("%.3lf ",(double)result[i][j]/a);  
	//		}  
	//		printf("\n");  
	//	}  
	//}


	//for(int i = 0; i < 9; i++)
	//{
	//		printf("%lf\n", res_temp[i]);
	//
	//}
	//
	//
	//for (int i = 0; i < 3; ++i)
	//	for (int j = 0; j < 3; ++j)
	//		result[i][j] = res_temp[i * 3 + j];
	////????
	//free(res_temp);
	//
	//{
	//	FILE *fout = fopen("res.txt", "w");
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//			fprintf(fout, "%6.2f, ", result[i][j]);
	//		fprintf(fout, "\n");
	//	}
	//	fclose(fout);
	//}


	//矩阵result 与paraB相乘
	Mult(result, paraB, para, 2, 1, 2);


	//for (int i = 0; i < height + 4; ++i)
	//	free(matrix_in[i]);
	//free(matrix_in);

	for (int i = 0; i < 4; ++i)
	{
		free(result[i]);
		free(paraA[i]);
		free(paraB[i]);
	}
	free(result);
	free(paraA);
	free(paraB);
}
void AccumulateParaAB_three_para(double **paraA, double **paraB, short block_image[10][10], int x, int y, int mode)
{   //需要调整多个块累加的程序。。。。??
	static short u[2][2] = {0};	// u[y][x] equals to u(x, y)
	switch (mode)
	{
	case 0:
		u[1][0] = block_image[y - 1][x + 1];
		u[0][0] = block_image[y - 1][x];
		u[1][0] = block_image[y - 1][x - 1];
	case 1:
		u[1][0] = block_image[y + 1][x - 1];
		u[0][1] = block_image[y - 1][x - 1];
		u[0][0] = block_image[y][x - 1];
	case 2:
		u[1][0] = block_image[y][x - 1];
		u[0][1] = block_image[y - 1][x];
		u[0][0] = block_image[y - 1][x - 1];
		break;
	case 3:
// 		u[1][0] = block_image[y][x - 1];
// 		u[0][1] = (block_image[y - 1][x] + 2 * block_image[y - 1][x + 1] + block_image[y - 1][x + 2]) / 4;
// 		u[0][0] = block_image[y - 1][x];
		u[1][0] = block_image[y - 1][x];
		u[0][1] = block_image[y - 1][x + 2];
		u[0][0] = block_image[y - 1][x + 1];
		break;
	case 4:
// 		u[1][0] = block_image[y][x - 1];
// 		u[0][1] = (block_image[y - 1][x - 2] + 2 * block_image[y - 1][x - 1] + block_image[y - 1][x]) / 4;
// 		u[0][0] = block_image[y - 1][x - 2];
		u[1][0] = block_image[y - 1][x - 2];
		u[0][1] = block_image[y - 1][x];
		u[0][0] = block_image[y - 1][x - 1];
		break;	
	case 5:
// 		u[1][0] = block_image[y][x - 1];
// 		u[0][1] = (block_image[y - 1][x - 1] + block_image[y - 1][x]) / 2;
// 		u[0][0] = (block_image[y - 1][x - 2] + block_image[y - 1][x - 1]) / 2;
		u[1][0] = block_image[y - 2][x - 2];
		u[0][1] = block_image[y - 2][x];
		u[0][0] = block_image[y - 2][x - 1];
		break;	
	case 6:
// 		u[1][0] = (block_image[y][x - 1] + block_image[y - 1][x - 1]) / 2;
// 		u[0][1] = block_image[y - 1][x];
// 		u[0][0] = (block_image[y - 1][x - 1] + block_image[y - 2][x - 1]) / 2;
		u[1][0] = block_image[y - 1][x - 3];
		u[0][1] = block_image[y - 1][x - 1];
		u[0][0] = block_image[y - 1][x - 2];
		break;	
	case 7:
// 		u[1][0] = block_image[y][x - 1];
// 		u[0][1] = (block_image[y - 1][x] + block_image[y - 1][x + 1]) / 2;
// 		u[0][0] = (block_image[y - 1][x - 1] + block_image[y - 1][x]) / 2;
		u[1][0] = block_image[y - 2][x];
		u[0][1] = block_image[y - 2][x + 2];
		u[0][0] = block_image[y - 2][x + 1];
		break;	
	case 8:
		u[1][0] = (block_image[y + 1][x - 1] + block_image[y][x - 1]) / 2;
		u[0][1] = block_image[y - 1][x];
		u[0][0] = (block_image[y][x - 1] + block_image[y - 1][x - 1]) / 2;
		break;	
	default:
		break;
	}
	u[1][1] = block_image[y][x];

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
void estimate_three_para(short block_image[10][10], double **para, int mode)
{

	double **paraA = NULL;
	double **paraB = NULL;
	double **result = NULL;
	short pixAverage = 0;              //当前块像素的平均值
	short pixSum = 0;              //像素的总和

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

	for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			AccumulateParaAB_three_para(paraA, paraB, block_image, j, i, mode);

	//printf("para end \n");


	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraA[0][0] = 1;
	//		paraA[0][1] = 1;
	//		paraA[0][2] = 2;
	//		paraA[1][0] = -1;
	//		paraA[1][1] = 2;
	//		paraA[1][2] = 0;
	//		paraA[2][0] = 1;
	//		paraA[2][1] = 1;
	//		paraA[2][2] = 3;
	//	}
	//}
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraB[0][0] = 1;
	//		paraB[1][0] = 0;
	//		paraB[2][0] = 0;
	//	}
	//}

	if (Gauss(paraA, result, 3))  
	{  
		//printf("Matrix A inve is\n");
		//for(int i = 0; i < 3; ++i)
		//{
		//	for(int j = 0; j < 3; ++j)
		//	{
		//		printf("%.31f ", result[i][j]); 
		//	}
		//printf("\n");
		//}
	} 
	// 矩阵求逆l
	//double *res_temp = MatrixOpp((double *)paraA, 3, 3); 
		//for(int i = 0; i < 3; i++)
		//{
		//	for(int j = 0; j < 3; ++j)
		//	{
		//		printf("%d\n", paraA[i][j]);
		//	}
		//}

		//long long a = getA(paraA, 3);  
		//if(a==0)  
		//{  
		//	printf("can not transform!\n");  
		//}  
		//else  
		//{  
		//	getAStart(paraA, 3, result);  
		//	for(int i=0;i<3;i++)  
		//	{  
		//		for(int j=0;j<3;j++)  
		//		{  
		//			printf("%.3lf ",(double)result[i][j]/a);  
		//		}  
		//		printf("\n");  
		//	}  
		//}


	//for(int i = 0; i < 9; i++)
	//{
	//		printf("%lf\n", res_temp[i]);
	//
	//}
	//
	//
	//for (int i = 0; i < 3; ++i)
	//	for (int j = 0; j < 3; ++j)
	//		result[i][j] = res_temp[i * 3 + j];
	////????
	//free(res_temp);
	//
	//{
	//	FILE *fout = fopen("res.txt", "w");
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//			fprintf(fout, "%6.2f, ", result[i][j]);
	//		fprintf(fout, "\n");
	//	}
	//	fclose(fout);
	//}


	//矩阵result 与paraB相乘
	Mult(result, paraB, para, 3, 1, 3);


	//for (int i = 0; i < height + 4; ++i)
	//	free(matrix_in[i]);
	//free(matrix_in);

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

/****************备份原来的模式与角度对应关系!!!!!!!!!!!!!!!!!!!!******************************/
void AccumulateParaAB_four_para(double **paraA, double **paraB, short block_image[10][10], int i, int j, int mode)
{   //需要调整多个块累加的程序。。。。??
	static short u[5] = {0};	// u[y][x] equals to u(x, y) y equals i, x equals j
	switch (mode)
	{
	case 0:
		u[1] = block_image[i - 1][j - 1];
		u[2] = block_image[i - 1][j];
		u[3] = block_image[i - 1][j + 1];
		u[4] = block_image[i - 1][j + 2];
		break;
	case 1:
		u[1] = block_image[i - 2][j - 1];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i][j - 1];
		u[4] = block_image[i + 1][j - 1];
		break;
	case 2:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i - 1][j];
		u[4] = block_image[i - 2][j];
		break;
	case 3:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j];
		u[3] = block_image[i - 1][j + 1];
		u[4] = block_image[i - 1][j + 2];
		break;
	case 4:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i - 1][j];
		u[4] = block_image[i - 1][j - 2];
		break;	
	case 5:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i - 1][j];
		u[4] = (block_image[i - 2][j - 2] + block_image[i - 1][j - 2]) / 2;  //自己构造
		break;	
	case 6:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i - 1][j];
		u[4] = block_image[i - 1][j - 2];  //可自己构造
		break;	
	case 7:
		u[1] = block_image[i][j - 1];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i - 1][j];
		u[4] = block_image[i - 1][j + 1];  //可自己构造
		break;	
	case 8:
		u[1] = block_image[i - 1][j];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i][j - 1];
		u[4] = block_image[i + 1][j - 1];  //可自己构造
		break;
	case 9:
		u[1] = block_image[i - 1][j];
		u[2] = block_image[i - 1][j - 1];
		u[3] = block_image[i][j - 1];
		u[4] = block_image[i + 1][j - 1];  //可自己构造
		break;
	default:
		break;
	}
	u[0] = block_image[i][j];

	paraA[0][0] += u[1] * u[1];
	paraA[0][1] += u[1] * u[2];
	paraA[0][2] += u[1] * u[3];
	paraA[0][3] += u[1] * u[4];
	paraA[1][0] += u[1] * u[2];
	paraA[1][1] += u[2] * u[2];
	paraA[1][2] += u[2] * u[3];
	paraA[1][3] += u[2] * u[4];
	paraA[2][0] += u[1] * u[3];
	paraA[2][1] += u[2] * u[3];
	paraA[2][2] += u[3] * u[3];
	paraA[2][3] += u[3] * u[4];
	paraA[3][0] += u[1] * u[4];
	paraA[3][1] += u[2] * u[4];
	paraA[3][2] += u[3] * u[4];
	paraA[3][3] += u[4] * u[4];

	paraB[0][0] += u[0] * u[1];
	paraB[1][0] += u[0] * u[2];
	paraB[2][0] += u[0] * u[3];
	paraB[3][0] += u[0] * u[4];
}
void estimate_four_para(short block_image[10][10], double **para, int mode)
{

	double **paraA = NULL;
	double **paraB = NULL;
	double **result = NULL;
	short pixAverage = 0;              //当前块像素的平均值
	short pixSum = 0;              //像素的总和

	paraA  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		paraA[i] = (double *)calloc(4,sizeof(double));
	}

	result  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		result[i] = (double *)calloc(4,sizeof(double));
	}

	paraB  = (double **)calloc(4, sizeof(double *));
	for(int i = 0; i < 4; i++)
	{
		paraB[i] = (double *)calloc(1,sizeof(double));
	}

	for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
		for(int j = 3; j < BLOCKWIDTH + 3; ++j)
			AccumulateParaAB_four_para(paraA, paraB, block_image, i, j, mode);

	//printf("para end \n");


	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraA[0][0] = 1;
	//		paraA[0][1] = 1;
	//		paraA[0][2] = 2;
	//		paraA[1][0] = -1;
	//		paraA[1][1] = 2;
	//		paraA[1][2] = 0;
	//		paraA[2][0] = 1;
	//		paraA[2][1] = 1;
	//		paraA[2][2] = 3;
	//	}
	//}
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		paraB[0][0] = 1;
	//		paraB[1][0] = 0;
	//		paraB[2][0] = 0;
	//	}
	//}

	if (Gauss(paraA, result, 4))  
	{  
		//printf("Matrix A inve is\n");
		//for(int i = 0; i < 4; ++i)
		//{
		//	for(int j = 0; j < 4; ++j)
		//	{
		//		printf("%.15lf ", result[i][j]); 
		//	}
		//printf("\n");
		//}
	} 
	// 矩阵求逆l
	//double *res_temp = MatrixOpp((double *)paraA, 3, 3); 
	//for(int i = 0; i < 3; i++)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		printf("%d\n", paraA[i][j]);
	//	}
	//}

	//long long a = getA(paraA, 3);  
	//if(a==0)  
	//{  
	//	printf("can not transform!\n");  
	//}  
	//else  
	//{  
	//	getAStart(paraA, 3, result);  
	//	for(int i=0;i<3;i++)  
	//	{  
	//		for(int j=0;j<3;j++)  
	//		{  
	//			printf("%.3lf ",(double)result[i][j]/a);  
	//		}  
	//		printf("\n");  
	//	}  
	//}


	//for(int i = 0; i < 9; i++)
	//{
	//		printf("%lf\n", res_temp[i]);
	//
	//}
	//
	//
	//for (int i = 0; i < 3; ++i)
	//	for (int j = 0; j < 3; ++j)
	//		result[i][j] = res_temp[i * 3 + j];
	////????
	//free(res_temp);
	//
	//{
	//	FILE *fout = fopen("res.txt", "w");
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		for (int j = 0; j < 3; ++j)
	//			fprintf(fout, "%6.2f, ", result[i][j]);
	//		fprintf(fout, "\n");
	//	}
	//	fclose(fout);
	//}


	//矩阵result 与paraB相乘
	Mult(result, paraB, para, 4, 1, 4);


	//for (int i = 0; i < height + 4; ++i)
	//	free(matrix_in[i]);
	//free(matrix_in);

	for (int i = 0; i < 4; ++i)
	{
		free(result[i]);
		free(paraA[i]);
		free(paraB[i]);
	}
	free(result);
	free(paraA);
	free(paraB);
}


/****************备份楔形滤波器模式与角度对应关系!!!!!!!!!!!!!!!!!!!!******************************/
//void AccumulateParaAB_four_para(double **paraA, double **paraB, short block_image[10][10], int i, int j, int mode)
//	{   //需要调整多个块累加的程序。。。。??
//		static short u[5] = {0};	// u[y][x] equals to u(x, y) y equals i, x equals j
//		switch (mode)
//		{
//		case 1:   
//			u[1] = block_image[i][j - 1];
//			u[2] = block_image[i][j - 2];
//			u[3] = block_image[i][j - 3];
//			u[4] = block_image[i - 1][j - 2];
//			break;
//		case 2:  
//			u[1] = block_image[i - 1][j];
//			u[2] = block_image[i - 1][j + 1];
//			u[3] = block_image[i - 1][j + 2];
//			u[4] = block_image[i - 2][j + 2];
//			break;
//		case 3:   
//			u[1] = block_image[i - 1][j];
//			u[2] = block_image[i - 1][j + 1];
//			u[3] = block_image[i - 2][j + 1];
//			u[4] = block_image[i - 2][j + 2];
//			break;
//		case 4:   
//			u[1] = block_image[i - 1][j];
//			u[2] = block_image[i - 1][j + 1];
//			u[3] = block_image[i - 2][j];
//			u[4] = block_image[i - 2][j + 1];
//			break;
//		case 5:   
//			u[1] = block_image[i - 1][j - 1];
//			u[2] = block_image[i - 1][j];
//			u[3] = block_image[i - 1][j + 1];
//			u[4] = block_image[i - 2][j];
//			break;	
//		case 6:   
//			u[1] = block_image[i - 1][j - 1];
//			u[2] = block_image[i - 1][j];
//			u[3] = block_image[i - 2][j - 1];
//			u[4] = block_image[i - 2][j];  //自己构造
//			break;	
//		case 7:   
//			u[1] = block_image[i][j - 1];
//			u[2] = block_image[i - 1][j - 1];
//			u[3] = block_image[i - 1][j];
//			u[4] = block_image[i - 2][j - 2];  //可自己构造
//			break;	
//		case 8:   /*7*/
//			u[1] = block_image[i][j - 1];
//			u[2] = block_image[i - 1][j - 2];
//			u[3] = block_image[i - 1][j - 1];
//			u[4] = block_image[i - 1][j];  //可自己构造
//			break;	
//		default:
//			assert("遍历mode出错!");
//		}
//		u[0] = block_image[i][j];
//	
//		paraA[0][0] += u[1] * u[1];
//		paraA[0][1] += u[1] * u[2];
//		paraA[0][2] += u[1] * u[3];
//		paraA[0][3] += u[1] * u[4];
//		paraA[1][0] += u[1] * u[2];
//		paraA[1][1] += u[2] * u[2];
//		paraA[1][2] += u[2] * u[3];
//		paraA[1][3] += u[2] * u[4];
//		paraA[2][0] += u[1] * u[3];
//		paraA[2][1] += u[2] * u[3];
//		paraA[2][2] += u[3] * u[3];
//		paraA[2][3] += u[3] * u[4];
//		paraA[3][0] += u[1] * u[4];
//		paraA[3][1] += u[2] * u[4];
//		paraA[3][2] += u[3] * u[4];
//		paraA[3][3] += u[4] * u[4];
//	
//		paraB[0][0] += u[0] * u[1];
//		paraB[1][0] += u[0] * u[2];
//		paraB[2][0] += u[0] * u[3];
//		paraB[3][0] += u[0] * u[4];
//
//	}
//	void estimate_four_para(short block_image[10][10], double **para, int mode)
//	{
//	
//		double **paraA = NULL;
//		double **paraB = NULL;
//		double **result = NULL;
//		short pixAverage = 0;              //当前块像素的平均值
//		short pixSum = 0;              //像素的总和
//	
//		paraA  = (double **)calloc(4, sizeof(double *));
//		for(int i = 0; i < 4; i++)
//		{
//			paraA[i] = (double *)calloc(4,sizeof(double));
//		}
//	
//		result  = (double **)calloc(4, sizeof(double *));
//		for(int i = 0; i < 4; i++)
//		{
//			result[i] = (double *)calloc(4,sizeof(double));
//		}
//	
//		paraB  = (double **)calloc(4, sizeof(double *));
//		for(int i = 0; i < 4; i++)
//		{
//			paraB[i] = (double *)calloc(1,sizeof(double));
//		}
//	
//		for(int i = 3; i < BLOCKHEIGHT + 3; ++i)
//			for(int j = 3; j < BLOCKWIDTH + 3; ++j)
//				AccumulateParaAB_four_para(paraA, paraB, block_image, i, j, mode);
//	
//		//printf("para end \n");
//	
//	
//		//for(int i = 0; i < 3; i++)
//		//{
//		//	for(int j = 0; j < 3; ++j)
//		//	{
//		//		paraA[0][0] = 1;
//		//		paraA[0][1] = 1;
//		//		paraA[0][2] = 2;
//		//		paraA[1][0] = -1;
//		//		paraA[1][1] = 2;
//		//		paraA[1][2] = 0;
//		//		paraA[2][0] = 1;
//		//		paraA[2][1] = 1;
//		//		paraA[2][2] = 3;
//		//	}
//		//}
//		//for(int i = 0; i < 3; i++)
//		//{
//		//	for(int j = 0; j < 3; ++j)
//		//	{
//		//		paraB[0][0] = 1;
//		//		paraB[1][0] = 0;
//		//		paraB[2][0] = 0;
//		//	}
//		//}
//	
//		if (Gauss(paraA, result, 4))  
//		{  
//			//printf("Matrix A inve is\n");
//			//for(int i = 0; i < 4; ++i)
//			//{
//			//	for(int j = 0; j < 4; ++j)
//			//	{
//			//		printf("%.15lf ", result[i][j]); 
//			//	}
//			//printf("\n");
//			//}
//		} 
//		// 矩阵求逆l
//		//double *res_temp = MatrixOpp((double *)paraA, 3, 3); 
//		//for(int i = 0; i < 3; i++)
//		//{
//		//	for(int j = 0; j < 3; ++j)
//		//	{
//		//		printf("%d\n", paraA[i][j]);
//		//	}
//		//}
//	
//		//long long a = getA(paraA, 3);  
//		//if(a==0)  
//		//{  
//		//	printf("can not transform!\n");  
//		//}  
//		//else  
//		//{  
//		//	getAStart(paraA, 3, result);  
//		//	for(int i=0;i<3;i++)  
//		//	{  
//		//		for(int j=0;j<3;j++)  
//		//		{  
//		//			printf("%.3lf ",(double)result[i][j]/a);  
//		//		}  
//		//		printf("\n");  
//		//	}  
//		//}
//	
//	
//		//for(int i = 0; i < 9; i++)
//		//{
//		//		printf("%lf\n", res_temp[i]);
//		//
//		//}
//		//
//		//
//		//for (int i = 0; i < 3; ++i)
//		//	for (int j = 0; j < 3; ++j)
//		//		result[i][j] = res_temp[i * 3 + j];
//		////????
//		//free(res_temp);
//		//
//		//{
//		//	FILE *fout = fopen("res.txt", "w");
//		//	for (int i = 0; i < 3; ++i)
//		//	{
//		//		for (int j = 0; j < 3; ++j)
//		//			fprintf(fout, "%6.2f, ", result[i][j]);
//		//		fprintf(fout, "\n");
//		//	}
//		//	fclose(fout);
//		//}
//	
//	
//		//矩阵result 与paraB相乘
//		Mult(result, paraB, para, 4, 1, 4);
//	
//	
//		//for (int i = 0; i < height + 4; ++i)
//		//	free(matrix_in[i]);
//		//free(matrix_in);
//	
//		for (int i = 0; i < 4; ++i)
//		{
//			free(result[i]);
//			free(paraA[i]);
//			free(paraB[i]);
//		}
//		free(result);
//		free(paraA);
//		free(paraB);
//	}