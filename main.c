// Adicionar mais retorno para as threads!
// - Quantos elementos a thread processou
// - Quantas diagonais a thread processou
// - Testar se o free est� funcionanando no linux
// - Fazer novos testes no linux


/*
Escrito por Andr� L. R. Estevam 		como trabalho para a disciplina de Sistemas
Operacionais (1� Semestre de 2017) da Faculdade de Tecnologia da Unicamp
-------------------------------------------------------------------------------
Este � um programa que l� n�meros com ponto flutuante a partir de um arquivo para
uma matriz m x n com m e n sendo inseridos pelo usu�rio, o programa usa de 1 a n
threads para processar a soma das diagonais principais armazenando o resultado em
um vetor, podendo ainda gerar dados sobre a execu��o do programa gravando em um
arquivo .csv que pode ser lido por algum software de panilha eletr�nica 
*/

//Incluindo bibliotecas padr�o
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

//Incluindo bibliotecas criadas
#include "util/util.h"//Biblioteca com fun��es de utilidades diversas
#include "datadefine.h"//Defini��o de dados (vari�veis globais, defines e tipos de dados novos)
#include "dataStructures/array/arrayMngr.h"//Biblioteca para gerenciar o tipo de vetor usado
#include "dataStructures/matrix/matrixMngr.h"//Biblioteca para gerenciar o tipo de matriz usada
#include "thread/thread.h"//Biblioteca contendo fun��es executadas por threads
#include "file/fileMngr.h"//Bilioteca para definir gerenciamento dos arquivos usados

int main(){
	time_t tStart, tEnd;
	double elapsedTime;
	int auxm, auxn;
	int numThreads;
	
	putHeader(DEFAULTEXDATACSVFILE,"tempo;m;n;diagonais;threads\n");
	putHeader(DELAULTTHREADEXCSVFILE,"tnum;elementos_processados;diagonais_processadas\n");
	
	//Gerar entrada com n�meros aleat�rios
	if(fillInputWithRandom){
		generateRandomFloatFile(defaultInputPath, fileElementsAmount);
	}else if(fillInputWithNum){
		fillFileWithValue(defaultInputPath, fileElementsAmount, fillElement);
	}

	//Se vari�vel ativada vai requerir dados do usu�rio, caso contr�rio vai rodar com os valores default
	if(getInputFromUser){
		printf("Digite os valores:\n<linhas> <colunas> <qtd. de threads>\n");
		
		scanf("%d %d %d",&auxm, &auxn, &numThreads);
		getchar();	
	}else{
		auxm = default_M;
		auxn = default_N;
		numThreads = default_NumThreads;
	}
	
	
	if(getInputFromUser){
		printf("Matriz: (%d X %d)\nThreads: %d\n", auxm, auxn, numThreads);
		pause();
	}
	
	tStart = clock();//Iniciando rel�gio
	register int i;
	
	
	//Definindo vetores e matrizes
    MatrixDescriber matrix;
    ArrayDescriber rspArr;
    ArrayDescriber tidArr;

	//Criando vetores e matrizes
	createMatrix(&matrix, auxm, auxn);
    createArray(&rspArr, matrix.diagNum);
	createArray(&tidArr, numThreads);
	rspArr.top = matrix.diagNum-1;
	
	//Lendo arquivo de entrada
    fileToMatrix(matrix, defaultInputPath);

    
	//Alocando na mem�ria espa�o para o argumento das threads
	ThreadArgsInfo * tinfoptr = (ThreadArgsInfo*) malloc(numThreads * sizeof(ThreadArgsInfo));
	if(tinfoptr == NULL){
		fprintf(stderr,"Erro ao alocar argumentos de threads\n");
		return -1;
	}

	//Para cada thread
    for(i = 0; i < numThreads; i++){

		//Inicializando os argumentos das threads
		tinfoptr[i].threadNum = (unsigned short int)i;
        tinfoptr[i].mx = &matrix;
    	tinfoptr[i].rspArr = &rspArr;
    	tinfoptr[i].totThreads = (unsigned int)numThreads;
        
        //Criando thread
		pthread_create(&(tidArr.data[i].dt.tid), NULL, threadSumFunc, &tinfoptr[i]);
	
    }
    
    for(i = 0; i < numThreads; i++){
		pthread_join(tidArr.data[i].dt.tid, NULL);
	}
	tEnd = clock();//Parando rel�gio
	

	
	printf("\t\tIMPRIMINDO RESULTADOS\n\n");
    for(i = 0; i < matrix.diagNum; i++){
        printf("Diagonal: [%d]\t|\tSoma: %.3f\n",i, rspArr.data[i].dt.rsp);
    }
    

    arrayFloatToFile(rspArr, defaultOutputPath);
 
    
 	//Liberando mem�ria utilizada
 	deleteMatrix(&matrix);
 	deleteArray(&rspArr);
	deleteArray(&tidArr);
 	free(tinfoptr);

	 
	 elapsedTime = difftime(tEnd, tStart);//Calculando tempo gasto
	 
	 if(generateExecutionData){
		 //Gerando estrutura com dados da execu��o para gravar num arquivo .csv
		 ExecutionData exdt;
		 exdt.elapsedTime 	= elapsedTime;
		 exdt.m 			= matrix.m;
		 exdt.n 			= matrix.n;
		 exdt.diags 		= matrix.diagNum;
		 exdt.numThreads 	= numThreads;
		 executionDataToCSV(exdt, DEFAULTEXDATACSVFILE);
	 }
	 printf("\nTEMPO GASTO: [%.2lf s]\n", elapsedTime);

    return 0;
}



