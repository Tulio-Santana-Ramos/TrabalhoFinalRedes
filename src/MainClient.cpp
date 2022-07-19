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
    sockaddr_in endereco_servidor;
    char mensagem_servidor[LIMITE_MENSAGEM];

    // Definição de ação para tratar SIGINT
    signal(SIGINT, ctrlc_handler);

    getline(cin, entrada);

    while (!client->get_shutdown()) {

        cout << "Digite sua mensagem ou comando: \n";
        getline(cin, entrada);

        if (entrada.length() == 0) {
            entrada = "/quit";

        } else if (entrada == "/connect") {
            // Conexão com o servidor:
            if(client->conectar_servidor(&endereco_servidor))
                cout << "Conexão com servidor efetuada com sucesso\n";
            else
                cerr << "Erro ao conectar com o servidor!\n";
        } else {
            if (entrada == "/quit")
                client->set_shutdown(true);
            // Tentativa de envio da mensagem:
            if (client->mandar_mensagem_servidor(entrada))
                cout << "Mensagem enviada com sucesso!\n";
            else
                cerr << "Erro ao enviar a mensagem!\n";
            if (entrada == "/ping") {
                int recv_response = recv(
                    client->get_fd_cliente(),
                    mensagem_servidor,
                    sizeof(mensagem_servidor),
                    MSG_NOSIGNAL
                );
                if (recv_response != -1)
                    cout << "Servidor respondeu: " << mensagem_servidor << "\n";
            }
        } 
    }
    
    close(client->get_fd_cliente());
    cout << "Socket fechado!\n";

    return 0;
}