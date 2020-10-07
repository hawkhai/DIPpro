#ifndef CONTRAST_H
#define CONTRAST_H

#include <stdio.h>
#include "mathematic.h"

#define CONTRASTFUNC0 -1	//翻转
#define CONTRASTFUNC1 0		//分段
#define CONTRASTFUNC2 1		//对数
#define CONTRASTFUNC3 2		//反对数
#define CONTRASTFUNC4 3		//n次幂
#define CONTRASTFUNC5 4		//n次根
#define CONTRASTGAMA  5     //gama
#define GRAYLEVEL 8
#define MAX_PVALUE (1<<GRAYLEVEL)


#include "Contrast.h"


extern unsigned char ContrastTable[MAX_PVALUE];//映射表

void ContrastStretch(double *src,double *dst,int width,int height,int method,double p0,double p1,int p2,int p3);





#endif