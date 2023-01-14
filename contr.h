#ifndef CONTR_H
#define CONTR_H

// contr
// controla o hardware simulado
// em especial, contém o laço de execução de instruções e verificação de
//   interrupções, com chamada ao SO para tratá-las
// concentra os dispositivos de hardware

#include "mem.h"
#include "exec.h"
#include "rel.h"
#include "term.h"

typedef struct contr_t {
  mem_t mem;
  exec_t exec;
  rel_t rel;
  term_t term;
  es_t es;
} contr_t;

void contr_init(contr_t self[static 1]);

// o laço principal da simulação
void contr_laco(contr_t *self);

void contr_atualiza_estado(contr_t *self);

#endif // CONTR_H
