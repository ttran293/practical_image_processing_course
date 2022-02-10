#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

 static const int   _512by512_IMG_SIZE  = 262144; // 512*512
 static const int   BMP_COLOR_TABLE_SIZE= 1024;
 static const int   BMP_HEADER_SIZE     = 54;
 static const int   MAX_COLOR           = 255;
 static const int   MIN_COLOR           =  0;
 static const int   WHITE               =  MAX_COLOR;
 static const int   BLACK               =  MIN_COLOR;
 static const int   NO_OF_GRAYLEVELS     = 255;


 struct Mask{
 int Rows;
 int Cols;
 unsigned char *Data;
 };


 static const int LINE_DETECTOR_HOR_MSK[3][3] = {{-1,2,-1},
                                                 {-1,2,-1},
                                                 {-1,2,-1}
                                                 };

static const int LINE_DETECTOR_VER_MSK[3][3]   =  {{-1,-1,-1},
                                                   {2, 2, 2},
                                                   {-1,-1,-1}
                                                 };

static const int LINE_DETECTOR_LDIA_MSK[3][3] = {{2,-1,-1},
                                                   {-1,2,-1},
                                                   {-1,-1,2}
                                                   };
 static const int LINE_DETECTOR_RDIA_MSK[3][3] = {{-1,-1,2},
                                                  {-1,2,-1},
                                                  {2,-1,-1}
                                                 };

/*Prewitt Operator - Vertical Edges*/
/*
   -1  0   1
   -1  0   1
   -1  0   1
*/
static const int PREWITT_VER[] ={-1,0,1,-1,0,1,-1,0,1};

/*Prewitt Operator - Horizontal Edges*/
/*
   -1  -1  -1
    0   0   0
   -1   -1 -1
*/
static const int PREWITT_HOR[] ={-1,-1,-1,0,0,0,1,1,1};

/*Sobel Operator -  Vertical Edges*/
/* -1  0  1
   -2  0  2
   -1  0  1*/
 static const int SOBEL_VER[] {-1,0,1,-2,0,2,-1,0,1};
/*Sobele Operator -  Horizontal Edges*/
/*  -1  -2  -1
     0   0   0
     1   2   1*/

 static const int SOBEL_HOR[] ={-1,-2,-1,0,0,0,1,2,1};

 //Robinson Operator

 static const int ROBINSON_NTH[] ={-1,0,1,-2,0,2,-1,0,1};
 static const int ROBINSON_STH[] ={1,0,-1,2,0,-2,1,0,-1};
 static const int ROBINSON_WST[] ={1,2,1,0,0,0,-1,-2,-1};
 static const int ROBINSON_EST[] ={-1,-2,-1,0,0,0,1,2,1};

class ImageProcessing
{
    public:
        ImageProcessing(
                                 const char *_inImgName,
                                 const char *_outImgName,
                                 int * _height,
                                 int * _width,
                                 int * _bitDepth,
                                 unsigned char * _header,
                                 unsigned char * _colorTable,
                                 unsigned char * _inBuf,
                                 unsigned char * _outBuf

                                 );
        Mask myMask;

        void readImage();
        void writeImage();
        void copyImgData(unsigned char *_srcBuf, unsigned char *_destBuf, int bufSize);
        void binarizeImage(unsigned char *_inImgData, unsigned char *_outImgData, int imgSize, int threshold);
        void brigthnessUp(unsigned char *_inputImgData, unsigned char *_outImgData, int imgSize, int brightness);
        void brigthnessDown(unsigned char *_inputImgData, unsigned char *_outImgData, int imgSize, int darkness);
        void computeHistogram(unsigned char * _imgData, int imgRows, int imgCols, float hist[]);
        void computeHistogram2(unsigned char * _imgData, int imgRows, int imgCols, float hist[],const char *histFile);
        void equalizeHistogram(unsigned char * _inputImgData, unsigned char * _outputImgData, int imgRows, int imgCols);
        void getImageNegative(unsigned char *_inImgData, unsigned char * _outImgData,int imgWidth,int imgHeight);
        void Convolve2D(int imgRows, int imgCols, struct Mask *myMask, unsigned char *input_buf, unsigned char *output_buf);
        void  detectLine(unsigned char *_inputImgData, unsigned char *_outputImgData, int imgCols, int imgRows, const int MASK[][3]);
        void setMask(int mskRows, int mskCols, const int mskData[]);

        virtual ~ImageProcessing();

    protected:

    private:
        const char *inImgName;
        const char *outImgName;
        int * height;
        int * width;
        int * bitDepth;
        unsigned char * header;
        unsigned char * colorTable;
        unsigned char * inBuf;
        unsigned char * outBuf;
};

#endif // IMAGEPROCESSING_H
