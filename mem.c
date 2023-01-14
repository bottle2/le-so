#include "mem.h"
#include <assert.h>
#include <stdlib.h>

// função auxiliar, verifica se endereço é válido
static err_t verif_permissao(mem_t *self, int endereco)
{
  if (endereco < 0 || endereco >= MEM_TAM) {
    return ERR_END_INV;
  }
  return ERR_OK;
}

err_t mem_le(mem_t *self, int endereco, int *pvalor)
{
  err_t err = verif_permissao(self, endereco);
  if (err == ERR_OK) {
    *pvalor = self->conteudo[endereco];
  }
  return err;
}

err_t mem_escreve(mem_t *self, int endereco, int valor)
{
  err_t err = verif_permissao(self, endereco);
  if (err == ERR_OK) {
    self->conteudo[endereco] = valor;
  }
  return err;
}

void mem_grava(mem_t *self, int const *origem, int tam)
{
  assert(MEM_TAM >= tam);

  for (int i = 0; i < tam; i++) {
    self->conteudo[i] = origem[i];
  }
}
