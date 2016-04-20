/********************************************************************
	created:	2011/06/17
	created:	17:6:2011   15:20
	filename: 	D:\Program Files\vc\my project\image_cut\image_cut.cpp
	file path:	D:\Program Files\vc\my project\image_cut
	file base:	image_cut
	file ext:	cpp
	author:		wangyan
	
	purpose:	to cut an image into tiles :col N row M
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imagecut(int argc, char *argv[])
{
	int row=1;
	int col=1;
	int outWidth=1024;
	int outHeight=1024;
	int inWidth=1024;
	int inHeight=1024;
	int pixByte=8;
	int inputSize;
	int outputSize;
	unsigned char *inImg;
	unsigned char *outImg;
	char infilename[100],infile_temp[100],a[100],temp[100];
	char outfilename[100];
	char			*point;
	int				result;
	unsigned char smallimage[256][256][8][8] = {0};

	FILE *fpInput;
	FILE *fpOutput;
	int i_col = 0;
	int i_row = 0;
	int i,j;
	int zheng_en = 1;

	if (argc!=7&&argc!=9)
	{
		printf("the argv error!!!\n");
		printf("Inputfile(*.raw)\n");
		scanf("%s",&infilename);
		printf("图像宽度;\n");
		scanf("%d",&inWidth);
		printf("图像高度;\n");
		scanf("%d",&inHeight);
		printf("图像精度(8-16bit)\n");
		scanf("%d",&pixByte);
		printf("横向分片个数(col);\n");
		scanf("%d",&col);
		printf("纵向分片个数(row);\n");
		scanf("%d",&row);
		printf("图像大小是否能被整除;1-yes   0-no\n");

		scanf("%d",&zheng_en);
		if (zheng_en==0)
		{
			printf("分割小图宽度:\n");
			scanf("%d",&outWidth);
			printf("分割小图高度:\n");
			scanf("%d",&outHeight);
		}
	}
	else //参数个数为7或者9
	{
		strcpy(infilename,argv[1]);
		inWidth = (int) atoi(argv[2]);
		inHeight = (int) atoi(argv[3]);
		pixByte = (int) atoi(argv[4]);
		col = (int) atoi(argv[5]);
		row = (int) atoi(argv[6]);
	}
	strcpy(temp,infilename);
	point=strchr(temp,'.');
	result=point-temp;
	for (i=0;i<result;i++)
		a[i]=temp[i];
	a[i]='\0';
	strcpy(infile_temp,a);

	if (inWidth%col!=0||inHeight%row!=0)
	{
		printf("按%dx%d分片数目分割图像不能整除 ! \n",col,row);
		if (argc!=9&&zheng_en==1) //整除
		{
			printf(" exit.\n");
			exit(0);
		}
	}
	//参数个数为9或者不能整除
	if (argc==9||zheng_en==0)   
	{
		outWidth = (int) atoi(argv[7]);
		outHeight = (int) atoi(argv[8]);
		printf("按%dx%d分片大小分割图像:\n",outWidth,outHeight);
	}
	else if (zheng_en==1)//参数个数不为9并且可以整除
	{
		outWidth	= inWidth/col;
		outHeight	= inHeight/row;
		printf("按%dx%d分片大小分割图像:\n",outWidth,outHeight);
	}
	if (pixByte<8||pixByte>16)
	{
		printf("Error  precision ! exit.\n");
		exit(0);
	}
	inputSize	= inWidth*inHeight;
	outputSize	= outWidth*outHeight;
	if (pixByte!=8)
	{
		inputSize	*= 2;
		outputSize	*= 2;
		inWidth		*= 2;
		outWidth	*= 2;
	}

	inImg = (unsigned char *)malloc(inputSize*sizeof(char));
	outImg = (unsigned char *)malloc(outputSize*sizeof(char));
    fpInput = fopen(infilename, "rb");
	if (fpInput == NULL)
	{
		printf("Inputfile error!!!\n");
		exit(0);
	}
	fread(inImg, inputSize, 1, fpInput);
	if (argc==9||zheng_en==0)
	{
		row = inHeight/outHeight;
		col = inWidth/outWidth;
		
	}



/////////////////////////////////////////////
	for (i_row=0; i_row<row; ++i_row)
	{
		for (i_col=0; i_col<col; ++i_col)
		{
			for (i=0; i<outHeight; ++i)
			{
				for (j=0; j<outWidth; ++j)
				{
					smallimage[i_row][i_col][i + 4][j + 4] = inImg[(i_row*outHeight+i)*inWidth+i_col*outWidth+j];
				}
			}

/*
			sprintf(outfilename,"%s_row%d_col%d.raw",infile_temp,i_row,i_col);
			fpOutput = fopen(outfilename, "wb");
			if (fpOutput == NULL)
			{
				printf("Outputfile error!!!\n");
				exit(0);
			}
			fwrite(outImg, outputSize, 1, fpOutput);
			fclose(fpOutput);
			fpOutput = NULL;*/
			
		}
	}
	if (argc==9||zheng_en==0)
	{
		outputSize	= outWidth*(inHeight%outHeight);
		for (i_col=0; i_col<col; ++i_col)
		{
			if (inHeight%outHeight==0)
			{
				break;
			}
			for (i=0; i<inHeight%outHeight; ++i)
			{
				for (j=0; j<outWidth; ++j)
				{
					outImg[i*outWidth+j] = inImg[(row*outHeight+i)*inWidth+i_col*outWidth+j];
				}
			}
			if (pixByte!=8)
			{
				sprintf(outfilename,"%s_row%d_col%d_%dx%d.raw",infile_temp,row,i_col,outWidth/2,inHeight%outHeight);
			}
			else
				sprintf(outfilename,"%s_row%d_col%d_%dx%d.raw",infile_temp,row,i_col,outWidth,inHeight%outHeight);

			fpOutput = fopen(outfilename, "wb");
			if (fpOutput == NULL)
			{
				printf("Outputfile error!!!\n");
				exit(0);
			}
			fwrite(outImg, outputSize, 1, fpOutput);
			fclose(fpOutput);
			fpOutput = NULL;
		}
		outputSize	= outHeight*(inWidth%outWidth);
		for (i_row=0; i_row<row; ++i_row)
		{
			if (inWidth%outWidth==0)
			{
				break;
			}
			for (i=0; i<outHeight; ++i)
			{
				for (j=0; j<inWidth%outWidth; ++j)
				{
					outImg[i*(inWidth%outWidth)+j] = inImg[(i_row*outHeight+i)*inWidth+col*outWidth+j];
				}
			}
			if (pixByte!=8)
			{
				sprintf(outfilename,"%s_row%d_col%d_%dx%d.raw",infile_temp,i_row,col,(inWidth%outWidth)/2,outHeight);
			}
			else
			{
				sprintf(outfilename,"%s_row%d_col%d_%dx%d.raw",infile_temp,i_row,col,inWidth%outWidth,outHeight);
			}
			fpOutput = fopen(outfilename, "wb");
			if (fpOutput == NULL)
			{
				printf("Outputfile error!!!\n");
				exit(0);
			}
			fwrite(outImg, outputSize, 1, fpOutput);
			fclose(fpOutput);
			fpOutput = NULL;
		}
		if (inWidth%outWidth!=0&&inHeight%outHeight!=0)
		{
			outputSize	= (inHeight%outHeight)*(inWidth%outWidth);
			for (i=0; i<(inHeight%outHeight); ++i)
			{
				for (j=0; j<inWidth%outWidth; ++j)
				{
					outImg[i*(inWidth%outWidth)+j] = inImg[(row*outHeight+i)*inWidth+col*outWidth+j];
				}
			}
			if (pixByte!=8)
			{
				sprintf(outfilename,"%s_row%d_col%d_%dx%d.raw",infile_temp,row,col,(inWidth%outWidth)/2,inHeight%outHeight);
			}
			else
			{
				sprintf(outfilename,"%s_row%d_col%d_%dx%d.raw",infile_temp,row,col,inWidth%outWidth,inHeight%outHeight);
			}
			fpOutput = fopen(outfilename, "wb");
			if (fpOutput == NULL)
			{
				printf("Outputfile error!!!\n");
				exit(0);
			}
			fwrite(outImg, outputSize, 1, fpOutput);
			fclose(fpOutput);
			fpOutput = NULL;
		}
	}
	free(outImg);
	outImg = NULL;
	fclose(fpInput);
	fpInput = NULL;
	free(inImg);
	inImg = NULL;
}