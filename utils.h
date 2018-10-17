#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

char *strdup(const char *s);  // problemas de warning com -std=c11

#define ABS(num)  ((num) < 0.0 ? -(num) : (num))


// Retorna tempo em segundos
double timestamp(void);

/**
 * Função que gera os coeficientes de um sistema linear k-diagonal
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
 * @param k numero de diagonais da matriz A
 */
double generateRandomA( unsigned int i, unsigned int j, unsigned int k );

/**
 * Função que gera os termos independentes de um sistema linear k-diagonal
 * @param k numero de diagonais da matriz A
 *
 */
double generateRandomB( unsigned int k );
