#ifndef _CLIENT_H_
#define _CLIENT_H_

// Definição do limite de tamanho das mensagens trocadas entre Servidor e Cliente
#define LIMITE_MENSAGEM 4097

#include <thread>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>

using namespace std;

// Classe do Servidor da aplicação
class Client{
    private:
        // Variável de controle
        bool shutdown;

        // Variável para criação do socket
        int fd_cliente;

        // Identificação do cliente no servidor
        int id;

        // Endereço do Servidor
        sockaddr_in endereco_servidor;

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

        // Thread do cliente
        thread self_thread;

        // Controller
        void thread_logic();

    public:
        // Endereço do Cliente
        sockaddr_in endereco_cliente;

        // Construtor
        Client(string nick);

        // Conexão com servidor
        bool conectar_servidor(sockaddr_in *endereco_servidor);

        // Envio de mansagens
        bool mandar_mensagem_servidor(string mensagem_total);

        // Limpeza do buffer mensagem_cliente:
        void limpar_buffer();

        // Get fd_cliente
        int get_fd_cliente(void);

        // Set fd_cliente
        void set_fd_cliente(int fd_cliente);

        // Set id
        void set_id(int id);

        // Get shutdown
        bool get_shutdown(void);

        // Set shutdown
        void set_shutdown(bool shutdown);

        // Get mensagem
        char* get_mensagem();

        // Get endereco_servidor
        sockaddr_in get_endereco_servidor(void);

        // Set endereco_servidor
        void set_endereco_servidor(sockaddr_in endereco);

        // Set nickname
        void set_nickname(string new_nick);

        string get_nickname();

        // Inicializa thread
        void start_thread();

        // TODO: Criação e inicialização da thread
        // TODO: Adaptação do loop while presente em Cliente.cpp em Oldfiles
};

#endif /*_CLIENT_H_*/