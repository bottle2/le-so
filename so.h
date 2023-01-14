#ifndef SO_H
#define SO_H

#include "contr.h"
#include "proc.h"
#include "tela.h"

// Simula o sistema operacional

// as chamadas de sistema
typedef enum {
  SO_LE = 1,       // lê do dispositivo em A; coloca valor lido em X
  SO_ESCR,         // escreve o valor em X no dispositivo em A
  SO_FIM,          // encerra a execução do processo
  SO_CRIA,         // cria um novo processo, para executar o programa A
} so_chamada_t;

typedef struct so_t
{
  contr_t contr;
  bool paniquei;        // apareceu alguma situação intratável
  struct procs procs;   // Tabela de processos.
} so_t;

#include "err.h"
#include <stdbool.h>

void so_init(so_t self[static 1]);

// o laço principal da simulação
void so_laco(so_t *self);

#endif // SO_H
