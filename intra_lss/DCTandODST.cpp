/*double   **DCT = NULL;				// DCT系数矩阵
	double   **ODST = NULL;				// ODST系数矩阵
	double   **TDCT = NULL;				// DCT系数矩阵的转置
	double   **f  = NULL;				// 变换后的矩阵？？？？
	double   **F  = NULL;				// 变换后的矩阵？？？？
	double   **temp = NULL;				// 变换中间矩阵*/




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