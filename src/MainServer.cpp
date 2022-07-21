#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>
#include <../lib/Server.hpp>

using namespace std;

int main(){

    // Inicialização do servidor:
    Server *server = new Server();

    server->start_thread();

    server->join_thread();

    // Fechamento do socket:
    close(server->get_fd_servidor());
    cout << "Socket fechado!\n";
    delete server;

    return 0;
}