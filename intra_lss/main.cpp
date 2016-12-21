#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cassert>
#include <cstdint>
#include "pre.h"
#include "common.h"
using namespace std;
/********************************************************************************************
 * date: 2016.3 by lss
 * description: prediction, transformation and encoding for single frame picture.
 * 1. estimate prediction param of 9 modes for block of 4 x 4 and 4 modes for block of 16 x 16
 *    by Markov prediction method
 * 2. use estimated prediction param to predict
 * 3. transform the residual of prediction
 * 4. quantization and entropy encoding
 *********************************************************************************************/

const double PI = 3.1415;
short block_012pre[4][4] = {0};				 //每一个小块的预测值
short block_pre3[4][4] = {0};				 //每一个小块的预测值
short block_pre4[4][4] = {0};				 //每一个小块的预测值
short block_pre5[4][4] = {0};				 //每一个小块的预测值
short block_pre6[4][4] = {0};				 //每一个小块的预测值
short block_pre7[4][4] = {0};				 //每一个小块的预测值
short block_pre8[4][4] = {0};				 //每一个小块的预测值
int count_para_mode[4][10] = {0};
int count[9] = {0};
int  para_num[4] = {0};											//标记不同参数个数下预测最优的图像块数


struct block blocktab[ROWS][COLS];

//short dc_left_image[256][256][8][8];    // 全局块
//short dc_top_image[256][256][8][8];
//short dc_image[256][256][8][8];
//short h_image[256][256][8][8];
//short v_image[256][256][8][8];
//short ddl_image[256][256][8][8];
//short ddr_image[256][256][8][8];
//short vr_image[256][256][8][8];
//short hd_image[256][256][8][8];
//short vl_image[256][256][8][8];

//void imagecutextern short hu_image[256][256][8][8];(int argc, char *argv[]);

// print a matrix to a file for debugging
int print_matrix_to_file(double **mat, int rows, int cols, const char *filename)
{
	FILE *fout = fopen(filename, "w");
	assert(fout);
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			fprintf(fout, "%6.2f, ", mat[i][j]);
		fprintf(fout, "\n");
	}
	fclose(fout);
	return 0;
}
int print_matrix_to_file(short **mat, int rows, int cols, const char *filename)
{
	FILE *fout = fopen(filename, "w");
	assert(fout);
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
			fprintf(fout, "%4d", mat[i][j]);
		fprintf(fout, "\n");
	}
	fclose(fout);
	return 0;
}

// format transformation for input img file to little endian
// img:		output
// img_in:	origin img
void transfer_end(short **img, unsigned char *img_in, int height, int width, int endian, int precision);

void transfer_end(short **img, unsigned char *img_in,  int height, int width, int endian, int precision)
{
	if(precision <= 8)
	{
		for(int i = 0; i < height; i++)
			for(int j = 0; j < width; j++)
				img[i][j] = img_in[i* width + j];
	}

	else if((precision >= 8) && (precision <= 16))
	{
		for(int i = 0; i < height; i++)
			for(int j = 0; j < width; j++)
			{
				if(endian == 0) //大端 
					img[i][j] = (short)(img_in[(i * width + j)*2] * 256 + img_in[(i * width + j) * 2 + 1]);
				else if(endian == 1) //小端
					img[i][j] = (short)(img_in[(i * width + j) * 2 + 1] * 256 + img_in[(i * width + j) * 2]); 
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

//void Fast(double ** TDCT, double ** DCT, int rows, int cols)
//{
//	for(int i=0; i < rows; i++)
//		for(int j = 0; j < cols; j++)
//			TDCT[j][i] = DCT[i][j];
//}

int main(int argc, char *argv[])
{
	freopen("engery.txt", "w", stdout);
	FILE *filein = NULL;				// 输入原图
	FILE *fileout = NULL;				// 输出编码后的图像
	int  height;						// 原图高
	int  width;							// 原图宽
	int  endian;						// 大小端
	int  precision;						// 原图比特精度，一般8bit
	unsigned char *img_in = NULL;		// 原始图像的输入
	short **img = NULL;					// 原始图像大小端转换后的数据
	
	//short img264Constructed[1024 + 1][1024 + 5] = {0}; //重建图像数组，利用264方法得到原图相应块位置的重建值，存储利用重建值更新原图后的整幅图数据
	//short resi264[1024][1024] = {0};                   //存储264预测后整幅图的残差数据
	//short pre264[1024][1024] = {0};                    //存储264预测后的预测数据
	//long long H264resi_energy = 0;                       //原始大图的预测残差能量

	short **imgLSConstructed = NULL; //利用LS方法得到原图相应块位置的重建值，存储利用重建值更新原图后的整幅图数据
	short **LS_resi = NULL;         //存储LS方法预测后得到的整幅图的残差数据
	short **preLS = NULL;           //原始大图的预测值
	long long LS_resi_energy = 0;       //原始大图的预测残差能量
	short pixAverage = 0;              //像素的平均值
	long long pixSum = 0;              //像素的总和

	//short imgLSConstructed[1024 + 4][1024 + 4] = {0}; //利用LS方法得到原图相应块位置的重建值，存储利用重建值更新原图后的整幅图数据
	//short LS_resi[1024][1024] = {0};                  //存储LS方法预测后得到的整幅图的残差数据
	//int16_t LS_resi_energy = 0;                       //原始大图的预测残差能量

	
	double **resi = NULL;				// 预测矩阵残差
	int   img_size = 0;					// 图像尺寸
	int   resi_size = 0;				// 残差矩阵尺寸
	//double Ck;							// 

	/*double   **DCT = NULL;				// DCT系数矩阵
	double   **ODST = NULL;				// ODST系数矩阵
	double   **TDCT = NULL;				// DCT系数矩阵的转置
	double   **f  = NULL;				// 变换后的矩阵？？？？
	double   **F  = NULL;				// 变换后的矩阵？？？？
	double   **temp = NULL;				// 变换中间矩阵*/


	
	//	double Q[16][16];//   double q[16][16];
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
	//smallheight = (int)atoi(argv[7]);
	//smallwidth = (int)atoi(argv[8]);
	img_size = height * width;
	resi_size = height * width;
	img_in = (unsigned char *)calloc(img_size, sizeof(short));//如果分配成功则返回指向被分配内存的指针，否则返回空指针NULL
	assert(img_in);
	//malloc 向系统申请分配指定size个字节的内存空间。返回类型是 void* 类型。void* 表示未确定类型的指针。
	//C,C++规定，void* 类型可以强制转换为任何其它类型的指针。返回后强行转换为实际类型的指针。


	// 给动态数组分配空间并读入文件
	img = (short **)calloc(height, sizeof(char *));
	for(int i = 0; i < height; i++)
		img[i] = (short *)calloc(width,sizeof(short));

	
	imgLSConstructed = (short **)calloc(height + 6, sizeof(short *));
	for(int i = 0; i < height + 6; i++)
		imgLSConstructed[i] = (short *)calloc(width + 6,sizeof(short));

	LS_resi = (short **)calloc(height, sizeof(short *));
	for(int i = 0; i < height; i++)
		LS_resi[i] = (short *)calloc(width,sizeof(short));

	preLS = (short **)calloc(height, sizeof(short *));
	for(int i = 0; i < height; i++)
		preLS[i] = (short *)calloc(width,sizeof(short));

	// 输入原始图像文件
	if((filein = fopen(argv[1],"rb")) == NULL)
	{
		printf("the file can not open\n");
		exit(0);
	}
	fread(img_in,sizeof(unsigned char), precision == 8 ? img_size : 2 * img_size, filein);
	fclose(filein);
	filein = NULL;
	printf("打开文件成功\n");
	

	
	//image cut 
	//char cutpara[9][100] = {"", "03.raw", "1024", "1024", "8", "256", "256", "4", "4"};
	
	//imagecut(9, (char **)cutpara);

	// 大小端格式转换
	transfer_end(img, img_in, height, width, endian, precision);
	printf("转换文件成功\n");
	//将输入图像值赋值给imgLSConstructed  重建图像数组
	for(int r = 0; r < height + 6; r++)    
	{
		for(int c = 0; c < width + 6; c++)
		{
			if(r < 3 || r > height + 2 || c < 3 || c > 2 + width)
				imgLSConstructed[r][c] = 128;
			else
				imgLSConstructed[r][c] = img[r - 3][c - 3];
		}
	}

	// 打印初始重建图矩阵进行调试
	FILE *fout = fopen("initConstruct.txt", "w");
	assert(fout);
	for (int i = 0; i < height + 6; ++i)
	{
		for (int j = 0; j < width + 6; ++j)
		{
			fprintf(fout, "%4d", imgLSConstructed[i][j]);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
	// 打印原图矩阵与初始重建值矩阵进行比较进行调试
	if((fout = fopen("lena1024.txt", "w")) == NULL)
	{
		printf("oppenning error output_text\n");
		exit(0);
	}
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			fprintf(fout, "%4d", img[i][j]);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);

	// 读入图像块方向信息文件
	int** direction_block = NULL;
	direction_block = (int **)calloc(ROWS, sizeof(int *));
	for(int i = 0; i < ROWS; i++)
		direction_block[i] = (int *)calloc(COLS,sizeof(int));
	if((filein = fopen("direction_lena_L_16_2.8.txt","r")) == NULL)
	{
		printf("the file can not open\n");
		exit(0);
	}
	for(int i_row = 0; i_row < ROWS; ++i_row){
		for(int i_col = 0; i_col < COLS; ++i_col){
			fscanf(filein, "%d", &direction_block[i_row][i_col]);
		}
	}
	
	fclose(filein);
	filein = NULL;
	printf("打开图像块方向信息文件成功\n");

	//LS_resi_energy = predict(imgLSConstructed, LS_resi, preLS, height, width);
	LS_resi_energy = predict(imgLSConstructed, LS_resi, preLS, height, width, direction_block);
	cout << "Total energy: " << LS_resi_energy << endl;

	int blocknum = 0;  //总块数为65536
	/************每个图像块将不同参数个数下预测的结果进行比较取最优***********
	**************统计各参数个数下，各模式为最优预测的图像块数******************
	for(int i = 0; i < 4; ++i)
	{
		cout << "para_num: " << i + 1 << endl;
		for(int j = 0; j < 9; ++j)
		{
			cout << "mode " << j << ": " << count_para_mode[i][j] << "\t";
			blocknum += count_para_mode[i][j];
		}
		cout << " " << endl;
	}
	cout << "blocknum: " << blocknum << endl;
	cout << endl;
	for(int i = 0; i < 4; ++i)
	{
		cout << "para_num " << i + 1 << ": "<< para_num[i] << "\t";
	}
	*******************************************************************/

	/**************单独统计每种参数个数下，各模式为最优预测的总图像块数******/
	for(int i = 0; i < 1; ++i)
	{
		for(int j = 0; j < 10; ++j)
		{
			cout << "count_para_mode: " << j << " " << count_para_mode[0][j] << endl;
			blocknum += count_para_mode[0][j];
		}
		cout << " " << endl;
	}
	cout << "blocknum: " << blocknum << endl;
	/********************************************************************/

	printf("预测结束\n");

	// 打印残差矩阵进行调试

	if((fout = fopen("resi.txt", "w")) == NULL)
	{
		printf("oppenning error output_text\n");
		exit(0);
	}
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			fprintf(fout, "%4d", LS_resi[i][j]);
		}
		fprintf(fout, "\n");
	}
	fclose(fout);

	// 完成编码，打开输出码流文件进行回写
	if((fileout = fopen(argv[2],"wb")) == NULL)
	{
		printf("oppenning error fileout\n");
		exit(0);
	}
	fwrite(imgLSConstructed, sizeof(short), (height + 6) * (width + 6), fileout);
	fclose(fileout);

	//// 转换成8bit
	unsigned char **pRes = new unsigned char*[height + 6];
	for (int i = 0; i < height + 6; ++i)
		pRes[i] = new unsigned char[width + 6];

	for (int r = 0; r < height + 6; ++r)
		for (int c = 0; c < width + 6; ++c)
			pRes[r][c] = static_cast<unsigned char>(imgLSConstructed[r][c]);
	fileout = fopen("img264Constructed_8bit.raw", "wb");
	if (fileout == NULL)
	{
		printf("openning error fileout\n");
		exit(0);
	}
	for (int r = 3; r < height + 3; ++r)
		fwrite(pRes[r] + 3, sizeof(unsigned char), width, fileout);
	fclose(fileout);
	for (int i = 0; i < height + 6; ++i)
		delete [] pRes[i];
	delete [] pRes;
	pRes = NULL;
	//打印最终的重建图像
	if((fileout = fopen("out_imgLSConstructed.txt", "w")) == NULL)
	{
		printf("oppenning error out_imgLSConstructed\n");
		exit(0);
	}
	for(int r = 3; r < height + 3; ++r)
	{
		for(int c = 3; c < width + 3; ++c)
			fprintf(fileout, "%4d", imgLSConstructed[r][c]);
		fprintf(fileout, "\n");
	}
	fclose(fileout);

	// 动态分配的内存的释放
	for(int i = 0; i < height + 6; i++)  
		free(imgLSConstructed[i]);  
	free(imgLSConstructed);

	for(int i = 0; i < height; i++)  
		free(LS_resi[i]);  
	free(LS_resi);

	for(int i = 0; i < height; i++)  
		free(preLS[i]);  
	free(preLS);


	for(int i = 0; i < height; i++)  
		free(img[i]);  
	free(img);

	free(img_in);
	img_in = NULL;

	return 0;
}
