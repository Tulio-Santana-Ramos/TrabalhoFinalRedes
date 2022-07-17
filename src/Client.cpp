#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>

using namespace std;

// Variável auxiliar para tratamento de input
string aux_input = "";

// Função auxiliar para fechar aplicação através do CTRL + C
void ctrlc_handler(int s) {
    aux_input = "/quit";
}

// Construtor
Client::Client(string nick){
    // Inicialização inicial do nickname
    Client::nickname = nick;

    // Inicialização inicial para permitir envio de mensagens
    Client::muted = false;

    // Definição de administrador de canal
    Client::administra = false;

    // Inicialização da variável de controle de estado do cliente
    Client::shutdown = false;

    // Criação do socket do Cliente
    Client::fd_cliente = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi criado
    if(Client::fd_cliente == -1){
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\n";

    // Definição de ação para tratar SIGINT
    signal(SIGINT, ctrlc_handler);
}

// Função para conexão com a aplicação Servidor
bool Client::conectar_servidor(int fd_cliente, sockaddr_in *endereco_servidor){
    // Configuração da porta e IP para o mesmo endereço do servidor:
    endereco_servidor->sin_family = AF_INET;
    endereco_servidor->sin_port = htons(2000);

    // O endereço IP abaixo pode ser setado para o local, caso rode o servidor e esta aplicação na mesma máquina
    // Assim como pode ser inserido um IP distinto, caso outra máquina na mesma rede esteja executando a aplicação de servidor
    endereco_servidor->sin_addr.s_addr = inet_addr("127.0.0.1");

    // Retorno da conexão com o servidor:
    return connect(fd_cliente, (sockaddr *) endereco_servidor, sizeof(*endereco_servidor)) != -1;
}

// Função para envio de mensagens ao servidor
bool Client::mandar_mensagem_servidor(int fd_cliente, string mensagem_total, char* buffer_mensagem){
    // Inicialização do buffer de mansagens
    memset(buffer_mensagem, 0, LIMITE_MENSAGEM * sizeof(char));

    // Envio da mensagem em blocos de no máximo LIMITE_MENSAGEM:
    int j = 0;
    for (int i = 0; i < mensagem_total.size(); i++) {
        buffer_mensagem[j++] = mensagem_total[i];
        if (i == mensagem_total.size() - 1 || j == LIMITE_MENSAGEM - 1) {
            buffer_mensagem[j] = '\0';
            // Envia um bloco da mensagem ao servidor:
            if (send(fd_cliente, buffer_mensagem, strlen(buffer_mensagem) + 1, MSG_NOSIGNAL) == -1)
                return false;
            j = 0;
        }
    }

    // Sinaliza final do envio:
    buffer_mensagem[0] = '\0';
    if (send(fd_cliente, buffer_mensagem, 1, MSG_NOSIGNAL) == -1)
        return false;
    return true;
}

// Função para setar socket cliente
void Client::set_fd(int fd_cliente){
    Client::fd_cliente = fd_cliente;
}

// Função para setar endereço cliente
void Client::set_address(sockaddr_in end_cliente){
    Client::endereco_cliente = end_cliente;
}