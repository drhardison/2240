# Project name
SOURCE          = hw1.c
SOURCE2		= hw2.c
ERROR		= error.c
# Build and link executable
go: $(SOURCE) $(ERROR)
	rm -f go
	gcc $(SOURCE) $(ERROR) -o go
2: $(SOURCE2) $(ERROR)
	rm -f 2
	rm -f storage.bin
	gcc $(SOURCE2) $(ERROR) -o 2
