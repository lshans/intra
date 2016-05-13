#ifndef COMMON_H
#define COMMON_H

extern short block_012pre[4][4];			 //每一个小块的预测值
extern short block_pre3[4][4];				 //每一个小块的预测值
extern short block_pre4[4][4];				 //每一个小块的预测值
extern short block_pre5[4][4];				 //每一个小块的预测值
extern short block_pre6[4][4];				 //每一个小块的预测值
extern short block_pre7[4][4];				 //每一个小块的预测值
extern short block_pre8[4][4];				 //每一个小块的预测值
extern int count_para_mode[1][9];
extern int count[9];

struct block{
	long long min_energy;
	int       modeMinRes;
	short     min_resi[4][4];
	short     min_pre[4][4];
};
extern struct block blocktab[256][256];

#endif