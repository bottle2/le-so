# Trabalho de SO

## Informações adicionais

### `SO_LE` vs instrução `LE`

Hardware sem SO usa instruções `LE` e `ESCR`.
Depois que SO entra, `LE` e `ESCR` somem.
Durante toda execução a CPU fica em modo usuário,
modo supervisor só o SO, mas o SO tá escrito em C.

## Alterações relevantes

- [X] Carregar múltiplos programas durante inicialização.
- [X] Definir programa inicial dentre os carregados.
- [X] Argumento para definir programa inicial.
- [X] Função de cópia integral de imagem para memória.
- [ ] Função de cópia integral da memória para buffer.
- [X] Tabela de processos
  - [X] Estado da CPU por programa.
  - [X] Estado do processo (execução, pronto ou bloqueado).
  - [X] Razão do bloqueio, se houver.
  - [ ] Como/quando desbloquear, se for o caso.
  - [X] Cópia da memória principal por processo.
- [ ] Função de criação de processo.
- [ ] Função de término de processo.
- [ ] Colocar CPU em modo usuário.

## Outras alterações

- [X] Removido tipos opacos.
- [X] Removido destrutores e construtores.
- [X] Adicionado inicializadores, quando relevante.
- [X] Removido acessores.
- [X] Corrigido direção de controle e dependência cíclica entre SO e CPU.
- [X] Unido dados paralelos.
- [X] Dependências explicitadas no Makefile.

## Correções

- Fila do terminal devolvia último valor, apesar de remover o primeiro.
