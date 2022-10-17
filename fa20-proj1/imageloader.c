/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				macho5 - completer
**
**
** DATE:        2022-10-11
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *ppmfile = fopen(filename, "r");
	if(!ppmfile)
		return NULL;
	char title[3];
	uint8_t range;
	Image *img = (Image*) malloc (sizeof(Image));
	if(!img)
		return NULL;
	fscanf(ppmfile, "%s", title);
	if(title[0] != 'P' || title[1] != '3')
		return NULL;
	fscanf(ppmfile, "%u", &img->rows);
	fscanf(ppmfile, "%u", &img->cols);
	fscanf(ppmfile, "%hhu", &range);
	if(range != 255)
		return NULL;
	img->image = (Color**) malloc(img->cols * img->rows * sizeof(Color*));
	if(!img->image) 
		return NULL;
	for(int i = 0; i < img->cols * img->rows; ++i){
		*(img->image + i) = (Color*) malloc(sizeof(Color));
		if(!*(img->image + i))
			return NULL;
		Color *color = *(img->image + i);
		fscanf(ppmfile, "%hhu %hhu %hhu", &color->R, &color->G, &color->B);
	}
	fclose(ppmfile);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n%u %u\n255\n", image->cols, image->rows);
	Color **iter = image->image;
	for(int i = 0; i < image->rows; ++i){
		for(int j = 0; j < image->cols - 1; ++j){
			printf("%3hhu %3hhu %3hhu   ", (*iter)->R, (*iter)->G, (*iter)->B);
			++iter;
		}
		printf("%3hhu %3hhu %3hhu\n", (*iter)->R, (*iter)->G, (*iter)->B);
		++iter;
	}
}

//Frees an image
void freeImage(Image *image)
{
	for(int i = 0; i < image->cols * image->rows; ++i){
		free(*(image->image + i));
	}
	free(image->image);
	free(image);
}