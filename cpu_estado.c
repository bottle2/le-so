#include "cpu_estado.h"
#include <stdlib.h>
#include <stdio.h>

void cpue_muda_erro(cpu_estado_t *self, err_t err, int complemento)
{
  self->erro = err;
  self->complemento = complemento;
}
