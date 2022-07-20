#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>

using namespace std;

Client *client = NULL;

// Função auxiliar para fechar aplicação através do CTRL + C
void ctrlc_handler(int s) {
    if (client != NULL)
        client->set_entrada("/quit");
    else
        exit(1);
}

int main(){

    // Leitura do nickname:
    string nick;
    cout << "Bem vindo! Para comecar, digite um nickname: ";
    cin >> nick;

    // Criação do cliente:
    client = new Client(nick);

    // Definição de ação para tratar SIGINT
    signal(SIGINT, ctrlc_handler);

    // Limpeza do buffer do teclado (\n):
    string entrada;
    getline(cin, entrada);

    // Inicialização da thread
    client->start_thread();

    // Finalização da thread
    client->join_thread();

    // Fechamento do socket do cliente:
    close(client->get_fd_cliente());
    cout << "Socket fechado!\n";
    delete client;

    return 0;
}