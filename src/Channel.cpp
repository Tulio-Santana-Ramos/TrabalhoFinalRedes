#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <../lib/Channel.hpp>

using namespace std;

// Construtor
Channel::Channel(string name, Client *adm){
    // Atribuição do administrador do canal
    Channel::admin = adm;

    // Atribuição do nome do canal
    Channel::name = name;

    // Inicialização do vetor de usuários
    Channel::usuarios.push_back(Channel::admin);
}