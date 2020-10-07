#include "stdafx.h"
/*
 * 如果你下载或使用本代码说明你已阅读并同意本声明
 * 此代码由谭升（Tony）开发，并允许任何人，或团体下载，使用此代码用于任何商业或非商业用途
 * 使用本代码时必须复制此声明
 * 本函数库版权归谭升所有.
 * 如有第三方，例如部分使用OpenCV函数，OpenCV函数库版权属于Intel公司，将在后续版本中去除这些函数，特此声明
 
 * If you download or use the code that you have read and agree to this statement,
 * This code by Tan Sheng (Tony) development, and allow any person, or group to download,
 * use for any commercial or non-commercial use
 * Use the code must be copied to this statement
 * Copyright (C) 2015,Tony, all rights reserved.
 * Part of the use of the OpenCV function, OpenCV function library copyright belongs
 * to Intel company, will remove these functions in subsequent versions, hereby declare
 */
//
//  region segment
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/05.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "regionSegment.h"

/****************************************************************************/
/****************************************************************************/
/*区域生长，设置一个种子点x（灰度值为x_v），然后以种子点为中心
 *向四周进行图搜索，如果点y（灰度值为y_v）,邻域满足param+x_v>y_v>x_v-param（条件1）
 *则此点与种子点归为一个区域，以此递归，条件1可以根据不同的需要自行设置其他.
 */
//递归遍历邻域，并判断条件是否成立
void findGrowRegion(double *src,double *dst,int seed_x,int seed_y,int width,int height,int regionNum,double value,double param){
    dst[seed_y*width+seed_x]=(double)regionNum;
    for(int j=-1;j<2;j++)
        for(int i=-1;i<2;i++){
            if(seed_x>=0&&seed_y>=0&&seed_x<width&&seed_y<height&&(j!=0||i!=0)){
                if(src[(j+seed_y)*width+i+seed_x]>=value-param
                   &&src[(j+seed_y)*width+i+seed_x]<=value+param
                   &&dst[(j+seed_y)*width+i+seed_x]==0.0)
                    findGrowRegion(src, dst, i+seed_x, j+seed_y, width, height, regionNum, value,param);
            }
        }
}
void RegionGrow(double *src,double *dst,Position * position,int p_size,int width,int height,double param){
    double * dsttemp=(double *)malloc(sizeof(double)*width*height);
    Zero(dsttemp, width, height);
    int regionNum=100;
    
    for(int i=0;i<p_size;i++){
        int x=position[i].x;
        int y=position[i].y;
        findGrowRegion(src, dsttemp, x,y, width, height, regionNum,src[y*width+x],param);
        regionNum+=10;
    }
    
    matrixCopy(dsttemp, dst, width, height);
    free(dsttemp);
}


/****************************************************************************/
/****************************************************************************/
/*
 *
 *
 */
//区域均值
double RegionMean(double *src,int width,int height,int x,int y,int w_width,int w_height){
    double sum=0.0;
    double pix_num=0.0;
    for(int j=y;j<=y+w_height;j++)
        for(int i=x;i<=x+w_width;i++){
            if(j>=0&&j<height&&i>=0&&i<width){
                sum+=src[j*width+i];
                pix_num++;
            }
        }
    if(pix_num==0.0)
        return 0.0;
    return sum/pix_num;
}
//区域标准差
double RegionStdDeviation(double *src,int width,int height,int x,int y,int w_width,int w_height){
    double sum=0.0;
    double pix_num=0.0;
    for(int j=y;j<y+w_height;j++)
        for(int i=x;i<x+w_width;i++){
            if(j>=0&&j<height&&i>=0&&i<width){
                sum+=src[j*width+i];
                pix_num++;
            }
        }
    if(pix_num==0.0)
        return 0.0;
    else{
        double mean=sum/pix_num;
        for(int j=y;j<y+w_height;j++)
            for(int i=x;i<x+w_width;i++){
                if(j>=0&&j<height&&i>=0&&i<width){
                    sum+=(src[j*width+i]-mean)*(src[j*width+i]-mean);
                }
            }
    }
    return sqrt(sum/pix_num);


}
//矩阵局部地区设置为1
void RegionSetOne(double *src,int width,int height,int x,int y,int w_width,int w_height){
    for(int j=y;j<y+w_height;j++)
        for(int i=x;i<x+w_width;i++){
            if(j>=0&&j<height&&i>=0&&i<width){
                src[j*width+i]=255.0;
            }
        }
}

/*******************************************************************************/
/*
 *区域分割算法，递归进行判断
 *如果区域不符合条件，将区域
 *分为四份，递归判断每个区域
 *知道区域分为最小设定值
 */

void findSplitRegion(double *src,double *dst,int width,int height,int x,int y,int w_width,int w_height,double mean_param1,double mean_param2,double variance_param1,double variance_param2){
    double mean=RegionMean(src, width, height, x, y, w_width, w_height);
    double variance=RegionStdDeviation(src, width, height, x, y, w_width, w_height);
    if(mean>mean_param1&&
       mean<=mean_param2&&
       variance>variance_param1&&
       variance<=variance_param2){
        RegionSetOne(dst, width, height, x, y, w_width, w_height);
    }else{
#define MINIMAL_CELL 3
        if(w_width>=MINIMAL_CELL&&w_height>=MINIMAL_CELL){
            findSplitRegion(src, dst, width,height, x, y, w_width/2+1, w_height/2+1, mean_param1, mean_param2, variance_param1, variance_param2);
            findSplitRegion(src, dst, width,height, x+w_width/2, y, w_width/2+1, w_height/2+1, mean_param1, mean_param2, variance_param1, variance_param2);
            findSplitRegion(src, dst, width,height, x+w_width/2, y+w_height/2+1, w_width/2+1, w_height/2, mean_param1, mean_param2, variance_param1, variance_param2);
            findSplitRegion(src, dst, width,height, x, y+w_height/2, w_width/2+1, w_height/2+1, mean_param1, mean_param2, variance_param1, variance_param2);
        }
    }
}

void RegionSplit(double *src,double *dst,int width,int height,double mean_param1,double mean_param2,double variance_param1,double variance_param2){
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    findSplitRegion(src, dsttemp, width, height, 0,0, width, height, mean_param1, mean_param2, variance_param1,variance_param2);
    matrixCopy(dsttemp, dst, width, height);
    free(dsttemp);

}