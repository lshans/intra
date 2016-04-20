#include "pre.h"
#define FDEC_STRIDE 32
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

//inline void PREDICT_4x4_DC(uint8_t *src, short v)    //本函数对4x4块的每行像素赋同样的值
//{
//	*(uint32_t *) &src[0 * FDEC_STRIDE] = //第一句既表明p 指向的是32 位无符号整型变量，也表明其指向的是src（像素起始位置）正因为是32 位所以才有后面一次能给4 个像素赋值
//	*(uint32_t *) &src[1 * FDEC_STRIDE] =
//	*(uint32_t *) &src[2 * FDEC_STRIDE] =
//	*(uint32_t *) &src[3 * FDEC_STRIDE] = v;
//}
//inline void PREDICT_4x4_LOAD_LEFT(uint32_t *src)   //该定义表示依次取出当前宏块左边的4个像素
//{
//	const int l0 = src[-1 + 0 * FDEC_STRIDE];
//	const int l1 = src[-1 + 1 * FDEC_STRIDE];
//	const int l2 = src[-1 + 2 * FDEC_STRIDE];
//	UNUSED const int l3 = src[-1 + 3 * FDEC_STRIDE];
//}
//
//inline void PREDICT_4x4_LOAD_TOP(uint32_t *src)    //该定义表示依次取出当前宏块上边的4个像素
//{
//	const int t0 = src[-1 + 0 * FDEC_STRIDE];
//	const int t1 = src[-1 + 1 * FDEC_STRIDE];
//	const int t2 = src[-1 + 2 * FDEC_STRIDE];
//	UNUSED const int t3 = src[-1 + 3 * FDEC_STRIDE];
//}
//inline void PREDICT_4x4_LOAD_TOP_RIGHT(uint32_t *src)    //该定义表示依次取出当前宏块右上方的4个像素(E/F/G/H)
//{
//	const int t4 = src[4 - 1 * FDEC_STRIDE];
//	const int t5 = src[5 - 1 * FDEC_STRIDE];
//	const int t6 = src[6 - 1 * FDEC_STRIDE];
//	UNUSED const int t7 = src[7 - 1 * FDEC_STRIDE];
//}

// 本函数对4x4块的每行像素赋同样的值128
static void predict_4x4_dc_128(uint8_t *src)
{
	PREDICT_4x4_DC(0x80808080);
}

// 用宏块左方四个像素（I、J、K、L）的均值对所有像素覆盖
static void predict_4x4_dc_left(uint8_t *src)
{
	uint32_t dc = ((src[-1 + 0 * FDEC_STRIDE] + src[-1 + FDEC_STRIDE] +
	src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 2) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// 用宏块上方四个像素（A、B、C、D）的均值对所有像素覆盖
static void predict_4x4_dc_top(uint8_t *src)
{
	uint32_t dc = ((src[-1 + 0 * FDEC_STRIDE] + src[-1 + FDEC_STRIDE] +
		src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 2) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// 用宏块左边和上方八个像素（I、J、K、L、A、B、C、D）的均值对所有像素覆盖
static void predict_4x4_dc(uint8_t *src)
{
	uint32_t dc = ((src[-1 + 0 * FDEC_STRIDE] + src[-1 + FDEC_STRIDE] +
		src[-1 + 2 * FDEC_STRIDE] + src[-1 + 3 * FDEC_STRIDE] +
		src[0 - FDEC_STRIDE] + src[1 - FDEC_STRIDE] +
		src[2 - FDEC_STRIDE] + src[3 - FDEC_STRIDE] + 4) >> 2) * 0x01010101;
	PREDICT_4x4_DC(dc);
}

// 本函数用每行左边的像素对行进行覆盖（一行的所有像素值相同）
static void predict_4x4_h(uint8_t *src)    
{
	*(uint32_t *) &src[0 * FDEC_STRIDE] = src[0 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[1 * FDEC_STRIDE] = src[1 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[2 * FDEC_STRIDE] = src[2 * FDEC_STRIDE] * 0x01010101;
	*(uint32_t *) &src[3 * FDEC_STRIDE] = src[3 * FDEC_STRIDE] * 0x01010101;
}

// 本函数用每列上边的像素对列进行覆盖
static void predict_4x4_v(uint8_t *src)    
{
	uint32_t top = *((uint32_t *) &src[-FDEC_STRIDE]);//取出当前块上方4个像素点的值
	PREDICT_4x4_DC(top);                                 //每行均用取出的值进行覆盖
}

// 45度右上至左下覆盖预测 t0代表A  t1代表B t2代表C t3代表D
static void predict_4x4_ddl(uint8_t *src) 
{
	PREDICT_4x4_LOAD_TOP;
	PREDICT_4x4_LOAD_TOP_RIGHT
	src[0 * FDEC_STRIDE + 0] = (t0 + 2 * t1 + t2 + 2) >> 2;  //(A + 2B + C + 2)/4  +2表示四舍五入

	src[0 * FDEC_STRIDE + 1] = src[1 * FDEC_STRIDE + 0] = (t1 + 2 * t2 + t3 + 2) >> 2;
    src[0*FDEC_STRIDE+2] =
	src[1*FDEC_STRIDE+1] =
	src[2*FDEC_STRIDE+0] = ( t2 + 2*t3 + t4 + 2 ) >> 2;      //-------c=f=i=（C+2D+E+2）/4

	src[0*FDEC_STRIDE+3] =
	src[1*FDEC_STRIDE+2] =
	src[2*FDEC_STRIDE+1] =
	src[3*FDEC_STRIDE+0] = ( t3 + 2*t4 + t5 + 2 ) >> 2;  //-------d=g=j=m=（D+2E+F+2）/4

	src[1*FDEC_STRIDE+3] =
	src[2*FDEC_STRIDE+2] =
	src[3*FDEC_STRIDE+1] = ( t4 + 2*t5 + t6 + 2 ) >> 2;   //------h=k=n=（E+2F+G+2）/4

	src[2*FDEC_STRIDE+3] =
	src[3*FDEC_STRIDE+2] = ( t5 + 2*t6 + t7 + 2 ) >> 2;   //------i=o=（F+2G+H+2）/4

	src[3*FDEC_STRIDE+3] = ( t6 + 3*t7 + 2 ) >> 2;        //-----------p=（G+3H+2）/4
}

// 45°左上至右下覆盖预测
static void predict_4x4_ddr(uint8_t *src)     
{
	const int lt = src[-1-FDEC_STRIDE];     // -----lt=M--------------I、J、K、L---------------A、B、C、D
	PREDICT_4x4_LOAD_LEFT
	PREDICT_4x4_LOAD_TOP           
	src[0*FDEC_STRIDE+0] =
	src[1*FDEC_STRIDE+1] =
	src[2*FDEC_STRIDE+2] =
	src[3*FDEC_STRIDE+3] = ( t0 + 2 * lt + l0 + 2 ) >> 2;//a=f=k=p=（A+2M+I+2）/4

	src[0*FDEC_STRIDE+1] =
	src[1*FDEC_STRIDE+2] =
	src[2*FDEC_STRIDE+3] = ( lt + 2 * t0 + t1 + 2 ) >> 2;//b=g=l=（M+2A+B+2）/4

	src[0*FDEC_STRIDE+2] =
	src[1*FDEC_STRIDE+3] = ( t0 + 2 * t1 + t2 + 2 ) >> 2;//c=h=（A+2B+C+2）/4

	src[0*FDEC_STRIDE+3] = ( t1 + 2 * t2 + t3 + 2 ) >> 2;//d=（B+2C+D+2）/4

	src[1*FDEC_STRIDE+0] =
	src[2*FDEC_STRIDE+1] =
	src[3*FDEC_STRIDE+2] = ( lt + 2 * l0 + l1 + 2 ) >> 2;//e=j=o=（M+2I+J+2）/4

	src[2*FDEC_STRIDE+0] =
	src[3*FDEC_STRIDE+1] = ( l0 + 2 * l1 + l2 + 2 ) >> 2;//i=n=（I+2J+K+2）/4

	src[3*FDEC_STRIDE+0] = ( l1 + 2 * l2 + l3 + 2 ) >> 2;//m=（J+2K+L+2）/4
}

//与 y夹角 26.6°左上至右下覆盖预测 （没用到 L）
static void predict_4x4_vr(uint8_t *src)     
{
	const int lt = src[-1-FDEC_STRIDE];//-----M
	PREDICT_4x4_LOAD_LEFT              //-------------I、J、K、L
	PREDICT_4x4_LOAD_TOP               //--------------A、B、C、D
	src[0*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+1]= ( lt + t0 + 1 ) >> 1; //---a=j=（M+A+1）/2

	src[0*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+2]= ( t0 + t1 + 1 ) >> 1; //---e=k=（A+B+1）/2

	src[0*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+3]= ( t1 + t2 + 1 ) >> 1;  //---c=i=（B+C+1）/2

	src[0*FDEC_STRIDE+3]= ( t2 + t3 + 1 ) >> 1;  //---d=（C+D+1）/2
	src[1*FDEC_STRIDE+0]=
	src[3*FDEC_STRIDE+1]= ( l0 + 2 * lt + t0 + 2 ) >> 2; //---e=n=（I+2M+A+2）/4

	src[1*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+2]= ( lt + 2 * t0 + t1 + 2 ) >> 2; //---f=o=（M+2A+B+2）/4

	src[1*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+3]= ( t0 + 2 * t1 + t2 + 2) >> 2; //---g=p=（A+2B+C+2）/4

	src[1*FDEC_STRIDE+3]= ( t1 + 2 * t2 + t3 + 2 ) >> 2; //---h=（B+2C+D+2）/4

	src[2*FDEC_STRIDE+0]= ( lt + 2 * l0 + l1 + 2 ) >> 2; //---i=（M+2I+J+2）/4

	src[3*FDEC_STRIDE+0]= ( l0 + 2 * l1 + l2 + 2 ) >> 2; //---m=（I+2J+K+2）/4
}

 //与 x夹角 26.6°左上至右下覆盖预测（没用到D）
static void predict_4x4_hd(uint8_t *src)  
{
	const int lt= src[-1-1*FDEC_STRIDE];   //-----M
	PREDICT_4x4_LOAD_LEFT                   //-------------I、J、K、L
	PREDICT_4x4_LOAD_TOP                   //--------------A、B、C、D
	src[0*FDEC_STRIDE+0]=
	src[1*FDEC_STRIDE+2]= ( lt + l0 + 1 ) >> 1; //---a=g=（M+I+1）/2

	src[0*FDEC_STRIDE+1]=
	src[1*FDEC_STRIDE+3]= ( l0 + 2 * lt + t0 + 2 ) >> 2;  //---b=h=（I+2M+A+2）/4

	src[0*FDEC_STRIDE+2]= ( lt + 2 * t0 + t1 + 2 ) >> 2;  //---c=（M+2A+B+2）/4

	src[0*FDEC_STRIDE+3]= ( t0 + 2 * t1 + t2 + 2 ) >> 2;  //---d=（A+2B+C+2）/4

	src[1*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+2]= ( l0 + l1 + 1 ) >> 1;           //---e=k=（I+J+1）/2

	src[1*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+3]= ( lt + 2 * l0 + l1 + 2 ) >> 2;  //---f=l=（M+2I+J+2）/4

	src[2*FDEC_STRIDE+0]=
	src[3*FDEC_STRIDE+2]= ( l1 + l2+ 1 ) >> 1;            //---i=o=（J+K+1）/2

	src[2*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+3]= ( l0 + 2 * l1 + l2 + 2 ) >> 2;  //---j=p=（I+2J+K+2）/4
	src[3*FDEC_STRIDE+0]= ( l2 + l3 + 1 ) >> 1;            //---m=（K+L+1）/2
	src[3*FDEC_STRIDE+1]= ( l1 + 2 * l2 + l3 + 2 ) >> 2;   //---n=（J+2K+L+2）/4
}

// 与y夹角 26.6°右上至左下覆盖预测（没用到H）
static void predict_4x4_vl(uint8_t *src)     
{
	PREDICT_4x4_LOAD_TOP                 //--------------A、B、C、D
	PREDICT_4x4_LOAD_TOP_RIGHT          //----E、F、G、H
	src[0*FDEC_STRIDE+0]= ( t0 + t1 + 1 ) >> 1;           //---a=（A+B+1）/2

	src[0*FDEC_STRIDE+1]=
	src[2*FDEC_STRIDE+0]= ( t1 + t2 + 1 ) >> 1;           //---b=i=（B+C+1）/2

	src[0*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+1]= ( t2 + t3 + 1 ) >> 1;            //---c=j=（C+D+1）/2

	src[0*FDEC_STRIDE+3]=
	src[2*FDEC_STRIDE+2]= ( t3 + t4 + 1 ) >> 1;           //---d=k=（D+E+1）/2

	src[2*FDEC_STRIDE+3]= ( t4 + t5 + 1 ) >> 1;           //---l=（E+F+1）/2

	src[1*FDEC_STRIDE+0]= ( t0 + 2 * t1 + t2 + 2 ) >> 2;  //---e=（A+2B+C+2）/4

	src[1*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+0]= ( t1 + 2 * t2 + t3 + 2 ) >> 2;  //---f=m=（B+2C+D+2）/4

	src[1*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+1]= ( t2 + 2 * t3 + t4 + 2 ) >> 2;  //---g=n=（C+2D+E+2）/4

	src[1*FDEC_STRIDE+3]=
	src[3*FDEC_STRIDE+2]= ( t3 + 2 * t4 + t5 + 2 ) >> 2;  //---h=o=（D+2E+F+2）/4

	src[3*FDEC_STRIDE+3]= ( t4 + 2 * t5 + t6 + 2 ) >> 2;  //---p=（E+2F+G+2）/4
}

//与 x夹角 26.6°左下至右上覆盖预测
static void predict_4x4_hu(uint8_t *src)    
{
	PREDICT_4x4_LOAD_LEFT                       //-------------I、J、K、L
	src[0*FDEC_STRIDE+0]= ( l0 + l1 + 1 ) >> 1; //-a=（I+J+1）/2
	src[0*FDEC_STRIDE+1]= ( l0 + 2 * l1 + l2 + 2 ) >> 2;  //---b=（I+2J+K+2）/4

	src[0*FDEC_STRIDE+2]=
	src[1*FDEC_STRIDE+0]= ( l1 + l2 + 1 ) >> 1;    //---c=e=（J+K+1）/2

	src[0*FDEC_STRIDE+3]=
	src[1*FDEC_STRIDE+1]= ( l1 + 2*l2 + l3 + 2 ) >> 2;  //---d=f=（J+2K+L+2）/4

	src[1*FDEC_STRIDE+2]=
	src[2*FDEC_STRIDE+0]= ( l2 + l3 + 1 ) >> 1;       //---g=i=（K+L+1）/2

	src[1*FDEC_STRIDE+3]=
	src[2*FDEC_STRIDE+1]= ( l2 + 2 * l3 + l3 + 2 ) >> 2;  //---h=j=（K+3L+2）/4

	src[2*FDEC_STRIDE+3]=
	src[3*FDEC_STRIDE+1]=
	src[3*FDEC_STRIDE+0]=
	src[2*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+2]=
	src[3*FDEC_STRIDE+3]= l3;                              //---k=l=m=n=o=p=L
}

// 对各个图像块按12种预测模式进行预测，选出最好的一种得到残差
void predict(short **img, double **pre, double **resi, int height, int width)
{
	for (int i_row = 0; i_row < ROWS; ++i_row)
	{
		for (int i_col = 0; i_col < COLS; ++i_col)
		{
			for(int mode = 0; mode < 10; ++mode)
			{
				//将输入图像读成8x8图像块（边界扩展后），赋值给用来存储每一种预测模式输入图像的数组
				for (int i = 0; i < BLOCKHEIGHT; ++i)
				{
					for (int j = 0; j < BLOCKWIDTH; ++j)
					{
						smallimage[i_row][i_col][i][j] = img[(i_row * BLOCKHEIGHT + i) * width + i_col * BLOCKWIDTH + j];
						dc_left_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						dc_top_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						dc_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						h_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						v_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						ddl_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						ddr_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						vr_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						hd_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						vl_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
						hu_image[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j];
					}
				}
				//分别按照11种模式对图像块进行预测
				predict_4x4_dc_left((uint8_t *)(dc_left_image[i_row][i_col]));
				predict_4x4_dc_top((uint8_t *)(dc_top_image[i_row][i_col]));
				predict_4x4_dc((uint8_t *)(dc_image[i_row][i_col]));
				predict_4x4_h((uint8_t *)(h_image[i_row][i_col]));
				predict_4x4_v((uint8_t *)(v_image[i_row][i_col]));
				predict_4x4_ddl((uint8_t *)(ddl_image[i_row][i_col]));
				predict_4x4_ddr((uint8_t *)(ddr_image[i_row][i_col]));
				predict_4x4_vr((uint8_t *)(vr_image[i_row][i_col]));
				predict_4x4_hd((uint8_t *)(hd_image[i_row][i_col]));
				predict_4x4_hl((uint8_t *)(hl_image[i_row][i_col]));
				predict_4x4_hu((uint8_t *)(hu_image[i_row][i_col]));

				//计算对每一种预测模式的预测残差，并比较残差能量，将能量最低的预测模式下的残差保留继续进行变换、量化、逆量化、逆变换，结果再与
				//预测值进行相加得到重建块，重建块的某些值作为下一个块的邻近参考像素值
				for (int i = 4; i < smallheight; ++i)
				{
					for (int j = 4; j < smallwidth; ++j)
					{
						resi_dc_left[i_row][i_col][i][j] = smallimage[i_row][i_col][i][j] - dc_left_image[i_row][i_col][i][j];//预测残差
						R_dc_left += resi_dc_left[i_row][i_col][i][j] * resi_dc_left[i_row][i_col][i][j];
					}
				}
			}
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