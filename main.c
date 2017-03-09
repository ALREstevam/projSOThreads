#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrixDymnAlloc.h"


int main()
{
	MatrixDescriber mxDesc;
	mxDesc.matrix = NULL;
	mxDesc.m = 3;
	mxDesc.n = 3;
	
	createMatrix(&mxDesc);
	
	printf("[%p]\n", &mxDesc);
	
	//setFillRandom(&mxDesc);
	setFillOrder(&mxDesc, 1.0);
	printMatrix(mxDesc);
	
	return 0;
}


