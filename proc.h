#ifndef PROC_H
#define PROC_H

#include <stdbool.h>

#include "cpu_estado.h"
#include "mem.h"

#define PROC_TABLE_SIZE   10
#define PROC_MEMORY     1000

struct procs
{
  int           prog_i  [PROC_TABLE_SIZE]; // Referenciando 'prog.inc'.
  bool          is_ready[PROC_TABLE_SIZE]; // Se não tá pronto, tá bloqueado.
  int          *memory  [PROC_TABLE_SIZE]; // Ponteiro para 'buffer'.
  cpu_estado_t  estados [PROC_TABLE_SIZE]; // Restaura estado da execução.

  int n_proc;    // Quantidade de processos.
  int executing; // Índice do processo em execução, último ou nenhum (-1).

  int buffer[PROC_MEMORY]; // Cópias da memória de processos não executando.

  mem_t *mem;
};

void proc_setup  (struct procs self[static 1], mem_t *mem);
bool proc_cria   (struct procs self[static 1], int prog_i);
void proc_termina(struct procs self[static 1]); // Mata processo ativo.

#endif
