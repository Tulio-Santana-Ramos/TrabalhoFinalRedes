# Este novo makefile ainda precisa de alterações
.PHONY : client server all srun crun clean

all: client server

client:	#lib/Client.hpp
# g++ -g -c Cliente.cpp
# g++ -g -o ClientSide Cliente.o -Wall -Werror
# rm Cliente.o
	g++ -o ClientSide src/Client.cpp src/MainClient.cpp -Wall -Werror -I lib

server: lib/Server.hpp
# g++ -g -c Servidor.cpp
# g++ -g -o ServerSide Servidor.o -Wall -Werror
# rm Servidor.o
	g++ -o ServerSide src/Server.cpp src/Client.cpp src/MainServer.cpp -Wall -Werror -I lib

srun:
	./ServerSide

crun:
	./ClientSide

clean:
	rm ServerSide
	rm ClientSide

zip: clean
	zip -r TrabalhoFinalRedes.zip Makefile lib/* src/*