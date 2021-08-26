#ifndef PARTICOES_H
#define PARTICOES_H

#include <stdio.h>

#include "lista.h"

/* Algoritmo de geracao de particoes por Classificacao Interna, recebe como parâmetro
o arquivo de dados de entrada, a lista contendo os nomes dos arquivos de saída das partições,
o númer de elementos M a ser armazenado em cada partição e o número de registro total do arquivo.*/
void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc);

/*Algoritmo a ser implementado no trabalho de geracao de particoes por Selecao Natural recebe como parâmetro
o arquivo de dados de entrada, a lista contendo os nomes dos arquivos de saída das partições,
o númer de elementos M a ser armazenado em cada partição e o número de registro total do arquivo e o tamanho do reservatório.
Fiquem a vontade para modificar a estrutura, foi uma sugestão.*/
void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int *n_particoes);
TFunc *encontra_menor_id(TFunc **vet_func, int M, int *posicao);
void OrdenaFunc (TFunc **vetor, int M);
int armazena (TFunc *func, int cont, FILE *armazenamneto, int M);
void ordenacao_part (Lista *nome_arq, int n, int numero_f);
void imprimirFuncs (Lista *nome_arq, int n, int numero_f);


#endif
