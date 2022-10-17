/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				macho5
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {1, 0, -1, 1, -1, 1, 0, -1};
int is_alive_R(Image *image, int row, int col){
	return (*(image->image + row * image->cols + col))->R == 255;
}
int is_alive_G(Image *image, int row, int col){
	return (*(image->image + row * image->cols + col))->G == 255;
}
int is_alive_B(Image *image, int row, int col){
	return (*(image->image + row * image->cols + col))->B == 255;
}
//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	int nei_R = 0, nei_G = 0, nei_B = 0;
	Color *next_state = (Color*) malloc(sizeof(Color));
	for(int i = 1; i <= 8; ++i){
		int nei_row = (row + dx[i]) % image->rows;
		int nei_col = (col + dy[i]) % image->cols;
		nei_R += is_alive_R(image, nei_row, nei_col);
		nei_G += is_alive_G(image, nei_row, nei_col);
		nei_B += is_alive_B(image, nei_row, nei_col);
	}
	int shift_R = 9 * is_alive_R(image, row, col) + nei_R;
	int shift_G = 9 * is_alive_G(image, row, col) + nei_G;
	int shift_B = 9 * is_alive_B(image, row, col) + nei_B;
	if(rule & (1<<shift_R)) next_state->R = 255;
	else next_state->R = 0;
	if(rule & (1<<shift_G)) next_state->G = 255;
	else next_state->G = 0;
	if(rule & (1<<shift_B)) next_state->B = 255;
	else next_state->B = 0;
	return next_state;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *img = (Image*) malloc(sizeof(Image));
	img->cols = image->cols;
	img->rows = image->rows;
	img->image = (Color**) malloc(img->rows * img->cols * sizeof(Color*));
	for(int i = 0; i < image->rows; ++i){
		for(int j = 0; j < image->cols; ++j){
			*(img->image + i * image->cols + j) = evaluateOneCell(image, i, j, rule);
		}
	}
	return img;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if(argc != 3){
		printf("usage: ./gameOfLife filename rule\n\
				filename is an ASCII PPM file (type P3) with maximum value 255.\n\
				rule is a hex number beginning with 0x; Life is 0x1808.");
		return -1;
	}
	Image *image = readData(argv[1]);
	if(!image)
		return -1;
	int32_t rule = strtol(argv[2], NULL, 16);
	Image *next = life(image, rule);
	writeData(next);
	freeImage(image);
	freeImage(next);
	return 0;
}
