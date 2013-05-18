#
# execute env.sh to set compiler env first then make
#
CC = gcc
LD = gcc

#LDFLAGS += -lrt -pthread
EXTRA_CFLAGS += -g -Wall 
EXE  = batlvl

all: $(EXE)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $^


batlvl: batlvl.o
	$(CC) $(EXTRA_CFLAGS) -o $@ batlvl.o $(LDLIBS)

clean:
	-rm -f $(EXE) *.o



