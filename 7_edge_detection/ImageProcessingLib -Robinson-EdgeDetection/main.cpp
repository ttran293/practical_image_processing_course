#include "ImageProcessing.h"

using namespace std;

int main()
{


    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgInBuffer[_512by512_IMG_SIZE];
    unsigned char imgOutBuffer[_512by512_IMG_SIZE];

    const char imgName[] ="images/lena512.bmp";
    const char newImgName[] ="images/lena_ronson_wst.bmp";

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
     myImage->setMask(3,3,ROBINSON_WST);
     myImage->Convolve2D(imgHeight,imgWidth,&myImage->myMask,imgInBuffer,imgOutBuffer);
     myImage->writeImage();


     cout<<"Success !"<<endl;
     cout<<"Image Height : "<<imgHeight<<endl;
     cout<<"Image Width  : "  <<imgWidth<<endl;

    return 0;
}
