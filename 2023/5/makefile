all:
	gcc -Wextra -Wall -Werror -Werror=vla -Wstrict-prototypes -pedantic-errors -std=c11 -g seedmap.c -o seedmap.out
	./seedmap.out

bruteforce: 
	gcc -Wextra -Wall -Werror -Werror=vla -Wstrict-prototypes -pedantic-errors -std=c11 -g multithreading.c -o multithread.out
	./multithread.out

debug:
	gdb ./seedmap.out