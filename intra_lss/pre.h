#ifndef PRE_H
#define PRE_H


typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

const int BLOCKWIDTH = 4;		// 小图的宽度
const int BLOCKHEIGHT = 4;		// 小图的高度
const int ROWS = 256;	// 将图像分成小块，每一行的图像块数目
const int COLS = 256;	// 将图像分成小块，每一列的图像块数目

void estimate_one_para(short block_image[10][10], double **para, int mode);
void estimate_two_para(short block_image[10][10], double **para, int mode);
void estimate_three_para(short block_image[10][10], double **para, int mode);
void estimate_four_para(short block_image[10][10], double **para, int mode);

long long predict(short **image_construct, short **resi, short **predicted, int height, int width);
void predict_one_para(short block_image[10][10], short resi[4][4], double **para, int mode);
void predict_two_para(short block_image[10][10], short resi[4][4], double **para, int mode);
void predict_three_para(short block_image[10][10], short resi[4][4], double **para, int mode);
void predict_four_para(short block_image[10][10], short resi[4][4], double **para, int mode);

long long predict_1_parameter(short block_image[8][8], short resi[4][4], short block_pre[4][4], int i_row, int i_col);
long long predict_2_parameter(short block_image[8][8], short resi[4][4], short block_pre[4][4], int i_row, int i_col);
long long predict_3_parameter(short block_image[8][8], short resi[4][4], short block_pre[4][4], int i_row, int i_col);
long long predict_4_parameter(short block_image[8][8], short resi[4][4], short block_pre[4][4], int i_row, int i_col);
struct block predict_1_parameter(short block_image[10][10], short resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256]);
struct block predict_2_parameter(short block_image[10][10], short resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256]);
struct block predict_3_parameter(short block_image[10][10], short resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256]);
struct block predict_4_parameter(short block_image[10][10], short resi[4][4], short block_pre[4][4], int i_row, int i_col, block blocktab[][256]);

void DCTCore4x4(uint8_t block[4][4], uint8_t tblock[4][4]);
void IDCTCore4x4(uint8_t tblock[4][4], uint8_t block[4][4]);

#endif