#ifndef EXEC_H
#define EXEC_H

// simulador do executor de instruções de uma CPU
// executa a instrução no PC se possível, ou retorna erro

// tem acesso a
// - controlador de ES para as instruções de ES
// - memória, onde está o programa e os dados -- alterável pelas instruções
// - estado da CPU, onde estão os registradores -- alterável pelas instruções

#include "err.h"
#include "mem.h"
#include "es.h"
#include "cpu_estado.h"

// uma CPU tem estado, memória, controlador de ES
typedef struct exec_t
{
  cpu_estado_t estado;
  mem_t *mem;
  es_t *es;

} exec_t;

// executa uma instrução
err_t exec_executa_1(exec_t *exec);

#endif // EXEC_H
