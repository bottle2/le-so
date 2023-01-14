#include <assert.h>
#include <stdlib.h>

#include "proc.h"
#include "prog.h"
#include "so.h"
#include "tela.h"

static void panico(so_t *self);

// houve uma interrupção do tipo err — trate-a
static void so_int(so_t *self, err_t err);

void so_init(so_t self[static 1])
{
  contr_init(&self->contr);
  self->paniquei = false;
  proc_setup(&self->procs, &self->contr.mem);

  // coloca a CPU em modo usuário
  /*
  exec_copia_estado(contr_exec(self->contr), self->cpue);
  cpue_muda_modo(self->cpue, usuario);
  exec_altera_estado(contr_exec(self->contr), self->cpue);
  */
}

// trata chamadas de sistema

// chamada de sistema para leitura de E/S
// recebe em A a identificação do dispositivo
// retorna em X o valor lido
//            A o código de erro
static void so_trata_sisop_le(so_t *self)
{
  // faz leitura assíncrona.
  // deveria ser síncrono, verificar es_pronto() e bloquear o processo
  int disp = self->cpue.A;
  int val;
  err_t err = es_le(&self->contr.es, disp, &val);
  self->cpue.A = err;
  if (err == ERR_OK) {
    self->cpue.X = val;
  }
  // incrementa o PC
  self->cpue.PC += 2;
  // interrupção da cpu foi atendida
  cpue_muda_erro(&self->cpue, ERR_OK, 0);
  // altera o estado da CPU (deveria alterar o estado do processo)
  exec_altera_estado(&self->contr.exec, &self->cpue);
}

// chamada de sistema para escrita de E/S
// recebe em A a identificação do dispositivo
//           X o valor a ser escrito
// retorna em A o código de erro
static void so_trata_sisop_escr(so_t *self)
{
  // faz escrita assíncrona.
  // deveria ser síncrono, verificar es_pronto() e bloquear o processo
  int disp = self->cpue.A;
  int val = self->cpue.X;
  err_t err = es_escreve(&self->contr.es, disp, val);
  self->cpue.A = err;
  // interrupção da cpu foi atendida
  cpue_muda_erro(&self->cpue, ERR_OK, 0);
  // incrementa o PC
  self->cpue.PC += 2;
  // altera o estado da CPU (deveria alterar o estado do processo)
  exec_altera_estado(&self->contr.exec, &self->cpue);
}

// chamada de sistema para término do processo
static void so_trata_sisop_fim(so_t *self)
{
  t_printf("SISOP FIM não implementado");
  panico(self);
  //...
}

// chamada de sistema para criação de processo
static void so_trata_sisop_cria(so_t *self)
{
  t_printf("SISOP CRIA não implementado");
  panico(self);
  //...
}

// trata uma interrupção de chamada de sistema
static void so_trata_sisop(so_t *self)
{
  // o tipo de chamada está no "complemento" do cpue
  exec_copia_estado(&self->contr.exec, &self->cpue);
  so_chamada_t chamada = self->cpue.complemento;
  switch (chamada) {
    case SO_LE:
      so_trata_sisop_le(self);
      break;
    case SO_ESCR:
      so_trata_sisop_escr(self);
      break;
    case SO_FIM:
      so_trata_sisop_fim(self);
      break;
    case SO_CRIA:
      so_trata_sisop_cria(self);
      break;
    default:
      t_printf("so: chamada de sistema não reconhecida %d\n", chamada);
      panico(self);
  }
}

// trata uma interrupção de tempo do relógio
static void so_trata_tic(so_t *self)
{
  // TODO: tratar a interrupção do relógio
}

void so_laco(so_t *self)
{
  // executa uma instrução por vez até SO dizer que chega
  do {
    err_t err;
    err = exec_executa_1(&self->contr.exec);
    if (err != ERR_OK) so_int(self, err);
    err = rel_tictac(&self->contr.rel);
    if (err != ERR_OK) so_int(self, err);
    contr_atualiza_estado(&self->contr);
    t_atualiza();
  } while (!self->paniquei);
      
  t_printf("Fim da execução.");
  t_printf("relógio: %d\n", rel_agora(&self->contr.rel));
}

// houve uma interrupção do tipo err — trate-a
static void so_int(so_t *self, err_t err)
{
  switch (err) {
    case ERR_SISOP:
      so_trata_sisop(self);
      break;
    case ERR_TIC:
      so_trata_tic(self);
      break;
    default:
      t_printf("SO: interrupção não tratada [%s]", err_nome(err));
      self->paniquei = true;
  }
}

static void panico(so_t *self) 
{
  t_printf("Problema irrecuperável no SO");
  self->paniquei = true;
}
