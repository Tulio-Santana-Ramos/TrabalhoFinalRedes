#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

#define LIMITE_MENSAGEM 11

using namespace std;


bool conectar_servidor(int fd_cliente, sockaddr_in *endereco_servidor) {
    // Configuração da porta e IP para o mesmo endereço do servidor:
    endereco_servidor->sin_family = AF_INET;
    endereco_servidor->sin_port = htons(2000);
    endereco_servidor->sin_addr.s_addr = inet_addr("127.0.0.1");
    // Retorno da conexão com o servidor:
    return connect(fd_cliente, (sockaddr *) endereco_servidor, sizeof(*endereco_servidor)) != -1;
}

bool mandar_mensagem_servidor(int fd_cliente, string mensagem_total, char* buffer_mensagem) {
    // Envio da mensagem em blocos de no máximo LIMITE_MENSAGEM:
    int j = 0;
    for (int i = 0; i < mensagem_total.size(); i++) {
        buffer_mensagem[j++] = mensagem_total[i];
        if (i == mensagem_total.size() - 1 || j == LIMITE_MENSAGEM - 1) {
            buffer_mensagem[j] = '\0';
            // Envia um bloco da mensagem ao servidor:
            if (send(fd_cliente, buffer_mensagem, strlen(buffer_mensagem), MSG_NOSIGNAL) == -1)
                return false;
            j = 0;
        }
    }
    // Sinaliza final do envio
    buffer_mensagem[0] = '\0';
    if (send(fd_cliente, buffer_mensagem, strlen(buffer_mensagem), MSG_NOSIGNAL) == -1)
        return false;
    return true;
}

int main() {

    int fd_cliente;
    sockaddr_in endereco_servidor;

    char mensagem[LIMITE_MENSAGEM];
    memset(mensagem, 0, LIMITE_MENSAGEM * sizeof(char));

    // Criação do socket do cliente:
    fd_cliente = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi devidamente criado:
    if (fd_cliente == -1) {
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\nPara conectar-se ao servior, digite /connect\n";

    bool shut_down = false;
    string entrada;

    while (!shut_down) {
        cout << "Digite sua mensagem ou comando: \n";
        getline(cin, entrada);
        if (entrada == "/connect") {
            // Conexão com o servidor:
            if (conectar_servidor(fd_cliente, &endereco_servidor))
                cout << "Conexão com servidor efetuada com sucesso\n";
            else
                cerr << "Erro ao conectar com o servidor!\n";
        } else {
            if (entrada == "/quit")
                shut_down = true;
            // Tentativa de envio da mensagem:
            if (mandar_mensagem_servidor(fd_cliente, entrada, mensagem))
                cout << "Mensagem enviada com sucesso!\n";
            else
                cerr << "Erro ao enviar a mensagem!\n";
        }
    }

    // Tratar aqui se receber mensagem de resposta do servidor

    close(fd_cliente);
    cout << "Socket fechado!\n";

    return 0;
}
