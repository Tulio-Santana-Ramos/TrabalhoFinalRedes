#ifndef _CLIENT_H_
#define _CLIENT_H_

// Definição do limite de tamanho das mensagens trocadas entre Servidor e Cliente
#define LIMITE_MENSAGEM 4097

#include <thread>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>

// Classe do Servidor da aplicação
class Client{
    private:
        // Variável de controle
        bool shutdown;

        // Variável para criação do socket
        int fd_cliente;

        // Endereço do Servidor
        sockaddr_in endereco_servidor;

        // Endereço do Servidor
        sockaddr_in endereco_cliente;

        // Entrada do Cliente
        string entrada;

        // Apelido
        string nickname;

        // Buffer para mensagens
        char mensagem_cliente[LIMITE_MENSAGEM];

        // Administra canal
        bool administra;

        // Se pode ou não enviar mensagens
        bool muted;

    public:
        // Construtor
        Client(string nick);

        // Conexão com servidor
        bool conectar_servidor(int fd_cliente, sockaddr_in *endereco_servidor);

        // Envio de mansagens
        bool mandar_mensagem_servidor(int fd_cliente, string mensagem_total, char* buffer_mensagem);

        // Set fd_client
        void set_fd(int fd_cliente);

        // Set endereço_cliente
        void set_address(sockaddr_in end_cliente);

        // TODO: Criação e inicialização da thread
        // TODO: Adaptação do loop while presente em Cliente.cpp em Oldfiles
};

#endif /*_CLIENT_H_*/