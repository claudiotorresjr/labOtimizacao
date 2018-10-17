#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */

#include "matmult.h"
#include "matriz.h"
#include "utils.h"

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ -n <ordem> ] [ -b <blocking> ] \n", progname);
  exit(1);
}

/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ] [ -m <stride> ] [ -b <blocking> ]
 * -n <ordem>: ordem das matrizes quadradas e de vetores
 * -b <blocking>: fator de 'blocking'
 *
 */

int main (int argc, char *argv[]) 
{
  int c, b=BLK, n=N;
  
  MatRow A, B, C;
  Vetor vet, res;

  double t, t_uj, t_blk;
  
  /* =============== TRATAMENTO DE LINHA DE COMAANDO =============== */

  char *opts = "n:b:";
  c = getopt (argc, argv, opts);
  
  while ( c != -1 ) {
    switch (c) {
    case 'n':  n = atoi(optarg);   break;
    case 'b':  b = atoi(optarg);   break;
    default:   usage(argv[0]);
    }
      
    c = getopt (argc, argv, opts);
  }

  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */
  
  
  srand(20182);
  
  A = geraMat (n, n);
  B = geraMat (n, n);
  C = geraMat (n, n);
  vet = geraVetor (n);
    
#ifdef DEBUG
  prnMat (A, n);
  prnMat (B, n);
  prnVetor (vet, n);
  printf ("=================================\n\n");
#endif /* DEBUG */
  
  t = timestamp();
  mt (A, B, n);
  t =  timestamp() - t;

  t_uj = timestamp();
  mt_uj (A, B, n);
  t_uj = timestamp() - t_uj;

  t_blk = timestamp();
  mt_blk (A, B, n, b);
  t_blk = timestamp() - t_blk;

  printf("\t\t Tmt = %lg\n\t\t Tmt_uj = %lg\n\t\t Tmt_blk = %lg\n\n", t, t_uj, t_blk);

  zeraMat (C, n, n);
  t = timestamp();
  mmm (A, B, n, C);
  t = timestamp() - t;

  zeraMat (C, n, n);
  t_uj = timestamp();
  mmm_uj (A, B, n, C);
  t_uj = timestamp() - t_uj;

  zeraMat (C, n, n);
  t_blk = timestamp();
  mmm_blk (A, B, n, b, C);
  t_blk = timestamp() - t_blk;

  printf("\t\t Tmmm = %lg\n\t\t Tmmm_uj = %lg\n\t\t Tmmm_blk = %lg\n\n", t, t_uj, t_blk);

#ifdef DEBUG
  prnMat (C, n);
  prnVetor (res, n);
#endif /* DEBUG */

  free (A);
  free (B);
  free (C);
  free (vet);

}

