#ifndef CPU_E_H
#define CPU_E_H

#include "err.h"

// modos de execução da CPU
//   supervisor: executa todas as instruções
//   usuario: não executa instruções privilegiadas (causa interrupção)
//   zumbi: não executa (para quando o SO não tiver processos pronto)
typedef enum { supervisor, usuario, zumbi } cpu_modo_t;

// TAD para manter o estado interno da CPU (valores dos registradores, modo de execução, etc)

typedef struct cpu_estado_t
{
  int PC;
  int A;
  int X;
  err_t erro;      // Read-only.
  int complemento; // Read-only.
  cpu_modo_t modo;

} cpu_estado_t;  // estrutura desopaca >:(

// imprime o conteúdo de um descritor (para debug)
void cpue_imprime(cpu_estado_t *self);

void cpue_muda_erro(cpu_estado_t *self, err_t err, int complemento);

#endif // CPU_E_H
