
#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>

#include "particoes.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int *n_particoes){

    rewind(arq);
    TFunc *grava, *menor_cod, **vetor;
    menor_cod = NULL;
    int registro = 0, count = 0, i = 0, aberto = 0, menor_reg = 0, pos = 0, auxiliar = 0, parada = 0;
    vetor = calloc(sizeof(**vetor), M);
    grava = calloc(sizeof(*grava), 1);
    FILE *guardar = fopen("armazena.dat", "wb+");
    FILE *arq_aux;

    if (guardar == NULL || vetor == NULL || grava == NULL)
    {
        printf("\n\n***ERRO AO ABRIR ARQUIVO***");
    }
    else
    {
    while (registro != nFunc) {
        while (parada == 0)
        {
            fseek(arq, registro* tamanho_registro(), SEEK_SET);
            vetor[i] = le_funcionario(arq);
            i++;
            if(i>=M){
            parada = 1;
            }
            else {
            registro++;
                 }
        }

        menor_cod = encontra_menor_id(vetor, M, &pos);

        if (menor_cod->cod > grava->cod || menor_reg  == 0){
            menor_reg  = 1;
            char *nome_particao = nome_arquivos_saida->nome;
            if (aberto == 0){
                printf("\n%s\n", nome_arquivos_saida->nome);
                nome_arquivos_saida = nome_arquivos_saida->prox;
                arq_aux = fopen(nome_particao, "wb+");
                if (arq_aux == NULL) printf("Erro ao abrir o arquivo.\n");
                aberto = 1;
                *n_particoes+=1;
                auxiliar = 0;
                }
                fseek(arq_aux, auxiliar*tamanho_registro(), SEEK_SET);
                salva_funcionario(menor_cod, arq_aux);
                imprime_funcionario(menor_cod);
                *grava = *menor_cod;
                registro++;
                auxiliar++;
            }
            else{
                if (count < M){
                    count = armazena(menor_cod, count,guardar, M);
                    registro++;
                            }
                else{
                    count = 0;
                    aberto = 0;
                    OrdenaFunc(vetor, M);
                    for (int j = 0; j < M; j++){
                        fseek(arq_aux, auxiliar*tamanho_registro(), SEEK_SET);
                        salva_funcionario(vetor[j], arq_aux);
                        imprime_funcionario(vetor[j]);
                        auxiliar++;
                        free(vetor[j]);
                    }
                    for (int j = 0; j < M; j++){
                        fseek(guardar, j*tamanho_registro(), SEEK_SET);
                        vetor[j] = le_funcionario(guardar);
                    }
                    fclose(guardar);
                    guardar = fopen("reservatorio.dat", "wb+");
                    menor_reg  = 0;
                    fclose(arq_aux);
                }
            }
            if (menor_reg  != 0 && registro < 20){
                fseek(arq, registro*tamanho_registro(), SEEK_SET);
                free(vetor[pos]);
                vetor[pos] = le_funcionario(arq);
            }
        }
OrdenaFunc(vetor, M);

    for (int j = 0; j < M; j++){
        if (vetor[j]->cod != grava->cod && vetor[j]->cod != menor_cod->cod){
            fseek(arq_aux, auxiliar*tamanho_registro(), SEEK_SET);
            salva_funcionario(vetor[j], arq_aux);
            imprime_funcionario(vetor[j]);
            auxiliar++;
        }
    }

    int contador = 0;
    for (int j = 0; fgetc(guardar) != EOF; j++){
        fseek(guardar, j*tamanho_registro(), SEEK_SET);
        vetor[j] = le_funcionario(guardar);
        contador++;
    }
OrdenaFunc(vetor, M);

    for (int j = 0; j < contador; j++){
        fseek(arq_aux, auxiliar*tamanho_registro(), SEEK_SET);
        salva_funcionario(vetor[j], arq_aux);
        imprime_funcionario(vetor[j]);
        auxiliar++;
    }
    for (int i = 0; i < M; i++)
    free(vetor[i]);
    free(vetor);
    free(grava);
    fclose(arq_aux);
    fclose(guardar);
    }
}

TFunc *encontra_menor_id(TFunc **vet_func, int M, int *posicao){
     *posicao = 0;
     TFunc *menor_id = vet_func[0];
     int i;
     for (i=0; i < M; i++)
     {
        if (vet_func[i]->cod < menor_id->cod){
            menor_id = vet_func[i];
            *posicao = i;
        }
     }
    return menor_id;
}

void OrdenaFunc (TFunc **vetor, int M){
    int aux = 0;
    for (int i = 1; i < M; i++)
    {
        TFunc *func = vetor[i];
        aux = i - 1;
        while ((aux >= 0) && (vetor[aux]->cod > func->cod))
        {
            vetor[aux + 1] = vetor[aux];
            aux --;
        }
        vetor[aux + 1] = func;
    }
  }

int armazena (TFunc *func, int cont, FILE *armazenamento, int M){
    if (cont < M){
        fseek(armazenamento, (cont)*tamanho_registro(), SEEK_SET);
        salva_funcionario(func, armazenamento);
        cont++;
    }
    return cont;
}

void ordenacao_part (Lista *nome_arq, int n, int numero_f){
    FILE *part;
    char *nomes;
    TFunc **vetor = calloc(sizeof(**vetor), numero_f);
    int cont1, cont2, i, j;

    for (i = 0; i < n; i++)
        {
        nomes = nome_arq->nome;
        part = fopen (nomes, "rb+");
        cont1 = 0;
        for ( j = 0; fgetc(part) != EOF; j++)
            {
            fseek (part, j*tamanho_registro(), SEEK_SET);
            vetor[j] = le_funcionario(part);
            cont1++;
            }
        OrdenaFunc(vetor, cont1);
        cont2 = 0;
        fclose(part);
        part = fopen(nomes, "wb+");
        while (cont2 < cont1)
        {
            fseek(part, cont2*tamanho_registro(), SEEK_SET);
            salva_funcionario(vetor[cont2], part);
            cont2++;
        }
        fclose(part);
        nome_arq = nome_arq->prox;
        for (int i = 0; i < cont1; i++)
        free(vetor[i]);
        }
        free(vetor);
}

void imprimirFuncs (Lista *nome_arq, int n, int numero_f){
     TFunc **vetor = calloc (sizeof (**vetor), numero_f);
     FILE *particao;
     char *nomes_func;
     int i;
     int j;
     printf("\n\n***PARTIÇÕES ORDENADAS***");

    for (i = 0; i < n; i++)
    {
        nomes_func = nome_arq->nome;
        particao = fopen (nomes_func, "rb+");
        printf("\n %s\n", nome_arq->nome);

    for (j = 0; fgetc(particao) != EOF; j++)
        {
        fseek (particao, j*tamanho_registro(), SEEK_SET);
        vetor[j] = le_funcionario(particao);
        imprime_funcionario(vetor[j]);
        free(vetor[j]);
        }
        fclose(particao);
        nome_arq = nome_arq->prox;
    }
    free(vetor);
}
