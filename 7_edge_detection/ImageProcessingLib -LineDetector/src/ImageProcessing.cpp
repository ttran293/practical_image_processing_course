#include "ImageProcessing.h"


using namespace std;

ImageProcessing::ImageProcessing(
                                 const char *_inImgName,
                                 const char *_outImgName,
                                 int * _height,
                                 int * _width,
                                 int * _bitDepth,
                                 unsigned char * _header,
                                 unsigned char * _colorTable,
                                 unsigned char * _inBuf,
                                 unsigned char * _outBuf

                                 )
{
    inImgName  = _inImgName;
    outImgName = _outImgName;
    height     = _height;
    width      = _width;
    bitDepth   = _bitDepth;
    header     = _header;
    colorTable = _colorTable;
    inBuf      = _inBuf;
    outBuf     = _outBuf;
}


void ImageProcessing::readImage()
{

    int i;
    FILE *streamIn;
    streamIn = fopen(inImgName,"rb");
    if(streamIn ==(FILE *)0)
    {
        cout<<"Unable to open file. Maybe file does not exist"<<endl;
        exit(0);
    }
    for(i=0;i<BMP_HEADER_SIZE;i++)
    {
        header[i] =  getc(streamIn);
    }
    *width = *(int *)&header[18];           //read the width from the image header
    *height = *(int *)&header[22];
    *bitDepth = *(int *)&header[28];

    if(*bitDepth <=8)
    {

        fread(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,streamIn);
    }

    fread(inBuf,sizeof(unsigned char),_512by512_IMG_SIZE,streamIn );

    fclose(streamIn);
}

void ImageProcessing::writeImage(){

    FILE  *fo = fopen(outImgName,"wb");
    fwrite(header,sizeof(unsigned char),BMP_HEADER_SIZE,fo);
    if(*bitDepth <=8)
    {
        fwrite(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,fo);
    }

   fwrite(outBuf, sizeof(unsigned char),_512by512_IMG_SIZE,fo);
   fclose(fo);
}

void ImageProcessing ::copyImgData(unsigned char *_srcBuf, unsigned char *_destBuf, int bufSize)
{
    for(int i =0;i<bufSize;i++)
    {
        _destBuf[i] = _srcBuf[i];
    }
}

void ImageProcessing::binarizeImage(unsigned char *_inImgData, unsigned char *_outImgData, int imgSize, int threshold)
{
    for(int i=0;i<imgSize;i++)
    {
        _outImgData[i] =  (_inImgData[i] > threshold) ? WHITE :BLACK;
    }
}
void ImageProcessing::brigthnessUp(unsigned char *_inputImgData, unsigned char *_outImgData, int imgSize, int brightness)
{

    for(int i =0;i<imgSize;i++)
    {
        int temp = _inputImgData[i]+ brightness;
        _outImgData[i] =  (temp > MAX_COLOR)? MAX_COLOR :temp;
    }
}

void ImageProcessing::brigthnessDown(unsigned char *_inputImgData, unsigned char *_outImgData, int imgSize, int darkness)
{
     for(int i =0;i<imgSize;i++)
     {
         int temp = _inputImgData[i] - darkness;
         _outImgData[i] = (temp<MIN_COLOR) ? MIN_COLOR :temp;
     }
}

void ImageProcessing::computeHistogram(unsigned char * _imgData, int imgRows, int imgCols, float hist[])
{
    FILE *fptr;
    fptr =fopen("image_hist.txt","w");

    int x,y,i,j;
    long int ihist[255],sum;
    for(i =0;i<=255;i++)
    {
        ihist[i] =0;
    }
    sum =0;
    for(y=0;y<imgRows;y++)
    {
        for(x=0;x<imgCols;x++)
        {
            j = *(_imgData+x+y*imgCols);
            ihist[j] = ihist[j] +1;
            sum = sum+1;
        }

    }
    for(i=0;i<255;i++)
        hist[i] =  (float)ihist[i]/(float)sum;
  for(int i=0;i<255;i++)
    {
        fprintf(fptr,"\n%f",hist[i]);
    }
    fclose(fptr);
}


void ImageProcessing::computeHistogram2(unsigned char * _imgData, int imgRows, int imgCols, float hist[],const char *histFile)
{
    FILE *fptr;
    fptr =fopen(histFile,"w");

    int x,y,i,j;
    long int ihist[255],sum;
    for(i =0;i<=255;i++)
    {
        ihist[i] =0;
    }
    sum =0;
    for(y=0;y<imgRows;y++)
    {
        for(x=0;x<imgCols;x++)
        {
            j = *(_imgData+x+y*imgCols);
            ihist[j] = ihist[j] +1;
            sum = sum+1;
        }

    }
    for(i=0;i<255;i++)
        hist[i] =  (float)ihist[i]/(float)sum;
  for(int i=0;i<255;i++)
    {
        fprintf(fptr,"\n%f",hist[i]);
    }
    fclose(fptr);
}

void ImageProcessing::equalizeHistogram(unsigned char * _inputImgData, unsigned char * _outputImgData, int imgRows, int imgCols)
{
    int x,y,i,j;
    int histeq[256];
    float hist[256];
    float sum ;

    const char initHist[] ="init_hist.txt";
    const char finalHist[] = "final_hist.txt";

    computeHistogram2(&_inputImgData[0],imgRows,imgCols,&hist[0],initHist);
    for(i =0;i<=255;i++)
    {
        sum =0.0;
        for(j=0;j<=i;j++){
            sum = sum+hist[j];
        }
        histeq[i] =  (int)(255*sum+0.5);

    }
    for(y =0;y<imgRows;y++)
    {
        for(x=0;x<imgCols;x++)
        {
            *(_outputImgData+x+y*imgCols) = histeq[*(_inputImgData+x+y*imgCols)];
        }
    }
    computeHistogram2(&_outputImgData[0], imgRows,imgCols,&hist[0],finalHist);
}

void ImageProcessing::getImageNegative(unsigned char *_inImgData, unsigned char * _outImgData,int imgWidth,int imgHeight)
{

    for(int i =0;i<imgHeight;i++)
    {
        for(int j=0;j<imgWidth;j++){
             _outImgData[i*imgWidth+j ]  = 255 - _inImgData[i*imgWidth+j];
        }
    }
}

void ImageProcessing::Convolve2D(int imgRows, int imgCols, struct Mask *myMask, unsigned char *input_buf, unsigned char *output_buf)
{
    long i,j,m,n,idx,jdx;
    int ms,im,val;
    unsigned char *tmp;

    //the outer summation loop
    for(i =0;i<imgRows;++i)
        for(j =0;j<imgCols;++j){
            val =0;
            for(m=0;m<myMask->Rows;++m)
            for(n=0;n<myMask->Cols;++n){
                ms = (signed char)*(myMask->Data+ m*myMask->Rows+n);
                idx = i-m;
                jdx = j-n;
                if(idx>=0 && jdx >=0)
                    im = *(input_buf+idx*imgRows+jdx);
                val +=ms*im;
        }
            if(val >255) val =255;
            if(val <0)val =0;
            tmp =output_buf + i*imgRows +j;
            *tmp =(unsigned char)val;

    }

}

void  ImageProcessing::detectLine(unsigned char *_inputImgData, unsigned char *_outputImgData, int imgCols, int imgRows, const int MASK[][3])
{

    int x,y,i,j,sum;
    for(y=1;y<=imgRows-1;y++)
    {
        for(x=1;x<=imgCols;x++)
        {
            sum =0;
            for(i=-1;i<=1;i++)
            {
                for(j=-1;j<=1;j++)
                {
                    sum = sum + *(_inputImgData+x+i+(long)(y+j)*imgCols)*MASK[i+1][j+1];
                }
            }
            if(sum >255) sum = 255;
            if(sum<0)sum =0;
            *(_outputImgData+x+(long)y*imgCols) =sum ;
        }
    }
}
ImageProcessing::~ImageProcessing()
{
    //dtor
}


