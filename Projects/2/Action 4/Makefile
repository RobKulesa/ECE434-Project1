INPUT=action4
OUTPUT=action4
CFLAGS=-g -Wall 
LFLAGS=-lm -lpthread

%: %.c %.h
	gcc $(CFLAGS) -o $@ $< $(LFLAGS)

%: %.c
	gcc $(CFLAGS) -o $(OUTPUT) $< $(LFLAGS)

all: $(INPUT)

clean:
	rm -f *.o $(OUTPUT)