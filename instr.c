#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "instr.h"

static char *nomes    [] = { INSTR_XS(EXPAND_STR_1_OF_3)   };
static int   nums_args[] = { INSTR_XS(EXPAND_3_SUB_1_OF_3) };

opcode_t instr_opcode(char *nome)
{
  if (nome == NULL) return -1;
  for (int i=0; i<N_OPCODE; i++) {
    if (strcasecmp(nomes[i], nome) == 0) {
      return i;
    }
  }
  return -1;
}

char *instr_nome(int opcode)
{
  assert(opcode >= 0);
  assert(opcode <  N_OPCODE);
  return nomes[opcode];
}

int instr_num_args(int opcode)
{
  assert(opcode >= 0);
  assert(opcode <  N_OPCODE);
  return nums_args[opcode];
}

