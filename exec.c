#include <stdbool.h>
#include <stdlib.h>

#include "exec.h"
#include "instr.h"

void exec_copia_estado(exec_t *self, cpu_estado_t *estado)
{
  *estado = self->estado;
}

void exec_altera_estado(exec_t *self, cpu_estado_t *estado)
{
  self->estado = *estado;
}


// ---------------------------------------------------------------------
// funções auxiliares para usar durante a execução das instruções
// alteram o estado da CPU caso ocorra erro

// lê um valor da memória
static bool pega_mem(exec_t *self, int endereco, int *pval)
{
  err_t err = mem_le(self->mem, endereco, pval);
  if (err != ERR_OK) {
    cpue_muda_erro(&self->estado, err, endereco);
  }
  return err == ERR_OK;
}

// lê o opcode da instrução no PC
static bool pega_opcode(exec_t *self, int *popc)
{
  return pega_mem(self, self->estado.PC, popc);
}

// lê o argumento 1 da instrução no PC
static bool pega_A1(exec_t *self, int *pA1)
{
  return pega_mem(self, self->estado.PC + 1, pA1);
}

static void incrementa_PC(exec_t *self)
{
   self->estado.PC++;
}

static void incrementa_PC2(exec_t *self)
{
   self->estado.PC += 2;
}

// escreve um valor na memória
static bool poe_mem(exec_t *self, int endereco, int val)
{
  err_t err = mem_escreve(self->mem, endereco, val);
  if (err != ERR_OK) {
    cpue_muda_erro(&self->estado, err, endereco);
  }
  return err == ERR_OK;
}

// lê um valor da E/S
static bool pega_es(exec_t *self, int dispositivo, int *pval)
{
  err_t err = es_le(self->es, dispositivo, pval);
  if (err != ERR_OK) {
    cpue_muda_erro(&self->estado, err, dispositivo);
  }
  return err == ERR_OK;
}

// escreve um valor na E/S
static bool poe_es(exec_t *self, int dispositivo, int val)
{
  err_t err = es_escreve(self->es, dispositivo, val);
  if (err != ERR_OK) {
    cpue_muda_erro(&self->estado, err, dispositivo);
  }
  return err == ERR_OK;
}

// ---------------------------------------------------------------------
// funções auxiliares para implementação de cada instrução

static void op_NOP(exec_t *self) // não faz nada
{
  incrementa_PC(self);
}

static void op_PARA(exec_t *self) // para a CPU
{
  if (self->estado.modo != supervisor) {
    cpue_muda_erro(&self->estado, ERR_INSTR_PRIV, PARA);
    return;
  }
  cpue_muda_erro(&self->estado, ERR_CPU_PARADA, 0);
}

static void op_CARGI(exec_t *self) // carrega imediato
{
  int A1;
  if (pega_A1(self, &A1)) {
    self->estado.A = A1;
    incrementa_PC2(self);
  }
}

static void op_CARGM(exec_t *self) // carrega da memória
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    self->estado.A = mA1;
    incrementa_PC2(self);
  }
}

static void op_CARGX(exec_t *self) // carrega indexado
{
  int A1, mA1mX;
  int X = self->estado.X;
  if (pega_A1(self, &A1) && pega_mem(self, A1+X, &mA1mX)) {
    self->estado.A = mA1mX;
    incrementa_PC2(self);
  }
}

static void op_ARMM(exec_t *self) // armazena na memória
{
  int A1;
  if (pega_A1(self, &A1) && poe_mem(self, A1, self->estado.A)) {
    incrementa_PC2(self);
  }
}

static void op_ARMX(exec_t *self) // armazena indexado
{
  int A1;
  int X = self->estado.X;
  if (pega_A1(self, &A1) && poe_mem(self, A1+X, self->estado.A)) {
    incrementa_PC2(self);
  }
}

static void op_MVAX(exec_t *self) // copia A para X
{
  self->estado.X = self->estado.A;
  incrementa_PC(self);
}

static void op_MVXA(exec_t *self) // copia X para A
{
  self->estado.A = self->estado.X;
  incrementa_PC(self);
}

static void op_INCX(exec_t *self) // incrementa X
{
  self->estado.X++;
  incrementa_PC(self);
}

#define DECL_op_MAT(NAME, OP) \
static void op_##NAME(exec_t *self) \
{ \
  int A1, mA1; \
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) { \
    self->estado.A OP##= mA1; \
    incrementa_PC2(self); \
  } \
}

DECL_op_MAT(SOMA , +) // Soma.
DECL_op_MAT(SUB  , -) // Subtração.
DECL_op_MAT(MULT , *) // Multiplicação.
DECL_op_MAT(DIV  , /) // Divisão.
DECL_op_MAT(RESTO, %) // Resto.

static void op_NEG(exec_t *self) // inverte sinal
{
  self->estado.A = -self->estado.A;
  incrementa_PC(self);
}

static void op_DESV(exec_t *self) // desvio incondicional
{
  int A1;
  if (pega_A1(self, &A1)) {
    self->estado.PC = A1;
  }
}

#define DECL_op_DESC(NAME, OP) \
static void op_##NAME(exec_t *self) \
{ \
  if (self->estado.A OP 0) { \
    op_DESV(self); \
  } else { \
    incrementa_PC2(self); \
  } \
}

DECL_op_DESC(DESVZ , ==)
DECL_op_DESC(DESVNZ, !=)
DECL_op_DESC(DESVN , < )
DECL_op_DESC(DESVP , & )

static void op_CHAMA(exec_t *self) // chamada de subrotina
{
  int A1;
  int PC = self->estado.PC;
  if (pega_A1(self, &A1) && poe_mem(self, A1, PC+2)) {
    self->estado.PC = A1 + 1;
  }
}

static void op_RET(exec_t *self) // retorno de subrotina
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    self->estado.PC = mA1;
  }
}
static void op_LE(exec_t *self) // leitura de E/S
{
  if (self->estado.modo != supervisor) {
    cpue_muda_erro(&self->estado, ERR_INSTR_PRIV, LE);
    return;
  }
  int A1, dado;
  if (pega_A1(self, &A1) && pega_es(self, A1, &dado)) {
    self->estado.A = dado;
    incrementa_PC2(self);
  }
}

static void op_ESCR(exec_t *self) // escrita de E/S
{
  if (self->estado.modo != supervisor) {
    cpue_muda_erro(&self->estado, ERR_INSTR_PRIV, ESCR);
    return;
  }
  int A1;
  if (pega_A1(self, &A1) && poe_es(self, A1, self->estado.A)) {
    incrementa_PC2(self);
  }
}

static void op_SISOP(exec_t *self) // chamada ao SO
{
  int A1;
  if (pega_A1(self, &A1)) {
    cpue_muda_erro(&self->estado, ERR_SISOP, A1);
    // não incrementa o PC, o SO deve fazer isso
  }
}


err_t exec_executa_1(exec_t *self)
{
  // não executa se CPU estiver em estado zumbi
  if (self->estado.modo == zumbi) return ERR_OK;
  // não executa se CPU já estiver em erro
  if (self->estado.erro != ERR_OK) return self->estado.erro;

  int opcode;
  if (!pega_opcode(self, &opcode)) return self->estado.erro;

  switch (opcode) {
    case NOP:    op_NOP(self);    break;
    case PARA:   op_PARA(self);   break;
    case CARGI:  op_CARGI(self);  break;
    case CARGM:  op_CARGM(self);  break;
    case CARGX:  op_CARGX(self);  break;
    case ARMM:   op_ARMM(self);   break;
    case ARMX:   op_ARMX(self);   break;
    case MVAX:   op_MVAX(self);   break;
    case MVXA:   op_MVXA(self);   break;
    case INCX:   op_INCX(self);   break;
    case SOMA:   op_SOMA(self);   break;
    case SUB:    op_SUB(self);    break;
    case MULT:   op_MULT(self);   break;
    case DIV:    op_DIV(self);    break;
    case RESTO:  op_RESTO(self);  break;
    case NEG:    op_NEG(self);    break;
    case DESV:   op_DESV(self);   break;
    case DESVZ:  op_DESVZ(self);  break;
    case DESVNZ: op_DESVNZ(self); break;
    case DESVN:  op_DESVN(self);  break;
    case DESVP:  op_DESVP(self);  break;
    case CHAMA:  op_CHAMA(self);  break;
    case RET:    op_RET(self);    break;
    case LE:     op_LE(self);     break;
    case ESCR:   op_ESCR(self);   break;
    case SISOP:  op_SISOP(self);  break;
    default:     cpue_muda_erro(&self->estado, ERR_INSTR_INV, 0);
  }

  return self->estado.erro;
}
