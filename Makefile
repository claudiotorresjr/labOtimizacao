       PROG   = matmult
    MODULOS   = matriz utils

           CC = gcc -std=c11 -g
         OBJS = $(addsuffix .o,$(MODULOS))

       LIKWID = /home/soft/likwid
 LIKWID_FLAGS = -I$(LIKWID)/include
  LIKWID_LIBS = -L$(LIKWID)/lib

       CFLAGS = $(LIKWID_FLAGS) 
       LFLAGS = $(LIKWID_LIBS) -lm

.PHONY: all clean limpa purge faxina distclean debug avx likwid

%.o: %.c %.h utils.h
	$(CC) $(CFLAGS) -c $<

all: $(PROG)

debug:     CFLAGS += -DDEBUG

likwid:    CFLAGS += -DLIKWID_PERFMON
likwid:    LFLAGS += -llikwid

likwid debug: $(PROG)

$(PROG):  $(PROG).o

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge distclean:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) *.o core a.out
	@rm -f marker.out *.log
