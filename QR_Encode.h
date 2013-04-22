/*
 * QR_Encode.h
 *
 *  Created on: Jan 18, 2012
 *      Original Author: swex
 *  Forked on 21 April 2013
 *      Updates by ksasq
 */

#include <string.h>
#include <stdlib.h>

// Error correction level
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

//Data Mode
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

//Group version (model number)
#define QR_VERSION_XS 0 // 1 ~ 3
#define QR_VERSION_S  1 // 4 ~ 9
#define QR_VERSION_M  2 // 10 ~ 26
#define QR_VERSION_L  3 // 27 ~ 40

// Users should pass -DMAX_REQUIRED_QR_VERSION=X where X is the desired maximum
// supported QR version. Must be in the range of 1~40.
#ifndef MAX_REQUIRED_QR_VERSION
#define MAX_REQUIRED_QR_VERSION 40
#endif

#if MAX_REQUIRED_QR_VERSION <= 0
#error "Minimum QR Version is 1"
#elif MAX_REQUIRED_QR_VERSION <= 3
#define MAX_QR_VERSION_GROUP QR_VERSION_XS
#define MAX_DATACODEWORD 55  // Maximum data word code (version 3-L)
#define MAX_ALLCODEWORD  70
#define MAX_CODEBLOCK   70
#elif MAX_REQUIRED_QR_VERSION <= 9
#define MAX_QR_VERSION_GROUP QR_VERSION_S
#define MAX_DATACODEWORD 232  // Maximum data word code (version 9-L)
#define MAX_ALLCODEWORD  292
#define MAX_CODEBLOCK   146
#elif MAX_REQUIRED_QR_VERSION <= 26
#define MAX_QR_VERSION_GROUP QR_VERSION_M
#define MAX_DATACODEWORD 1370  // Maximum data word code (version 26-L)
#define MAX_ALLCODEWORD  1706
#define MAX_CODEBLOCK   152
#elif MAX_REQUIRED_QR_VERSION <= 40
#define MAX_QR_VERSION_GROUP QR_VERSION_L
#define MAX_DATACODEWORD 2956  // Maximum data word code (version 40-L)
#define MAX_ALLCODEWORD  3706  // The maximum total number of code words
#define MAX_CODEBLOCK   153 //(Including RS code word) the maximum number of block data code word
#else
#error "Maximum QR Version is 40"
#endif

#define MAX_MODULESIZE ((MAX_REQUIRED_QR_VERSION * 4) + 17) // Maximum number of modules in a side
#define MAX_BITDATA    (((MAX_MODULESIZE * MAX_MODULESIZE) / 8) + 1)

//Margin when drawing a bitmap
//#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////

typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		//RS block number
	int ncAllCodeWord;	//The number of codewords in the block
	int ncDataCodeWord;	//The number of data code words (the number of code words - the number of RS code word)

} RS_BLOCKINFO, *LPRS_BLOCKINFO;
#define INIT_RS_BLOCK_INFO(RSBlockCount, allCodeWordCount, dataCodewordCount) \
  { \
    .ncRSBlock = RSBlockCount, \
    .ncAllCodeWord = allCodeWordCount, \
    .ncDataCodeWord = dataCodewordCount \
  }


/////////////////////////////////////////////////////////////////////////////
//Version code-related information (model number)

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;
	int ncAllCodeWord;

	// Error correction levels (0 = L, 1 = M, 2 = Q, 3 = H)
	int ncDataCodeWord[4];	// data len

	int ncAlignPoint;	// position
	int nAlignPoint[6];	// numberof

	RS_BLOCKINFO RS_BlockInfo1[4]; // EC pos
	RS_BLOCKINFO RS_BlockInfo2[4]; // EC pos

} QR_VERSIONINFO, *LPQR_VERSIONINFO;

#define INIT_QR_VERSION(versionNo, allCodeWordCount, dataCW1, dataCW2, dataCW3,dataCW4, \
                        alignPointCount, alignPoint1, alignPoint2, alignPoint3, alignPoint4, alignPoint5, alignPoint6, \
                        rsBlockInfo1_1, rsBlockInfo1_2, rsBlockInfo1_3, rsBlockInfo1_4, \
                        rsBlockInfo2_1, rsBlockInfo2_2, rsBlockInfo2_3, rsBlockInfo2_4) \
  { \
    .nVersionNo = versionNo, \
    .ncAllCodeWord = allCodeWordCount, \
    .ncDataCodeWord = { dataCW1, dataCW2, dataCW3,dataCW4 }, \
    .ncAlignPoint = alignPointCount, \
    .nAlignPoint = { alignPoint1, alignPoint2, alignPoint3, alignPoint4, alignPoint5, alignPoint6 }, \
    .RS_BlockInfo1 = { rsBlockInfo1_1, rsBlockInfo1_2, rsBlockInfo1_3, rsBlockInfo1_4 }, \
    .RS_BlockInfo2 = { rsBlockInfo2_1, rsBlockInfo2_2, rsBlockInfo2_3, rsBlockInfo2_4 } \
  }

typedef unsigned short WORD;

typedef unsigned char BYTE;

typedef BYTE* LPBYTE;

typedef const char* LPCSTR;

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
int EncodeData(int nLevel, int nVersion , LPCSTR lpsSource, unsigned sourcelen,unsigned char QR_m_data[MAX_BITDATA]);

/*nLevel - уровень коррекции ошибок
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

nVersion - версия кода 1 - 40 разрешение итд 0 - автоопределение исходя из размера

lpsSource - данные на вход
sourcelen - длинна строки 0 - автоопределение, strlen \0 оборвет.
QR_m_data - массив для записи, пишет в биты.

функция возвращает сторону квадрата.
*/
