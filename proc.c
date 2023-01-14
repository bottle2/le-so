#include <assert.h>
#include <stddef.h>

#include "mem.h"
#include "proc.h"
#include "prog.h"

void proc_setup(struct procs self[static 1], mem_t *mem)
{
  assert(self != NULL);
  assert(mem  != NULL);

  self->n_proc    = 0;
  self->executing = -1;

  self->mem = mem;
}

bool proc_cria(struct procs self[static 1], int prog_i)
{
  assert(self != NULL);
  assert(prog_i >= 1);
  assert(prog_i <= n_prog);

  mem_grava(self->mem, prog_images[prog_i - 1], prog_sizes[prog_i - 1]);

  return true;
}

void proc_termina(struct procs self[static 1])
{
  assert(self != NULL);

  // XXX
}
