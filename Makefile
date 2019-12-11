CC ?=g++
CFLAGS ?=-g

main: main.o fonctions.o
	g++ $(CFLAGS) ./obj/main.o ./obj/fonctions.o -o ./bin/main -lsfml-graphics -lsfml-window -lsfml-system

main.o: ./src/main.cc
	g++ $(CFLAGS) -o ./obj/main.o -c ./src/main.cc

fonctions.o: ./src/fonctions.cc
	g++ $(CFLAGS) -o ./obj/fonctions.o -c ./src/fonctions.cc

.PHONY: clean
clean:

	rm -rf ./obj/*.o
	rm -f ./bin/main
