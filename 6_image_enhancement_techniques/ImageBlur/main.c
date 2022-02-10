#include <stdio.h>
#include <stdlib.h>

int main()
{
     FILE *fIn =fopen("fruits.bmp","rb");
     FILE *fOut = fopen("fruits_blur.bmp","wb");

     unsigned char imgHeader[54];
     unsigned char colorTable[1024];

     if(fIn ==NULL)
     {
         printf("Unable to open file \n");
     }

     for(int i =0;i<54;i++)
     {
         imgHeader[i] = getc(fIn);
     }
     fwrite(imgHeader,sizeof(unsigned char),54,fOut);

     int height = *(int*)&imgHeader[22];
     int width  = *(int*)&imgHeader[18];
     int bitDepth = *(int*)&imgHeader[28];

     if(bitDepth <=8)
     {
         fread(colorTable,sizeof(unsigned char),1024,fIn);
         fwrite(colorTable, sizeof(unsigned char),1024,fOut);

     }

     int imgSize =  height * width;

     unsigned char buffer[imgSize][3];
     unsigned char output_buffer[imgSize][3];

     for(int i =0;i<imgSize;i++)
     {
         buffer[i][2] = getc(fIn);
         buffer[i][1] = getc(fIn);
         buffer[i][0] = getc(fIn);
     }
     float kernel[3][3] = {{1.0/9.0, 1.0/9.0,1.0/9.0},
                           {1.0/9.0, 1.0/9.0,1.0/9.0},
                           {1.0/9.0, 1.0/9.0,1.0/9.0}
                            };

    for(int x =1 ;x<height-1;x++)
    {
        for(int y =1;y<width -1;y++)
        {
            float sum0 = 0.0;
            float sum1 = 0.0;
            float sum2 = 0.0;
            for(int i =-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    sum0  = sum0 + (float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
                    sum1  = sum1 + (float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
                    sum2  = sum2 + (float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];

                }
            }

            output_buffer[(x)*width+(y)][0] =  sum0;
            output_buffer[(x)*width+(y)][1] =  sum1;
            output_buffer[(x)*width+(y)][2] =  sum2;
        }
    }

    for(int i =0;i<imgSize;i++)
    {
        putc(output_buffer[i][2],fOut);
        putc(output_buffer[i][1],fOut);
        putc(output_buffer[i][0],fOut);
    }

    fclose(fOut);
    fclose(fIn);
    return 0;
}


