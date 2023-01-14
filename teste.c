#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "proc.h"
#include "prog.h"
#include "so.h"
#include "tela.h"

static char *nome = NULL;

static void usage(void);
static int progr_escolhe(char const *algum);

int main(int argc, char *argv[argc])
{
  nome = argv[0];

  t_inicio();

  so_t so = {0};
  so_init(&so);

  for (int arg_i = 0; arg_i < argc - 1; arg_i++)
  {
    proc_cria(&so.procs, progr_escolhe(argv[arg_i + 1]));
  }

  if (1 == argc)
  {
    proc_cria(&so.procs, PROG_DEFAULT);
  }

  so_laco(&so);

  t_fim();
  return 0;
}

static void usage(void)
{
  assert(nome != NULL);
  printf("Usage: %s [1-%d]\n", nome, n_prog);
  exit(EXIT_FAILURE);
}

static int progr_escolhe(char const *algum)
{
  assert(algum != NULL);

  int const progr = atoi(algum);

  if (progr <= 0 || progr > n_prog) usage();

  return progr;
}
