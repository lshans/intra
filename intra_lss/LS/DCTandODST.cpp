/*double   **DCT = NULL;				// DCTϵ������
	double   **ODST = NULL;				// ODSTϵ������
	double   **TDCT = NULL;				// DCTϵ�������ת��
	double   **f  = NULL;				// �任��ľ��󣿣�����
	double   **F  = NULL;				// �任��ľ��󣿣�����
	double   **temp = NULL;				// �任�м����*/




// TO-DO: ����DCT�任����ģ��ʱ�����޸ĳɺ������÷�ʽ���ع�
// ����DCTϵ��
for(int i=0;i<height;i++)
{
	if(i == 0) 
		Ck=1.000000/sqrt(2 * 1.0);
	else Ck=1;
	for(int j=0;j<width;j++)
		DCT[i][j]=sqrt(2.000000/width)*Ck*cos((2*j+1)*i*PI/(2*width));	//DCTϵ��
}



//��ת��
Fast(TDCT, DCT, height, width);	

//��DCT�任
Mult(DCT, resi, temp, height, width, height);	

// TO-DO: ����DCT�任����ģ��ʱ���޸ĳɺ������õķ�ʽ���ع�
//ODST-3ϵ��
for (int i = 0; i < height; i++)
{
	if (i == 0) 
		Ck = 2.000000 / sqrt((2 * width + 1) * 0.1);
	else Ck = 1;
	for (int j = 0; j < width; j++)
		ODST[i][j] = Ck * sin((2 * i - 1)* j * PI / (2 * width + 1));	
}

//��ODST�任
Mult(temp, ODST, F, height, width, height);	
printf("�任����\n");

// ��ӡ������Ϣ����DCT�任����ODST�任���ֵ
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