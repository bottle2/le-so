#include "contr.h"
#include "mem.h"
#include "exec.h"
#include "rel.h"
#include "term.h"
#include "es.h"
#include "tela.h"
#include "instr.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void contr_init(contr_t self[static 1])
{
  // cria dispositivos de E/S (o relógio e um terminal)
  // cria o controlador de E/S e registra os dispositivos
  es_registra_dispositivo(&self->es, 0, &self->term, 0, term_le, term_escr, term_pronto);
  es_registra_dispositivo(&self->es, 1, &self->term, 1, term_le, term_escr, term_pronto);
  es_registra_dispositivo(&self->es, 2, &self->rel, 0, rel_le, NULL, NULL);
  es_registra_dispositivo(&self->es, 3, &self->rel, 1, rel_le, NULL, NULL);
  // cria a unidade de execução e inicializa com a memória e E/S
  self->exec.mem = &self->mem;
  self->exec.es  = &self->es;
}

static void str_estado(char *txt, exec_t *exec, mem_t *mem)
{
  // pega o estado da CPU, imprime registradores, opcode, instrução
  cpu_estado_t estado = exec->estado;
  int pc, opcode = -1;
  pc = estado.PC;
  mem_le(mem, pc, &opcode);
  sprintf(txt, "PC=%04d A=%06d X=%06d %02d %s",
                pc, estado.A, estado.X, opcode, instr_nome(opcode));
  // imprime argumento da instrução, se houver
  if (instr_num_args(opcode) > 0) {
    char aux[40];
    int A1;
    mem_le(mem, pc+1, &A1);
    sprintf(aux, " %d", A1);
    strcat(txt, aux);
  }
  // imprime estado de erro da CPU, se for o caso
  err_t err = estado.erro;
  if (err != ERR_OK) {
    char aux[40];
    sprintf(aux, " E=%d(%d) %s", err, estado.complemento, err_nome(err));
    strcat(txt, aux);
  }
}

void contr_atualiza_estado(contr_t *self)
{
  char s[N_COL+1];
  str_estado(s, &self->exec, &self->mem);
  t_status(s);
}
