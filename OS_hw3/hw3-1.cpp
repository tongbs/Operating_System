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

#define number 24 //20 = 1323709, 25 = 1166220, 24 = 1003134||975223, 16 = 1109368 23 = 1887611

using namespace std;

#define MYRED	2
#define MYGREEN 1
#define MYBLUE	0

int imgWidth, imgHeight;
int FILTER_SIZE;
int FILTER_SCALE;
int *filter_G;

int thread_identifier = 0;

const char *inputfile_name[5] = {
	"input1.bmp",
	"input2.bmp",
	"input3.bmp",
	"input4.bmp",
	"input5.bmp"
};
const char *outputBlur_name[5] = {
	"Blur1.bmp",
	"Blur2.bmp",
	"Blur3.bmp",
	"Blur4.bmp",
	"Blur5.bmp"
};
/*
const char *outputSobel_name[5] = {
	"Sobel1.bmp",
	"Sobel2.bmp",
	"Sobel3.bmp",
	"Sobel4.bmp",
	"Sobel5.bmp"
};*/

unsigned char *pic_in, *pic_grey, *pic_blur, *pic_final;

void* Grey(void*);
void* G_filter(void*);

pthread_mutex_t mutexlock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexlock2 = PTHREAD_MUTEX_INITIALIZER;


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

unsigned char GaussianFilter(int w, int h)
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

		tmp += filter_G[j*ws + i] * pic_grey[b*imgWidth + a];
	};
	tmp /= FILTER_SCALE;
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return (unsigned char)tmp;
}

int main()
{
	// read mask file
	FILE* mask;
	mask = fopen("mask_Gaussian.txt", "r");
	fscanf(mask, "%d", &FILTER_SIZE);
	fscanf(mask, "%d", &FILTER_SCALE);

	filter_G = new int[FILTER_SIZE];
	for (int i = 0; i<FILTER_SIZE; i++)
		fscanf(mask, "%d", &filter_G[i]);
	fclose(mask);


	BmpReader* bmpReader = new BmpReader();
	for (int k = 0; k<5; k++){
		// read input BMP file
		pic_in = bmpReader->ReadBMP(inputfile_name[k], &imgWidth, &imgHeight);
		// allocate space for output image
		pic_grey = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_blur = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_final = (unsigned char*)malloc(3 * imgWidth*imgHeight*sizeof(unsigned char));

		pthread_t thread[number];
		for(int i = 0; i<number; i++)
		{
			pthread_create(&thread[i] , NULL , &Grey , NULL);
		}
		for(int i = 0; i<number; i++)
		{
			pthread_join(thread[i] , NULL);
		}
		thread_identifier = 0;
		//convert RGB image to grey image ---> Grey

		pthread_t thread1[number];
		for(int i = 0; i<number; i++)
		{
			pthread_create(&thread1[i], NULL , &G_filter , NULL);
		}
		for(int i = 0; i<number; i++)
		{
			pthread_join(thread1[i] , NULL);
		}
		thread_identifier = 0;
		//apply the Gaussian filter to the image ---> G_filter
		/*for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				pic_blur[j*imgWidth + i] = GaussianFilter(i, j);
			}
		}*/

		//extend the size form WxHx1 to WxHx3
		for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				pic_final[3 * (j*imgWidth + i) + MYRED] = pic_blur[j*imgWidth + i];
				pic_final[3 * (j*imgWidth + i) + MYGREEN] = pic_blur[j*imgWidth + i];
				pic_final[3 * (j*imgWidth + i) + MYBLUE] = pic_blur[j*imgWidth + i];
			}
		}

		// write output BMP file
		bmpReader->WriteBMP(outputBlur_name[k], imgWidth, imgHeight, pic_final);

		//free memory space
		free(pic_in);
		free(pic_grey);
		free(pic_blur);
		free(pic_final);
	}

	return 0;
}

void* Grey(void* Null_ptr)
{
	pthread_mutex_lock(&mutexlock1);
	int last;
	int last_1;
	int now;
	if(thread_identifier == number-1)
	{
		last = imgHeight;
	}
	else
	{
		last_1 = thread_identifier+1;
		last = last_1*imgHeight/number;
	}
	now = thread_identifier*(imgHeight/number);
	thread_identifier++;
	pthread_mutex_unlock(&mutexlock1);

	for (int k = now; k<last; k++)
	{
		for (int i = 0; i<imgWidth; i++)
		{
			pic_grey[k*imgWidth + i] = RGB2grey(i, k);
		}
	}
	return NULL;
}

void* G_filter(void* Null_ptr)
{
	pthread_mutex_lock(&mutexlock2);
	int last;
	int last_1;
	int now;
	if(thread_identifier == number-1)
	{
		last = imgHeight;
	}
	else
	{
		last_1 = thread_identifier+1;
		last = last_1*imgHeight/number;
	}
	now = thread_identifier*(imgHeight/number);
	thread_identifier++;
	pthread_mutex_unlock(&mutexlock2);

	for (int k = now; k<last; k++)
	{
			for (int i = 0; i<imgWidth; i++)
			{
				pic_blur[k*imgWidth + i] = GaussianFilter(i, k);
			}
	}
	return NULL;
}



