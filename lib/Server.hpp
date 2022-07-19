#ifndef _SERVER_H_
#define _SERVER_H_

#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>
#include <../lib/Channel.hpp>

using namespace std;

// Classe do Servidor da aplicação
class Server{
    private:
        // Variável de controle
        bool shutdown;

        // Variável para criação do socket
        int fd_servidor;

        // Vetor de clientes conectados
        vector <Client> clientes;

        // Vetor de canais disponíveis
        vector <Channel> canais;

        // Endereço do Servidor
        sockaddr_in endereco_servidor;

        // Buffer para mensagens
        char mensagem_servidor[LIMITE_MENSAGEM];

    public:
        // Tamanho do endereço utilizado pelos clientes
        socklen_t tamanho_endereco;

        // Construtor
        Server();

        // Get fd_servidor
        int get_fd_servidor(void);

        // Set fd_servidor
        void set_fd_servidor(int fd);

        // Get shutdown
        bool get_shutdown(void);

        // Set shutdown
        void set_shutdown(bool shutdown);

        // Get mensagem
        char* get_mensagem(void);

        // Set mensagem
        void set_mensagem(char *mensagem);

    // TODO: Adaptação do loop while presente em Servidor.cpp em Oldfiles
};

#endif /*_SERVER_H_*/