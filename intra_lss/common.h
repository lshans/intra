#ifndef COMMON_H
#define COMMON_H

extern short block_012pre[4][4];			 //每一个小块的预测值
extern short block_pre3[4][4];				 //每一个小块的预测值
extern short block_pre4[4][4];				 //每一个小块的预测值
extern short block_pre5[4][4];				 //每一个小块的预测值
extern short block_pre6[4][4];				 //每一个小块的预测值
extern short block_pre7[4][4];				 //每一个小块的预测值
extern short block_pre8[4][4];				 //每一个小块的预测值
extern int count_para_mode[4][10];
extern int count[9];
extern int  para_num[4];											//标记不同参数个数下预测最优的图像块数

struct block{
	int       min_energy;
	int       modeMinRes;
	short     min_resi[4][4];
	short     min_pre[4][4];
};
extern struct block blocktab[ROWS][COLS];

#endif