#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "thread.h"
#include "../util/util.h"
#include "../datadefine.h"
#include "../dataStructures/array/arrayMngr.h"
#include "../dataStructures/matrix/matrixMngr.h"


//Thread de soma
void * threadSumFunc(void * args){
	register int sumCount = 0, elementCount = 0;

    ThreadArgsInfo *targs = (ThreadArgsInfo*)args;//Convertendo os argumentos da thread
    
    if(printInfoProcess){printf(ANSI_COLOR_RED"[THREAD %d INICIANDO]\n"ANSI_COLOR_RESET, targs->threadNum);}
    
	MatrixDescriber mxa = *(targs->mx);//Colocando em uma vari�vel para deixar mais curto
    unsigned int jmp;//Tamanho do pulo
    int numThreads = targs->totThreads;//Quantidade de threads
    int numDiag = mxa.diagNum;//Quantidade de diagonais
    Coords coordrsp;
	
    float sum, rsp;

    for(jmp = targs->threadNum; jmp <= numDiag; (jmp+=numThreads)){//Cada thread sempre processar� diagonais alternadas com o mesmo tamanho
        
		if(!digaNumToCoord(mxa, jmp, &coordrsp)){//convertendo n�mero da diagonal para o primeiro elemento
			continue;
		}
        if(!getElement(mxa, coordrsp, &rsp)){//Acessando primeiro elemento
			continue;
		}
        sum = rsp;//colocando na soma


        while(getNextElementPositionMdiags(mxa, &coordrsp) == true){//enquanto existir um pr�ximo elemento na diagonal
            if(getElement(mxa, coordrsp, &rsp) == true){//acessando elemento
                sum += rsp;//adicionando elemento na soma
            	elementCount++;
            }
            else{
                break;
            }
        }

        targs->rspArr->data[jmp].dt.rsp = sum;//gravando resultado na matriz de resultado
        if(printInfoProcess){printf(ANSI_COLOR_YELLOW"T%d : [DG: %.2d, \tSM: %.2f]\n"ANSI_COLOR_RESET, targs->threadNum, jmp, sum);}
		sumCount++;
    }
    
    if(printInfoProcess){
		printf(ANSI_COLOR_GREEN"[Thread %d terminou, fez %d somas]\n"ANSI_COLOR_RESET, targs->threadNum, sumCount);
	}
    	
	if(generateExecutionData){
			ThreadExecutionData td;
			td.tnum = targs->threadNum;
			td.processedElems = elementCount;
			td.processedDiags = sumCount;
			
			appendToTExecutionFile(DELAULTTHREADEXCSVFILE, td);
	}
    	
    	
    	
    	
    pthread_exit(NULL);//terminando a thread
    return NULL;
}