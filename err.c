#include "err.h"
#include "expand.h"

static char *nomes[N_ERR] = { ERR_XS(EXPAND_2_OF_2) };

// retorna o nome de erro
char *err_nome(err_t err)
{
  return (err < 0 || err >= N_ERR) ? "DESCONHECIDO" : nomes[err];
}
