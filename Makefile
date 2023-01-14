CC = gcc
CFLAGS = -Wall -Werror -Wpedantic $$(pkgconf --cflags ncursesw)
LDLIBS = $$(pkgconf --libs ncursesw)

OBJS = exec.o cpu_estado.o es.o mem.o rel.o term.o instr.o err.o \
			 tela.o contr.o so.o teste.o proc.o prog.o
OBJS_MONT = instr.o err.o montador.o
MAQS = ex1.maq ex2.maq ex3.maq ex4.maq ex5.maq init.maq p1.maq p2.maq test1.maq
TARGETS = teste montador

all: ${TARGETS}

# para gerar o montador, precisa de todos os .o do montador
montador: ${OBJS_MONT}

# para gerar o programa de teste, precisa de todos os .o)
teste: ${OBJS}

contr.c:contr.h es.h exec.h instr.h mem.h rel.h tela.h
	touch $@
contr.h:mem.h exec.h rel.h term.h
	touch $@
cpu_estado.c:cpu_estado.h
	touch $@
cpu_estado.h:err.h
	touch $@
err.c:err.h expand.h
	touch $@
err.h:expand.h
	touch $@
es.c:es.h
	touch $@
es.h:err.h
	touch $@
exec.c:exec.h instr.h
	touch $@
exec.h:err.h mem.h es.h cpu_estado.h
	touch $@
instr.c:instr.h
	touch $@
instr.h:expand.h
	touch $@
mem.c:mem.h
	touch $@
mem.h:err.h
	touch $@
montador.c:instr.h
	touch $@
proc.c:proc.h prog.h mem.h
	touch $@
proc.h:cpu_estado.h mem.h
	touch $@
prog.c:prog.h prog.inc
	touch $@
prog.inc:$(MAQS)
	touch $@
rel.c:rel.h
	touch $@
rel.h:err.h
	touch $@
so.c:proc.h prog.h so.h tela.h
	touch $@
so.h:contr.h proc.h tela.h
	touch $@
tela.c:tela.h
	touch $@
term.c:term.h tela.h
	touch $@
term.h:es.h
	touch $@
teste.c:proc.h prog.h so.h tela.h
	touch $@

clean:
	rm -f ${OBJS} ${OBJS_MONT} ${TARGETS} ${MAQS}

run:
	TERM= ./teste.exe

# para transformar um .asm em .maq, precisamos do montador
$(MAQS):montador
.SUFFIXES:.maq .asm
.asm.maq:
	./montador $< > $@
