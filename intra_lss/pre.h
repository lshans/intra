typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

const int FDEC_STRIDE = 9 * 8;


const int BLOCKWIDTH = 4;		// Сͼ�Ŀ��
const int BLOCKHEIGHT = 4;		// Сͼ�ĸ߶�
const int ROWS = 128;	// ��ͼ��ֳ�С�飬ÿһ�е�ͼ�����Ŀ
const int COLS = 128;	// ��ͼ��ֳ�С�飬ÿһ�е�ͼ�����Ŀ
static void predict_4x4_dc_128(uint8_t *src);
static void predict_4x4_dc_left(uint8_t *src);
static void predict_4x4_dc_top(uint8_t *src);
static void predict_4x4_dc(uint8_t *src);
static void predict_4x4_h(uint8_t *src);
static void predict_4x4_v(uint8_t *src);
static void predict_4x4_ddl(uint8_t *src);
static void predict_4x4_ddr(uint8_t *src);
static void predict_4x4_vr(uint8_t *src);
static void predict_4x4_hd(uint8_t *src);
static void predict_4x4_vl(uint8_t *src);
static void predict_4x4_hu(uint8_t *src);


//��������4*4���ÿ�����ظ�ͬ����ֵ,����һ�е�4 �����ظ�ֵ,���ڶ��е�4 �����ظ�ֵ,�������е�4 �����ظ�ֵ,�������е�4 �����ظ�ֵ
#define PREDICT_4x4_DC(v)\
{\
	*(uint32_t*)&src[0*FDEC_STRIDE] =\
	*(uint32_t*)&src[1*FDEC_STRIDE] =\
	*(uint32_t*)&src[2*FDEC_STRIDE] =\
	*(uint32_t*)&src[3*FDEC_STRIDE] = v;\
}

//�ö����ʾ����ȡ����ǰ�����ߵ� 4������,l0 �ǵ�һ���������I,l1 �ǵڶ����������J,l2 �ǵ������������K,l3 �ǵ������������L
#define PREDICT_4x4_LOAD_LEFT \
	const int l0 = src[-1+0*FDEC_STRIDE];\
	const int l1 = src[-1+1*FDEC_STRIDE];\
	const int l2 = src[-1+2*FDEC_STRIDE];\
	const int l3 = src[-1+3*FDEC_STRIDE];

//�ö����ʾ����ȡ����ǰ����Ϸ��� 4������,----t0 �ǵ�һ���Ϸ�������A, t1 �ǵڶ����Ϸ�������B, t2 �ǵ������Ϸ�������C,t3 �ǵ������Ϸ�������D
#define PREDICT_4x4_LOAD_TOP \
	const int t0 = src[0-1*FDEC_STRIDE]; \
	const int t1 = src[1-1*FDEC_STRIDE]; \
	const int t2 = src[2-1*FDEC_STRIDE]; \
	const int t3 = src[3-1*FDEC_STRIDE]; 

//�ö����ʾ����ȡ����ǰ������Ϸ��� 4������,t4 �����Ϸ���һ������E,t5 �����Ϸ��ڶ�������F,t6 �����Ϸ�����������G,t7 �����Ϸ����ĸ�����H
#define PREDICT_4x4_LOAD_TOP_RIGHT \
	const int t4 = src[4-1*FDEC_STRIDE];\
	const int t5 = src[5-1*FDEC_STRIDE];\
	const int t6 = src[6-1*FDEC_STRIDE];\
	const int t7 = src[7-1*FDEC_STRIDE]; 

short predict(uint8_t image_construct[1025][1029], short resi[1024][1024], int height, int width);
void DCTCore4x4(uint8_t block[4][4], uint8_t tblock[4][4]);
void IDCTCore4x4(uint8_t tblock[4][4], uint8_t block[4][4]);
