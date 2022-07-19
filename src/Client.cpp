#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>

using namespace std;

// // Variável auxiliar para tratamento de input
// string aux_input = "";

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
}

// Função para conexão com a aplicação Servidor
bool Client::conectar_servidor(sockaddr_in *endereco_servidor){
    // Configuração da porta e IP para o mesmo endereço do servidor:
    endereco_servidor->sin_family = AF_INET;
    endereco_servidor->sin_port = htons(2000);

    // O endereço IP abaixo pode ser setado para o local, caso rode o servidor e esta aplicação na mesma máquina
    // Assim como pode ser inserido um IP distinto, caso outra máquina na mesma rede esteja executando a aplicação de servidor
    endereco_servidor->sin_addr.s_addr = inet_addr("127.0.0.1");

    // Retorno da conexão com o servidor:
    return connect(Client::fd_cliente, (sockaddr *) endereco_servidor, sizeof(*endereco_servidor)) != -1;
}

// Função para envio de mensagens ao servidor
bool Client::mandar_mensagem_servidor(string mensagem_total){
    // Inicialização do buffer de mansagens
    memset(Client::mensagem_cliente, 0, LIMITE_MENSAGEM * sizeof(char));

    // Envio da mensagem em blocos de no máximo LIMITE_MENSAGEM:
    int j = 0;
    for (uint i = 0; i < mensagem_total.length(); i++) {
        Client::mensagem_cliente[j++] = mensagem_total[i];
        if (i == mensagem_total.length() - 1 || j == LIMITE_MENSAGEM - 1) {
            Client::mensagem_cliente[j] = '\0';
            // Envia um bloco da mensagem ao servidor:
            if (send(Client::fd_cliente, Client::mensagem_cliente, strlen(Client::mensagem_cliente) + 1, MSG_NOSIGNAL) == -1)
                return false;
            j = 0;
        }
    }

    // Sinaliza final do envio:
    Client::mensagem_cliente[0] = '\0';
    if (send(Client::fd_cliente, Client::mensagem_cliente, 1, MSG_NOSIGNAL) == -1)
        return false;
    return true;
}

// Função para limpar o buffer de mensagem:
void Client::limpar_buffer(void){
    bzero(Client::mensagem_cliente, LIMITE_MENSAGEM);
}

// Função para obter socket cliente
int Client::get_fd_cliente(void){
    return Client::fd_cliente;
}

// Função para setar socket cliente
void Client::set_fd_cliente(int fd_cliente){
    Client::fd_cliente = fd_cliente;
}

// Função para obter shutdown
bool Client::get_shutdown(void){
    return Client::shutdown;
}

// Função para setar shutdown
void Client::set_shutdown(bool shutdown){
    Client::shutdown = shutdown;
}

char* Client::get_mensagem(void){
    return mensagem_cliente;
}

// Função para obter endereço servidor
sockaddr_in Client::get_endereco_servidor(void){
    return Client::endereco_servidor;
}

// Função para setar endereço servidor
void Client::set_endereco_servidor(sockaddr_in endereco){
    Client::endereco_servidor = endereco;
}


// // Função para obter endereço cliente
// void Client::set_address(sockaddr_in end_cliente){
//     Client::endereco_cliente = end_cliente;
// }

// // Função para obter endereço cliente
// sockaddr_in Client::get_endereco_cliente(void){
//     return Client::endereco_cliente;
// }

// // Função para setar endereço cliente
// void Client::(sockaddr_in end_cliente){
//     Client::endereco_cliente = end_cliente;
// }
