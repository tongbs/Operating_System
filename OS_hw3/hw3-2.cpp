// Student ID:0413359
// Name      :
// Date      : 2017.11.03

#include "bmpReader.h"
#include "bmpReader.cpp"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define MYRED	2
#define MYGREEN 1
#define MYBLUE	0

int imgWidth, imgHeight;
int FILTER_SIZE;
//int FILTER_SCALE;
int *filter_Gx;
int *filter_Gy;

//-------------------------------------------
//base = 1430390
int sem_size = 6; //5 = 979729, 6 = 876140, 8 = 1410740, 4 = 871489
sem_t sem_x;        //3 = 1328378 , 2 = 1630426
sem_t sem_y;
int h2 = 0;
int h1 = 0;

void* Grey(void*);
void* Gx_f(void*);
void* Gy_f(void*);

pthread_mutex_t mutexlock1 = PTHREAD_MUTEX_INITIALIZER;
//-------------------------------------------
const char *inputfile_name[5] = {
	"input1.bmp",
	"input2.bmp",
	"input3.bmp",
	"input4.bmp",
	"input5.bmp"
};
/*const char *outputBlur_name[5] = {
	"Blur1.bmp",
	"Blur2.bmp",
	"Blur3.bmp",
	"Blur4.bmp",
	"Blur5.bmp"
};*/

const char *outputSobel_name[5] = {
	"Sobel1.bmp",
	"Sobel2.bmp",
	"Sobel3.bmp",
	"Sobel4.bmp",
	"Sobel5.bmp"
};

unsigned char *pic_in, *pic_grey, *pic_blur, *pic_final;
unsigned char *pic_blur_x, *pic_blur_y;

unsigned char RGB2grey(int w, int h)
{
	int tmp = (
		pic_in[3 * (h*imgWidth + w) + MYRED] +
		pic_in[3 * (h*imgWidth + w) + MYGREEN] +
		pic_in[3 * (h*imgWidth + w) + MYBLUE] )/3;

	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return (unsigned char)tmp;
}

unsigned char Gx_filter(int w, int h)
{
	int tmp = 0;
	int a, b;
    int ws = (int)sqrt((float)FILTER_SIZE);
    for (int j = 0; j<ws; j++)
        for (int i = 0; i<ws; i++)
        {
            a = w + i - (ws / 2);
            b = h + j - (ws / 2);
            
            // detect for borders of the image
            if (a<0 || b<0 || a>=imgWidth || b>=imgHeight) continue;
            
            tmp += filter_Gx[j*ws + i] * pic_grey[b*imgWidth + a];
        };
    //tmp /= FILTER_SCALE;
    if (tmp < 0) tmp = 0;
    if (tmp > 255) tmp = 255;
    return (unsigned char)tmp;
}

unsigned char Gy_filter(int w, int h)
{
	int tmp = 0;
	int a, b;
    int ws = (int)sqrt((float)FILTER_SIZE);
    for (int j = 0; j<ws; j++)
        for (int i = 0; i<ws; i++)
        {
            a = w + i - (ws / 2);
            b = h + j - (ws / 2);
            
            // detect for borders of the image
            if (a<0 || b<0 || a>=imgWidth || b>=imgHeight) continue;
            
            tmp += filter_Gy[j*ws + i] * pic_grey[b*imgWidth + a];
        };
    //tmp /= FILTER_SCALE;
    if (tmp < 0) tmp = 0;
    if (tmp > 255) tmp = 255;
    return (unsigned char)tmp;
}

int main()
{
	// read mask file
	FILE* mask;
	mask = fopen("mask_Sobel.txt", "r");
	fscanf(mask, "%d", &FILTER_SIZE);
	//fscanf(mask, "%d", &FILTER_SCALE);
	//-------------------------------------
	
	//-------------------------------------
	filter_Gx = new int[FILTER_SIZE];
	filter_Gy = new int[FILTER_SIZE];
	for (int i = 0; i<FILTER_SIZE; i++)
		fscanf(mask, "%d", &filter_Gx[i]);
	
	for(int i = 0; i<FILTER_SIZE; i++)
	    fscanf(mask, "%d", &filter_Gy[i]);
	
	fclose(mask);

	pthread_t thread1;
	pthread_t thread_x[sem_size];
	pthread_t thread_y[sem_size];

	BmpReader* bmpReader = new BmpReader();
	for (int k = 0; k<5; k++){
		// read input BMP file
		pic_in = bmpReader->ReadBMP(inputfile_name[k], &imgWidth, &imgHeight);
		// allocate space for output image
		pic_grey = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_blur = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_final = (unsigned char*)malloc(3 * imgWidth*imgHeight*sizeof(unsigned char));

		pic_blur_x = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_blur_y = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));

		//--------------------------------------------------
		sem_init(&sem_x,0,0);
		sem_init(&sem_y,0,0);

		pthread_create(&thread1 , NULL , Grey , NULL);

		//convert RGB image to grey image --->Grey
		/*for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				pic_grey[j*imgWidth + i] = RGB2grey(i, j);
			}
		}*/

		for(int i = 0; i<sem_size; i++)
		{
			pthread_create(&thread_x[i] , NULL , Gx_f , NULL);
		}

			for(int i = 0; i<sem_size; i++)
		{
			pthread_create(&thread_y[i] , NULL , Gy_f , NULL);
		}

		pthread_join(thread1 , NULL);

		for(int i = 0; i<sem_size; i++)
		{
			pthread_join(thread_x[i] , NULL);
		}


		for(int i = 0; i<sem_size; i++)
		{
			pthread_join(thread_y[i] , NULL);
		}
		//apply the Gx filter to the image --->Gx_f
		/*for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				pic_blur_x[j*imgWidth + i] = Gx_filter(i, j);
			}
		}*/

		//apply the Gy filter to the image --->Gy_f
		/*for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				pic_blur_y[j*imgWidth + i] = Gy_filter(i, j);
			}
		}*/

		//calculate sqrt
		/*for(int j = 0; j<imgHeight; j++)
		{
			for(int i = 0; i<imgWidth; i++)
			{
				pic_blur[j*imgWidth+i] = (unsigned char)sqrt(pic_blur_x[j*imgWidth+i]*pic_blur_x[j*imgWidth+i] + pic_blur_y[j*imgWidth+i]*pic_blur_y[j*imgWidth+i]);
				//pic_blur[j*imgWidth+i] = (unsigned char) tmp1;
			}
		}*/


		int temp;
		//extend the size form WxHx1 to WxHx3
		for (int j = 0; j<imgHeight; j++) 
		{
			for (int i = 0; i<imgWidth; i++)
			{
				temp = sqrt((double)pic_blur_x[j*imgWidth + i]*pic_blur_x[j*imgWidth + i] + (double)pic_blur_y[j*imgWidth + i]*pic_blur_y[j*imgWidth + i]);
				if (temp > 255)
				    temp = 255;
				else if (temp < 0)
				    temp = 0;
				pic_final[3 * (j*imgWidth + i) + MYRED] = temp;
				pic_final[3 * (j*imgWidth + i) + MYGREEN] = temp;
				pic_final[3 * (j*imgWidth + i) + MYBLUE] = temp;
			}
		}

		// write output BMP file
		bmpReader->WriteBMP(outputSobel_name[k], imgWidth, imgHeight, pic_final);

		//free memory space
		//unsigned char *pic_in, *pic_grey, *pic_blur, *pic_final;
//unsigned char *pic_blur_x, *pic_blur_y;
		free(pic_in);
		free(pic_grey);
		free(pic_blur);
		free(pic_blur_x);
		free(pic_blur_y);
		free(pic_final);
		
		h2 = 0;
		h1 = 0;
	}
	return 0;
}

void* Grey(void* null_ptr)
{
	int tong = 5;

	for (int j = 0; j<imgHeight; j++)
	{
			for (int i = 0; i<imgWidth; i++)
			{
				pic_grey[j*imgWidth + i] = RGB2grey(i, j);
			}
			if(j >= tong)
			{
				sem_post(&sem_x);
				sem_post(&sem_y);
			}
	}

	for(int i = 0; i<tong; i++)
	{
		sem_post(&sem_x);
		sem_post(&sem_y);
	}
	return NULL;
}

void* Gx_f(void* null_ptr)
{
	int count;
	while(1)
	{
 		sem_wait(&sem_x);
 		pthread_mutex_lock(&mutexlock1);
 		int k = h2++;
 		count = k-imgHeight+sem_size;
 		pthread_mutex_unlock(&mutexlock1);
		for (int i = 0; i<imgWidth; i++)
		{
			pic_blur_x[k*imgWidth + i] = Gx_filter(i, k);
		}
		if(count >= 0)
		{
			break;
		}
	}
	return NULL;
}

void* Gy_f(void* null_ptr)
{
	int count;
	while(1)
	{
		sem_wait(&sem_y);
		pthread_mutex_lock(&mutexlock1);
		int k = h1++;
		count = k-imgHeight+sem_size;
		pthread_mutex_unlock(&mutexlock1);
		for (int i = 0; i<imgWidth; i++)
		{
			pic_blur_y[k*imgWidth + i] = Gy_filter(i, k);
		}
		if(count >=0)
		{
			break;
		}
	}
	return NULL;
}
