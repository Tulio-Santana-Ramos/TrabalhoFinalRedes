#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <../lib/Client.hpp>

using namespace std;

// Função auxiliar para fechar aplicação através do CTRL + C
void Client::ctrlc_handler(int s) {
    Client::entrada = "/quit";
}

// Função auxiliar para divisão de strings
string split(string str, char delim){
    int i = 0, start = 0, end = int(str.size()) - 1;

    while(i++ < int(str.size())){
        if(str[i] == delim)
            start = i + 1;
    }
    string new_str = "";
    new_str.append(str, start, end - start + 1);

    return new_str;
}

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

    // Inicialização do fd do cliente:
    Client::fd_cliente = -1;
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

// Função para setar nickname do Cliente
void Client::set_nickname(string new_nick){
    Client::nickname = new_nick;
}

// Função para setar entrada do Cliente
void Client::set_entrada(string entrada){
    Client::entrada = entrada;
}

// Função para obter nickname do Cliente
string Client::get_nickname(){
    return Client::nickname;
}

// Função para inicialização da thread
void Client::start_thread(){
    // Criação do socket do Cliente
    Client::fd_cliente = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi criado
    if(Client::fd_cliente == -1){
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\n";

    // Setup da thread
    Client::self_thread = thread(&Client::thread_logic, this);
}

// Função para finalização da thread
void Client::join_thread(){
    Client::self_thread.join();
}

// Função privada com lógica da thread principal
void Client::thread_logic(){

    entrada = "";

    while (entrada != "/connect") {
        cout << "Digite /connect para conectar-se ao servidor!\n";
        cin >> entrada; getchar();
    }

    if(this->conectar_servidor(&endereco_servidor))
        cout << "Conexão com servidor efetuada com sucesso\n";
    else {
        cerr << "Erro ao conectar com o servidor!\n";
        return;
    }
    
    thread input_thread = thread(&Client::recv_thread, this);
    thread output_thread = thread(&Client::send_thread, this);

    while (!this->get_shutdown()) {
        if (entrada == "/quit")
            this->set_shutdown(true);
        sleep(1);
    }

    input_thread.detach();
    output_thread.detach();
}

// Função privada com a lógica da thread de recepção de mensagens
void Client::recv_thread(){
    while (true) {
        char mensagem_servidor[LIMITE_MENSAGEM];
        int recv_response = recv(
            this->get_fd_cliente(),
            mensagem_servidor,
            sizeof(mensagem_servidor),
            MSG_NOSIGNAL
        );
        if (recv_response != -1)
            cout << "Servidor respondeu: " << mensagem_servidor << "\n";
    }
}

// Função privada com a lógica da thread de envio de mensagens
void Client::send_thread(){
    string nick = "nickname";
    while (true) {
        cout << "Digite sua mensagem ou comando: \n";
        getline(cin, Client::entrada);
        if (Client::entrada == "/quit")
            return;
        if(entrada.find(nick) != string::npos)
            this->set_nickname(split(entrada, ' '));
        if (this->mandar_mensagem_servidor(Client::entrada))
            cout << "Mensagem enviada com sucesso!\n";
        else
            cerr << "Erro ao enviar a mensagem!\n";
    }
}
