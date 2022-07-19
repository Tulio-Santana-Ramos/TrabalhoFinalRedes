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

    // Inicialização da variável que conterá o cliente atual (apenas temporário para teste):
    Client *curr_client = new Client("");

    // Aceite de uma requisição:
    int client_fd = accept (
        server->get_fd_servidor(),
        (sockaddr*) &curr_client->endereco_cliente,
        &server->tamanho_endereco
    );

    // Verificação se o aceite foi válido:
    if (client_fd < 0) {
        cerr << "Não foi possível se conectar ao cliente!\n";
        exit(-1);
    }
    cout << "Cliente conectado no IP: " << inet_ntoa(curr_client->endereco_cliente.sin_addr)
         << " e porta " << ntohs(curr_client->endereco_cliente.sin_port) << "\n";

    curr_client->set_fd_cliente(client_fd);

    while (!server->get_shutdown()) {

        // Recepção da(s) mensagem(s) do cliente (pois podem ser quebradas se forem maiores que LIMITE_MENSAGEM):
        while (true) {
            curr_client->limpar_buffer();

            // Recebimento de 1 bloco da mensagem do cliente:
            int recv_response = recv (
                curr_client->get_fd_cliente(),
                curr_client->get_mensagem(),
                LIMITE_MENSAGEM, 0
            );

            if (recv_response == -1) {
                cout << "Mensagem não recebida!\n";
                exit(-1);
            }

            char* mensagem = curr_client->get_mensagem();

            if (strlen(mensagem) == 0)
                break;

            if (strcmp(mensagem, "/quit") == 0) {
                server->set_shutdown(true);
                break;
            }

            if (strcmp(mensagem, "/ping") == 0) {
                cout << "Ping recebido\n";
                char pong[6] = {'P', 'o', 'n', 'g', '!', '\0'};
                server->set_mensagem(pong);
                send(
                    curr_client->get_fd_cliente(),
                    server->get_mensagem(),
                    strlen(server->get_mensagem()) + 1, 0
                );
                break;
            } 

            cout << mensagem << "\n";
        }
    }

    // Fechamento do socket:
    close(server->get_fd_servidor());
    cout << "Socket fechado!\n";

    return 0;

}
    