# Este novo makefile ainda precisa de alterações
.PHONY : client server all srun crun clean

all: client server

client:	#lib/Client.hpp
# g++ -g -c Cliente.cpp
# g++ -g -o ClientSide Cliente.o -Wall -Werror
# rm Cliente.o
	g++ -pthread -lpthread -o ClientSide src/Client.cpp src/MainClient.cpp -Wall -Werror -I lib -g

server: lib/Server.hpp
# g++ -g -c Servidor.cpp
# g++ -g -o ServerSide Servidor.o -Wall -Werror
# rm Servidor.o
	g++ -pthread -lpthread -o ServerSide src/Server.cpp src/Client.cpp src/MainServer.cpp -I lib -g

srun:
	./ServerSide

crun:
	./ClientSide

svalgrind:
	valgrind -s --trace-children=yes --track-fds=yes --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./ServerSide

cvalgrind:
	valgrind -s --trace-children=yes --track-fds=yes --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./ClientSide

clean:
	rm ServerSide
	rm ClientSide

zip: clean
	zip -r TrabalhoFinalRedes.zip Makefile lib/* src/*