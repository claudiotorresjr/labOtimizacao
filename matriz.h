/* Constantes */

#define DBL_FIELD "%12.7lg"
#define SEP_RES "\n\n\n"

#define N 16
#define UF 4
#define BLK 4

#define MAT(m,cols,i,j)   m[(i)*(cols)+(j)]

/* Implementações  para matrizes e vetores */

typedef double * MatRow;
typedef double * Vetor;

/* ----------- FUNÇÕES ---------------- */

MatRow geraMat (int m, int n);
void zeraMat (MatRow mat, int m, int n);
Vetor geraVetor (int n);
void zeraVetor (Vetor v, int n);

void mmm (MatRow A, MatRow B, int n, Vetor C);
void mt (MatRow A, MatRow B, int n);

void mmm_uj (MatRow A, MatRow B, int n, Vetor C);
void mt_uj (MatRow A, MatRow B, int n);

void mmm_blk (MatRow A, MatRow B, int n, int b, Vetor C);
void mt_blk (MatRow A, MatRow B, int n, int b);

void prnMat (MatRow mat, int n);
void prnVetor (Vetor vet, int n);
