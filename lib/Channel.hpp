#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>

// Classe do Canal disponível no Servidor da aplicação
class Channel{
    private:
        // Nome do Canal
        string name;

        // Administrador do Canal
        Client *admin;

        // Lista de Clientes neste Canal
        vector <Client *> usuarios;

    public:
        // Construtor
        Channel(string name, Client *adm);
};

#endif /*_CHANNEL_H_*/