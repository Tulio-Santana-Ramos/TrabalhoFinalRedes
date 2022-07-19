#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Server.hpp>

using namespace std;

Server::Server(){
    // Inicialização de variável de controle de estado do servidor
    Server::shutdown = false;
    Server::tamanho_endereco = sizeof(Server::endereco_servidor);

    // Criação do socket
    Server::fd_servidor = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi criado
    if(Server::fd_servidor == -1){
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\n";

    // Configuração do endereço do servidor:
    Server::endereco_servidor.sin_family = AF_INET;
    Server::endereco_servidor.sin_port = htons(2000);
    Server::endereco_servidor.sin_addr.s_addr = INADDR_ANY;

    // Atribuição do endereço ao socket:
    if(bind(Server::fd_servidor, (sockaddr *) &(Server::endereco_servidor), sizeof(Server::endereco_servidor)) == -1){
        cerr << "Erro ao realizar bind do socket!\n";
        exit(-1);
    }else{
        cout << "Bind realizado com sucesso!\n";
    }

    // TODO : Definir quantos clientes colocar aqui a princípio, ou se há forma de alterar dinamicamente
    // Marcação do socket para a escuta de um único cliente:
    if(listen(Server::fd_servidor, 1) == -1){
        cerr << "Erro na marcação de escuta por clientes!\n";
        exit(-1);
    }else{
        cout << "Servidor escutando por requisições!\n";
    }
}

// Função para obter socket servidor
int Server::get_fd_servidor(void){
    return Server::fd_servidor;
}

// Função para setar socket servidor
void Server::set_fd_servidor(int fd){
    Server::fd_servidor = fd;
}

// Função para obter shutdown
bool Server::get_shutdown(void){
    return Server::shutdown;
}

// Função para setar shutdown
void Server::set_shutdown(bool shutdown){
    Server::shutdown = shutdown;
}

// Função para obter mensagem:
char* Server::get_mensagem(void){
    return Server::mensagem_servidor;
}

// Função para setar mensagem:
void Server::set_mensagem(char* mensagem){
    strcpy(Server::mensagem_servidor, mensagem);
}