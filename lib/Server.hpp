#ifndef _SERVER_H_
#define _SERVER_H_

#include <thread>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>
#include <../lib/Channel.hpp>

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

        // Tamanho do endereço utilizado pelos clientes
        socklen_t tamanho_endereco;

        // Buffer para mensagens
        char mensagem_servidor[LIMITE_MENSAGEM];

    public:
        // Construtor
        Server();

    // TODO: Adaptação do loop while presente em Servidor.cpp em Oldfiles
};

#endif /*_SERVER_H_*/