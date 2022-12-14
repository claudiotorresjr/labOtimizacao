#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matmult.h"
#include "matriz.h"
#include "utils.h"

/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMat: gera matriz como vetor único, 'row-oriented'
 * 
 *  @param m  número de linhas da matriz
 *  @param n  número de colunas da matriz
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMat (int m, int n)
{
  MatRow matriz = (double *) malloc(m*n*sizeof(double));

  if (matriz) {
    for (int i=0; i < m; ++i) {
      for (int j=0; j < n; ++j)
	     MAT(matriz,n,i,j) = generateRandomA(i, j, m*n);
    }
  }
  
  return (matriz);
}


/**
 *  Funcao zeraMat: inicializa em 0 (zero) conteúdo de matriz criada com 'geraMat()'
 * 
 *  @param mat matriz a ser inicializada, criada com 'geraMat()'
 *  @param m  número de linhas da matriz
 *  @param n  número de colunas da matriz
 *
 */

void zeraMat (MatRow mat, int m, int n)
{
  memset (mat, 0, m*n*sizeof(double));
}


/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 * 
 *  @param n  número de elementos do vetor
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor (int n)
{
  Vetor vetor = (double *) malloc(n*sizeof(double));

  if (vetor)
    for (int i=0; i < n; ++i)
      vetor[i] = generateRandomB(n);
  
  return (vetor);
}


/**
 *  Funcao zeraVetor: inicializa em 0 (zero) conteúdo de vetor criado com 'geraVetor()'
 * 
 *  @param v vetor a ser inicializado, criado com 'geraVetor()'
 *  @param n  número de elementos do vetor
 *
 */

void zeraVetor (Vetor v, int n)
{
  memset (v, 0, n*sizeof(double));
}


/**
 *  Funcao mmm:  Efetua multiplicacao de duas matrizes 'n x n'
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMat()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void mmm (MatRow A, MatRow B, int n, MatRow C)
{

  /* Efetua a multiplicação */
  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      for (int k=0; k < n; ++k)
	       MAT(C,n,i,j) += MAT(A,n,i,k) * MAT(B,n,k,j);
}


/**
 *  Funcao mt:  Efetua a transpposta de uma matriz 'n x n'
 *  @param A matriz que recebrá a transposta de 'B'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *
 */

void mt (MatRow A, MatRow B, int n)
{
  /* Efetua a transposição */
  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      MAT(A,n,i,j) = MAT(B,n,j,i);
}



/**
 *  Funcao mmm_uj:  Efetua multiplicacao de duas matrizes 'n x n', com 'unroll & jam'
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMat()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void mmm_uj (MatRow A, MatRow B, int n, MatRow C)
{
    for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      for (int k=0; k < n-n%UF; k+=UF)
      {
        MAT(C,n,i,j) += MAT(A,n,i,k) * MAT(B,n,k,j);
        MAT(C,n,i,j) += MAT(A,n,i,k+1) * MAT(B,n,k+1,j);
        MAT(C,n,i,j) += MAT(A,n,i,k+2) * MAT(B,n,k+2,j);
        MAT(C,n,i,j) += MAT(A,n,i,k+3) * MAT(B,n,k+3,j);
      }

  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      for (int k=n-n%UF; k < n; ++k)
        MAT(C,n,i,j) += MAT(A,n,i,k) * MAT(B,n,k,j);
}


/**
 *  Funcao mt_uj:  Efetua a transpposta de uma matriz 'n x n', com 'unroll & jam'
 *  @param A matriz que recebrá a transposta de 'B'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *
 */

void mt_uj (MatRow A, MatRow B, int n)
{
  for (int i = 0; i < n - n%UF; i = i + UF)
  {
    for(int j = 0; j < n; j++)
    {
      MAT(A,n,i,j) = MAT(B,n,j,i);
      MAT(A,n,i+1,j) = MAT(B,n,j+1,i);
      MAT(A,n,i+2,j) = MAT(B,n,j+2,i);
      MAT(A,n,i+3,j) = MAT(B,n,j+3,i);
    }
  }

  for(int i = n - n%UF; i<N; ++i)
    for(int j = 0; j < n; ++j)
      MAT(A,n,i,j) = MAT(B,n,j,i);
}



/**
 *  Funcao mmm_blk:  Efetua multiplicacao de duas matrizes 'n x n', com 'unroll & jam' e 'blocking'
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param b fator de 'blocking'
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMat()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void mmm_blk (MatRow A, MatRow B, int n, int b, MatRow C)
{
	int NB = n/b;
	int resto = n%b;

  for (int bi = 0; bi < NB; ++bi)
  	for (int bj = 0; bj < NB; ++bj)
    {
    	int iINI = bi*b;
		int iFIM = iINI+b;
		int jINI = bj*b;
		int jFIM = jINI+b;
		for (int i=iINI; i < iFIM; ++i)
			for (int j=jINI; j < jFIM; ++j)
				for (int k=0; k < n-n%UF; k+=UF)
				{
					MAT(C,n,i,j) += MAT(A,n,i,k) * MAT(B,n,k,j);
					MAT(C,n,i,j) += MAT(A,n,i,k+1) * MAT(B,n,k+1,j);
					MAT(C,n,i,j) += MAT(A,n,i,k+2) * MAT(B,n,k+2,j);
					MAT(C,n,i,j) += MAT(A,n,i,k+3) * MAT(B,n,k+3,j);
				}
	}

	for (int i = n-resto; i < n; ++i)
    	for (int j = 0; j < n-resto; ++j)
      		for (int k = 0; k < n; ++k)
        		MAT(C,n,i,j) += MAT(A,n,i,k) * MAT(B,n,k,j);

	for (int i = 0; i < n; ++i)
		for (int j = n-resto; j < n; ++j)
			for (int k = 0; k < n; ++k)
				MAT(C,n,i,j) += MAT(A,n,i,k) * MAT(B,n,k,j);
}


/**
 *  Funcao mt_blk:  Efetua a transposta de uma matriz 'n x n', com 'unroll & jam' e 'blocking'
 *  @param A matriz que recebrá a transposta de 'B'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param b fator de 'blocking'
 *
 */

void mt_blk (MatRow A, MatRow B, int n, int b)
{
    int NB = n/b;
    int resto = n%b;

	for (int bi = 0; bi < NB; ++bi)
		for (int bj = 0; bj < NB; ++bj)
		{
			int iINI = bi*b;
			int iFIM = iINI+b;
			int jINI = bj*b;
			int jFIM = jINI+b;
			for (int i = iINI; i < iFIM-iFIM%UF; i+=UF)
				for (int j = jINI; j < jFIM; ++j)
				{
					MAT(A,n,i,j) = MAT(B,n,j,i);
					MAT(A,n,i+1,j) = MAT(B,n,j,i+1);
					MAT(A,n,i+2,j) = MAT(B,n,j,i+2);
					MAT(A,n,i+3,j) = MAT(B,n,j,i+3);
				}
		}

	for (int i = n-resto; i < n; ++i)
		for (int j = 0; j < n-resto; ++j)
			MAT(A,n,i,j) = MAT(B,n,j,i);

	for (int i = 0; i < n; ++i)
		for (int j = n-resto; j < n; ++j)
			MAT(A,n,i,j) = MAT(B,n,j,i);
}

/**
 *  Funcao prnMat:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param n ordem da matriz quadrada
 *
 */

void prnMat (MatRow mat, int n)
{
  int i, j;
  for (i=0; i < n; ++i) {
    for (j=0; j < n; ++j)
      printf(DBL_FIELD, MAT(mat,n,i,j));
    printf("\n");
  }
  printf(SEP_RES);
}

void prnVetor (Vetor vet, int n)
{
  for (int i=0; i < n; ++i)
    printf(DBL_FIELD, vet[i]);
  printf(SEP_RES);
}

