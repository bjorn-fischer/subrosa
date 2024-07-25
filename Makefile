
CFLAGS = -O -Wall -pedantic

subrosa: subrosa.c
	gcc $(CFLAGS) -o $@ $<
	@echo $@ must be installed SUID root

clean:
	rm -f subrosa

