typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

const int FDEC_STRIDE = 9 * 8;


const int BLOCKWIDTH = 4;		// 小图的宽度
const int BLOCKHEIGHT = 4;		// 小图的高度
const int ROWS = 128;	// 将图像分成小块，每一行的图像块数目
const int COLS = 128;	// 将图像分成小块，每一列的图像块数目
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


//本函数对4*4块的每行像素赋同样的值,给第一行的4 个像素赋值,给第二行的4 个像素赋值,给第三行的4 个像素赋值,给第三行的4 个像素赋值
#define PREDICT_4x4_DC(v)\
{\
	*(uint32_t*)&src[0*FDEC_STRIDE] =\
	*(uint32_t*)&src[1*FDEC_STRIDE] =\
	*(uint32_t*)&src[2*FDEC_STRIDE] =\
	*(uint32_t*)&src[3*FDEC_STRIDE] = v;\
}

//该定义表示依次取出当前宏块左边的 4个像素,l0 是第一行左边像素I,l1 是第二行左边像素J,l2 是第三行左边像素K,l3 是第四行左边像素L
#define PREDICT_4x4_LOAD_LEFT \
	const int l0 = src[-1+0*FDEC_STRIDE];\
	const int l1 = src[-1+1*FDEC_STRIDE];\
	const int l2 = src[-1+2*FDEC_STRIDE];\
	const int l3 = src[-1+3*FDEC_STRIDE];

//该定义表示依次取出当前宏块上方的 4个像素,----t0 是第一列上方的像素A, t1 是第二列上方的像素B, t2 是第三列上方的像素C,t3 是第四列上方的像素D
#define PREDICT_4x4_LOAD_TOP \
	const int t0 = src[0-1*FDEC_STRIDE]; \
	const int t1 = src[1-1*FDEC_STRIDE]; \
	const int t2 = src[2-1*FDEC_STRIDE]; \
	const int t3 = src[3-1*FDEC_STRIDE]; 

//该定义表示依次取出当前宏块右上方的 4个像素,t4 是右上方第一个像素E,t5 是右上方第二个像素F,t6 是右上方第三个像素G,t7 是右上方第四个像素H
#define PREDICT_4x4_LOAD_TOP_RIGHT \
	const int t4 = src[4-1*FDEC_STRIDE];\
	const int t5 = src[5-1*FDEC_STRIDE];\
	const int t6 = src[6-1*FDEC_STRIDE];\
	const int t7 = src[7-1*FDEC_STRIDE]; 

short predict(uint8_t image_construct[1025][1029], short resi[1024][1024], int height, int width);
void DCTCore4x4(uint8_t block[4][4], uint8_t tblock[4][4]);
void IDCTCore4x4(uint8_t tblock[4][4], uint8_t block[4][4]);
