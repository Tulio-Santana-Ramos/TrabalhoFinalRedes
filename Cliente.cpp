#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

#define LIMITE_MENSAGEM 11

using namespace std;

int main(){
    int fd_cliente;
    sockaddr_in endereco_servidor;

    char mensagem[LIMITE_MENSAGEM];
    memset(mensagem, 0, LIMITE_MENSAGEM * sizeof(char));

    //criação do socket do servidor
	fd_cliente = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi devidamente criado:
    if (fd_cliente == -1) {
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\n";

    // Seta a porta e o IP para o mesmo endereço do servidor
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_port = htons(2000);
    endereco_servidor.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Verificação de conexão com o servidor
    if (connect(fd_cliente, (sockaddr *) &endereco_servidor, sizeof(endereco_servidor)) < 0) {
        cerr << "Erro ao conectar com o servidor!\n";
        exit(-1);
    }
    cout << "Conexão com servidor efetuada com sucesso\n";

    // Lê a mensagem do usuário
    string total;
    cout << "Digite uma mensagem: \n";
    getline(cin, total);

    int j = 0;
    for (int i = 0; i < total.size(); i++) {
        mensagem[j++] = total[i];
        if (i == total.size() - 1 || j == LIMITE_MENSAGEM - 1) {
            mensagem[j] = '\0';
            // Envia a mensagem ao servidor
            if (send(fd_cliente, mensagem, strlen(mensagem), 0) < 0) {
                cout << "Erro ao enviar a mensagem!\n";
                exit(-1);
            }
            j = 0;
        }
    }

    // Sinaliza final do envio
    mensagem[0] = '\0';

    if (send(fd_cliente, mensagem, strlen(mensagem), 0) < 0) {
        cout << "Erro ao enviar a mensagem!\n";
        exit(-1);
    }

    // Tratar aqui se receber mensagem de resposta do servidor

    close(fd_cliente);
    cout << "Socket fechado!\n";

    return 0;
}
