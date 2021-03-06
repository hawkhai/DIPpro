//
//  二维DFT和FFT
//  Fourer
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/25.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "Fourer.h"
/*
 
 
 */
int DFT2D(double *src,Complex *dst,int size_w,int size_h){
    for(int u=0;u<size_w;u++){
        for(int v=0;v<size_h;v++){
            double real=0.0;
            double imagin=0.0;
            for(int i=0;i<size_w;i++){
                for(int j=0;j<size_h;j++){
                    double I=src[i*size_w+j];
                    double x=M_PI*2*((double)i*u/(double)size_w+(double)j*v/(double)size_h);
                    real+=cos(x)*I;
                    imagin+=-sin(x)*I;
                    
                }
            }
            dst[u*size_w+v].real=real;
            dst[u*size_w+v].imagin=imagin;

        }

    }
    return 0;
}
/*
 
 */
int IDFT2D(Complex *src,Complex *dst,int size_w,int size_h){
    for(int i=0;i<size_w;i++){
        for(int j=0;j<size_h;j++){
            double real=0.0;
            double imagin=0.0;
            for(int u=0;u<size_w;u++){
                for(int v=0;v<size_h;v++){
                    double R=src[u*size_w+v].real;
                    double I=src[u*size_w+v].imagin;
                    double x=M_PI*2*((double)i*u/(double)size_w+(double)j*v/(double)size_h);
                    real+=R*cos(x)-I*sin(x);
                    imagin+=I*cos(x)+R*sin(x);
                    
                }
            }
            dst[i*size_w+j].real=(1./(size_w*size_h))*real;
            dst[i*size_w+j].imagin=(1./(size_w*size_h))*imagin;
            
        }
    }
    return 0;
}
/*
 
 
 
 */
void ColumnVector(Complex * src,Complex * dst,int size_w,int size_h){
    for(int i=0;i<size_h;i++)
        Copy_Complex(&src[size_w*i], &dst[i]);
    
}
/*
 
 */
void IColumnVector(Complex * src,Complex * dst,int size_w,int size_h){
    for(int i=0;i<size_h;i++)
        Copy_Complex(&src[i],&dst[size_w*i]);
    
}
/*
 */
int FFT2D(double *src,Complex *dst,int size_w,int size_h){
    if(isBase2(size_w)==-1||isBase2(size_h)==-1)
        exit(0);
    Complex *temp=(Complex *)malloc(sizeof(Complex)*size_h*size_w);
    if(temp==NULL)
        return -1;
    for(int i=0;i<size_h;i++){
        RealFFT(&src[size_w*i], &temp[size_w*i], size_w);
    }

    Complex *Column=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL)
        return -1;
    for(int i=0;i<size_w;i++){
        ColumnVector(&temp[i], Column, size_w, size_h);
        FFT(Column, Column, size_h);
        IColumnVector(Column, &temp[i], size_w, size_h);
        
    }
    
    
    
    for(int i=0;i<size_h*size_w;i++)
        Copy_Complex(&temp[i], &dst[i]);
    free(temp);
    free(Column);
    return 0;
}
/*
 */
int IFFT2D(Complex *src,Complex *dst,int size_w,int size_h){
    
    if(isBase2(size_w)==-1||isBase2(size_h)==-1)
        exit(0);
    
    Complex *temp=(Complex *)malloc(sizeof(Complex)*size_h*size_w);
    if(temp==NULL)
        return -1;
    Complex *Column=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL)
        return -1;
    
    for(int i=0;i<size_w;i++){
        ColumnVector(&src[i], Column, size_w, size_h);
        IFFT(Column, Column, size_h);
        IColumnVector(Column, &src[i], size_w, size_h);
        
    }
    for(int i=0;i<size_w*size_h;i++)
        src[i].imagin=-src[i].imagin;
    for(int i=0;i<size_h;i++){
        IFFT(&src[size_w*i], &temp[size_w*i], size_w);
        
    }
    
    
    for(int i=0;i<size_h*size_w;i++)
        Copy_Complex(&temp[i], &dst[i]);
    free(temp);
    free(Column);
    return 0;
    
}
