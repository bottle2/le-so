#ifndef ERR_H
#define ERR_H

#include "expand.h"

#define ERR_XS(X)                                                                \
X(ERR_OK        , "OK"                    ), /* sem erro */                       \
X(ERR_END_INV   , "Endereço inválido"     ), /* endereço inválido */              \
X(ERR_OP_INV    , "Operação inválida"     ), /* operação inválida */              \
X(ERR_OCUP      , "Dispositivo ocupado"   ), /* dispositivo ocupado */            \
X(ERR_CPU_PARADA, "CPU parada"            ), /* CPU está com execução suspensa */ \
X(ERR_INSTR_PRIV, "Instrução privilegiada"), /* instrução privilegiada */         \
X(ERR_INSTR_INV , "Instrução inválida"    ), /* instrução inválida */             \
X(ERR_TIC       , "Interrupção de relógio"), /* interrupção do relógio */         \
X(ERR_SISOP     , "Chamada de sistema"    )  /* chamada de sistema */

// os tipos de erro que podem acontecer ao tentar executar uma instrução
typedef enum { ERR_XS(EXPAND_1_OF_2), N_ERR } err_t;

// retorna o nome de erro
char *err_nome(err_t err);

#endif // ERR_H
