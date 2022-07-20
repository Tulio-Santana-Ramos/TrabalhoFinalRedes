#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>

using namespace std;

string entrada;

// Função auxiliar para fechar aplicação através do CTRL + C
void ctrlc_handler(int s) {
    entrada = "/quit";
}

int main(){

    // Leitura do nickname:
    string nick;
    cout << "Bem vindo! Para comecar, digite um nickname: ";
    cin >> nick;

    // Criação do cliente:
    Client *client = new Client(nick);

    // Inicialização de variáveis auxiliares:
    //sockaddr_in endereco_servidor;
    //char mensagem_servidor[LIMITE_MENSAGEM];

    // Definição de ação para tratar SIGINT
    signal(SIGINT, ctrlc_handler);

    // Obtenção do input
    getline(cin, entrada);

    while(!client->get_shutdown()){
        // Inicialização da thread
        client->start_thread();
    }

    //close(client->get_fd_cliente());
    cout << "Socket fechado!\n";

    return 0;
}