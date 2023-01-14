#ifndef TERM_H
#define TERM_H

// simulador do terminal
// realiza a entrada e saída de valores numéricos
// por enquanto só suporta um terminal

#include "es.h"

typedef struct term_t
{
  int dummy; // por enquanto não tem muito o que colocar aqui

} term_t;

// Funções para implementar o protocolo de acesso a um dispositivo pelo
//   controlador de E/S
err_t term_le(void *disp, int id, int *pvalor);
err_t term_escr(void *disp, int id, int valor);
bool term_pronto(void *disp, int id, acesso_t acesso);

#endif // TERM_H
