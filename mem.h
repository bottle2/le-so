#ifndef MEM_H
#define MEM_H

// simulador da memória principal
// é um vetor de inteiros

#include "err.h"

#define MEM_TAM 2000        // tamanho da memória principal

// tipo de dados para representar uma região de memória
typedef struct mem_t
{
  int conteudo[MEM_TAM];

} mem_t;

// coloca na posição apontada por 'pvalor' o valor no endereço 'endereco'
// retorna erro ERR_END_INV (e não altera '*pvalor') se endereço inválido
err_t mem_le(mem_t *self, int endereco, int *pvalor);

// coloca 'valor' no endereço 'endereco' da memória
// retorna erro ERR_END_INV se endereço inválido
err_t mem_escreve(mem_t *self, int endereco, int valor);

// grava 'origem' em 'self' da origem até 'tam'
// sempre funciona, morre se impossível
void mem_grava(mem_t *self, int const *origem, int tam);

#endif // MEM_H
