#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cassert>

/********************************************************************************************
 * date: 2016.3 by lss
 * description: prediction, transformation and encoding for single frame picture.
 * 1. estimate prediction param of 9 modes for block of 4 x 4 and 4 modes for block of 16 x 16
 *    by Markov prediction method
 * 2. use estimated prediction param to predict
 * 3. transform the residual of prediction
 * 4. quantization and entropy encoding
 *********************************************************************************************/

const int mode = 0;			// mode for predcition 0~8 in 4 x 4
const double PI = 3.1415;

// format transformation for input img file to little endian
// img:		output
// img_in:	origin img
void transformat(short **img, unsigned char *img_in, int height, int width, int endian, int precision);

// use estimated prediction param to predict
// img:		input img value
// pre:		output of prediction value
// resi:	output of residual
void predict(short **img, double **pre, double **resi, int height, int width);

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

void  predict(short **img, double **pre, double **resi, int height, int width)
{
	short  N, W, NW;
	//     short NE, SW;
	short  **tb = NULL;
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

	tb = (short **)calloc(height + 1,sizeof(short *));//新建数组，开辟空间，多开辟出一行一列存储上边和左边的邻近像素

	for(int j = 0; j < height + 1; j++)
	{
		tb[j] = (short *)calloc(width + 1,sizeof(short));
	}

	for(int i = 0; i < height + 1; i++)    //将输入图像值赋值给数组
	{
		for(int j = 0; j < width + 1; j++)
		{
			if((i == 0) || (j == 0))
			{
				tb[i][j] = 128;
			}
			else
				tb[i][j] = img[i-1][j-1];
		}
	}

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
			//         case 0: 
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

void transformat(short **img, unsigned char *img_in,  int height, int width, int endian, int precision)
{
	int i, j;

	if(precision <= 8)
	{
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				img[i][j] = img_in[i* width + j];
			}
		}
	}

	else if((precision >= 8)&&(precision <= 16))
	{
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				if(endian == 0) //大端 
				{  
					img[i][j] = (short)(img_in[(i * width + j)*2] * 256 + img_in[(i * width + j) * 2 + 1]);

				}
				else if(endian == 1) //小端
				{
					img[i][j] = (short)(img_in[(i * width + j) * 2 + 1] * 256 + img_in[(i * width + j) * 2]); 
				}
			}

		}
	}
}

//转置函数
// TDCT: 转置后的矩阵
// DCT：原始矩阵
// H1：原始矩阵的高
// W1：原始矩阵的宽

// transpose DCT to IDCT
// DCT:		input
// IDCT:	output
void Fast(double ** TDCT, double ** DCT, int rows, int cols)
{
	for(int i=0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			TDCT[j][i] = DCT[i][j];
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
// TO-DO verify whether is correct
/*矩阵求逆****A为一维数组*/ 
double *MatrixOpp(double A[],int   m,int   n)  
{ 
	double *SP = NULL
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

// TO-DO: 修改边界扩展至2行2列，模式3与4采用不同的估计方式
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
//
//	}
//	
//}
void calcupara(short **img, double **para, int width, int height);

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
	//paraB = (doule *)calloc(3, sizeof(double));
	para = (double **)calloc(3,sizeof(double *));
	for(int i = 0; i < 3; i++)
	{
		para[i] = (double *)calloc(1,sizeof(double));
	}
	//para = (doule *)calloc(3, sizeof(double));
	
	
	
	short **matrix_in = NULL;
	matrix_in = (short **)calloc(height + 1,sizeof(short *));//新建数组，开辟空间，多开辟出一行一列存储上边和左边的邻近像素

	for(int j = 0; j < height + 1; j++)
	{
		matrix_in[j] = (short *)calloc(width + 1,sizeof(short));
	}

	for(int i = 0; i < height + 1; i++)    //将输入图像值赋值给数组
	{
		for(int j = 0; j < width + 1; j++)
		{
			if((i == 0) || (j == 0))
			{
				matrix_in[i][j] = 128;
			}
			else
				matrix_in[i][j] = img[i-1][j-1];
		}
	}

	//判断模式，根据不同模式给左边，左上，上边元素进行赋值，再调用下面的计算公式

	for(int i = 1; i < height  + 1 ; ++i )
	{
		for(int j = 1; j < width + 1; ++j)
		{



			paraA[0][0] += matrix_in[i - 1][j] * matrix_in[i - 1][j];
			paraA[0][1] += matrix_in[i - 1][j] * matrix_in[i - 1][j - 1];
			paraA[0][2] += matrix_in[i - 1][j] * matrix_in[i][j - 1];
			paraA[1][0] += matrix_in[i - 1][j - 1] * matrix_in[i - 1][j];
			paraA[1][1] += matrix_in[i - 1][j - 1] * matrix_in[i - 1][j - 1];
			paraA[1][2] += matrix_in[i - 1][j - 1] * matrix_in[i][j - 1];
			paraA[2][0] += matrix_in[i][j - 1] * matrix_in[i - 1][j];
			paraA[2][1] += matrix_in[i][j - 1] * matrix_in[i - 1][j - 1];
			paraA[2][2] += matrix_in[i][j - 1] * matrix_in[i][j - 1];

		}
	}
	// 矩阵求逆l
	double *res_temp = MatrixOpp((double   *)paraA, 3, 3); 
	
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

	for(int i = 1; i < height + 1; ++i )
	{
		for(int j = 1; j < width + 1; ++j)
		{
			paraB[0][0] += matrix_in[i][j] * matrix_in[i - 1][j];

			paraB[1][0] += matrix_in[i][j] * matrix_in[i - 1][j - 1];

			paraB[2][0] += matrix_in[i][j] * matrix_in[i][j - 1];
		}
	}

	//矩阵result 与B相乘
	Mult(result, paraB, para, 3, 1, 3);
	
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

int main(int argc, char *argv[])
{
	FILE *filein = NULL;				// 输入原图
	FILE *fileout = NULL;				// 输出编码后的图像
	int  height;						// 原图高
	int  width;							// 原图宽
	int  endian;						// 大小端
	int  precision;						// 原图比特精度，一般8bit
	unsigned char *img_in = NULL;		// 原始图像的输入
	short **img = NULL;					// 原始图像大小端转换后的数据
	
	double **para = NULL;				// 估计的预测参数
	double **pre = NULL;				// 预测后的图像数据
	double **resi = NULL;				// 预测矩阵残差
	int   img_size = 0;					// 图像尺寸
	int   resi_size = 0;				// 残差矩阵尺寸
	double Ck;							// 

	double   **DCT = NULL;				// DCT系数矩阵
	double   **ODST = NULL;				// ODST系数矩阵
	double   **TDCT = NULL;				// DCT系数矩阵的转置
	double   **f  = NULL;				// 变换后的矩阵？？？？
	double   **F  = NULL;				// 变换后的矩阵？？？？
	double   **temp = NULL;				// 变换中间矩阵


	//   double q[16][16];
	//	double Q[16][16];

	/*double q[1][16] =  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	{-x1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,-x1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,-x1,1,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,-x1,1,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,-x1,1,0,0,0,0,0,0,0,0,0,0},
	q[0][0] = 1;
	for(i = 0,j = 0; j <= 15; j++)
	{
	q[i][j] = 0;
	}


	for(i = 2; i < 16; i++)
	{
	for(j = 2; j < 16; j++)
	{
	q[i][i] = 1;
	q[i][j-1] = -0.42;
	}
	}
	*/
	/*	
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


	*/

// 1. 读入参数
	if(argc != 7)
	{
		printf("please input :infile outfile height width endian precision\n");
		return 0;
	}
	height = (int)atoi(argv[3]);
	width  = (int)atoi(argv[4]);
	endian = (int)atoi(argv[5]);
	precision = (int)atoi(argv[6]);
	img_size = height * width;
	resi_size = height * width;
	img_in = (unsigned char *)calloc(img_size, sizeof(char));//如果分配成功则返回指向被分配内存的指针，否则返回空指针NULL
	assert(img_in);
	//malloc 向系统申请分配指定size个字节的内存空间。返回类型是 void* 类型。void* 表示未确定类型的指针。
	//C,C++规定，void* 类型可以强制转换为任何其它类型的指针。返回后强行转换为实际类型的指针。

	// 给动态数组分配空间并读入文件
	img = (short **)calloc(height, sizeof(char *));
	for(int i = 0; i < height; i++)
		img[i] = (short *)calloc(width,sizeof(short));

	para = (double **)calloc(3,sizeof(double *));
	for(int i = 0; i < 3; i++)
		para[i] = (double *)calloc(1,sizeof(double));

	pre = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		pre[i] = (double *)calloc(width,sizeof(double));

	resi = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		resi[i] = (double *)calloc(width,sizeof(double));

	DCT = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		DCT[i] = (double *)calloc(width,sizeof(double));

	TDCT = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		TDCT[i] = (double *)calloc(width,sizeof(double));
	
	ODST = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		ODST[i] = (double *)calloc(width,sizeof(double));
	
	f  = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		f[i] = (double *)calloc(width,sizeof(double));
	
	F  = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		F[i] = (double *)calloc(width,sizeof(double));
	
	temp  = (double **)calloc(height, sizeof(double *));
	for(int i = 0; i < height; i++)
		temp[i] = (double *)calloc(width,sizeof(double));

	// 输入原始图像文件
	if((filein = fopen(argv[1],"rb")) == NULL)
	{
		printf("the file can not open\n");
		exit(0);
	}
	fread(img_in,sizeof(unsigned char),img_size,filein);
	fclose(filein);
	filein = NULL;
	printf("打开文件成功\n");
	//// 完成编码，打开输出码流文件进行回写
	//if((fileout = fopen(argv[2],"wb")) == NULL)
	//{
	//	printf("oppenning error fileout\n");
	//	exit(0);
	//}
	


	// 大小端格式转换
	transformat(img,img_in, height, width, endian, precision);
	
	/**************************************************************
	 * 1. estimate prediction param of 9 modes for block of 4 x 4 and 4 modes for block of 16 x 16
	 *    by Markov prediction method
	 *************************************************************/
	//估计用于预测的邻近像素权值
	estimate(img, para, width, height);

	/**************************************************************
	 * 2. use estimated prediction param to predict
	 *************************************************************/
	predict(img, pre, resi, height, width);

	// 打印残差矩阵进行调试
	FILE *fout = fopen("output.txt", "w");
	assert(fout);
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			fprintf(fout, "%6.2lf  ", resi[i][j]);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
	printf("预测成功\n");


	/**************************************************************
	 * 3. transform the residual of prediction
	 *************************************************************/
	// 预测后的变换


	// TO-DO: 后期DCT变换划分模块时，再修改成函数调用方式以重构
	// 计算DCT系数
	for(int i=0;i<height;i++)
	{
		if(i == 0) 
			Ck=1.000000/sqrt(2 * 1.0);
		else Ck=1;
		for(int j=0;j<width;j++)
			DCT[i][j]=sqrt(2.000000/width)*Ck*cos((2*j+1)*i*PI/(2*width));	//DCT系数
	}

	//求转置
	Fast(TDCT, DCT, height, width);	

	//行DCT变换
	Mult(DCT, resi, temp, height, width, height);	

	// TO-DO: 后期DCT变换划分模块时，修改成函数调用的方式以重构
	//ODST-3系数
	for (int i = 0; i < height; i++)
	{
		if (i == 0) 
			Ck = 2.000000 / sqrt((2 * width + 1) * 0.1);
		else Ck = 1;
		for (int j = 0; j < width; j++)
			ODST[i][j] = Ck * sin((2 * i - 1)* j * PI / (2 * width + 1));	
	}

	//列ODST变换
	Mult(temp, ODST, F, height, width, height);	
	printf("变换结束\n");

	// 打印调试信息：行DCT变换，列ODST变换后的值
	FILE *fout_ODST = fopen("output1.txt", "w");
	assert(fout_ODST);
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			fprintf(fout_ODST, "%6.2lf  ", F[i][j]);
		}
		fprintf(fout_ODST, "\n");
	}

	// fwrite(temp, img_size + 1, 1, fout_ODST);
	fclose(fout_ODST);
	
	printf("%lf\n", F);
	fwrite(F, img_size + 1, 1, fileout);
	
	//	fprintf(fileout, "%lf\n", F);

	// 释放空间
	for(int i = 0; i < height; i++)  
		free(pre[i]);  
	free(pre);

	for(int i = 0; i < height; i++)  
		free(img[i]);  
	free(img);

	free(resi);
	resi = NULL;

	free(img_in);
	img_in = NULL;

	fclose(filein);
	filein = NULL;

	fclose(fileout);
	fileout = NULL;

	// 动态分配的内存的释放，二维动态数组的参数

	return 0;
}