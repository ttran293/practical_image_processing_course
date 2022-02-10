#include <iostream>
#include "ImageProcessing.h"

using namespace std;

int main()
{
    float imgHiSt[NO_OF_GRAYLEVELS];

    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgInBuffer[_512by512_IMG_SIZE];
    unsigned char imgOutBuffer[_512by512_IMG_SIZE];

    const char imgName[] ="images/lena512.bmp";
    const char newImgName[] ="images/blank.bmp";

    ImageProcessing *myImage  = new ImageProcessing(imgName,
                                                    newImgName,
                                                    &imgHeight,
                                                    &imgWidth,
                                                    &imgBitDepth,
                                                    &imgHeader[0],
                                                    &imgColorTable[0],
                                                    &imgInBuffer[0],
                                                    &imgOutBuffer[0]
                                                    );

     myImage->readImage();
     myImage->computeHistogram(imgInBuffer,imgHeight,imgWidth,imgHiSt);


     cout<<"Success !"<<endl;
     cout<<"Image Height : "<<imgHeight<<endl;
     cout<<"Image Width  : "  <<imgWidth<<endl;

    return 0;
}
