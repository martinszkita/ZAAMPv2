__start__: obj __lines_for_space__ interp __plugin__ commands
	export LD_LIBRARY_PATH="./libs"; ./interp commands

obj:
	mkdir obj

__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo

__plugin__:
	cd plugin; make

CPPFLAGS=-Wall -pedantic -std=c++17 -Iinc
LDFLAGS=-Wall

obj/main.o: src/main.cpp
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

# rozwijanie makr preprocesora dla dowolnego pliku z obecnej sciezki
# uzycie: make commands FILE=<plik.cmd>

FILE ?= commands.cmd # domyslny plik, jesli nie zostanie podany zaden inny
OUTPUT := $(basename $(FILE)) # plik z rozwinietymi makrami i usunietymi komentarzami

commands: $(FILE)
	@echo "Rozwijam makra z pliku $(FILE) -> $(OUTPUT)!"
	g++ -x c++ -E -P $(FILE) -o $(OUTPUT)

interp: obj/main.o commands
	g++ ${LDFLAGS} -o interp  obj/main.o -ldl

doc:
	cd dox; make

clean:
	rm -f obj/* interp core*

clean_plugin:
	cd plugin; make clean

cleanall: clean
	cd plugin; make cleanall
	cd dox; make cleanall
	rm -f libs/*
	find . -name \*~ -print -exec rm {} \;


 
