.PHONY : client server all srun crun clean

client:
	g++ -g -c Cliente.cpp
	g++ -g -o ClientSide Cliente.o -Wall -Werror
	rm Cliente.o

server:
	g++ -g -c Servidor.cpp
	g++ -g -o ServerSide Servidor.o -Wall -Werror
	rm Servidor.o

all:
	client server

srun:
	./ServerSide

crun:
	./ClientSide

clean:
	rm ServerSide
	rm ClientSide