#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Server.hpp>

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

int opt;
// int opt = 1, sd, aux, use;
// fd_set readfds;

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

    // Atribuição de opções alternativas (reuso de porta/endereço) ao nível de socket
    if(setsockopt(Server::fd_servidor, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&opt, sizeof(opt)) < 0){
        cerr << "Erro no set socket option" << endl;
        exit(-1);
    }

    // Atribuição do endereço ao socket:
    if(bind(Server::fd_servidor, (sockaddr *) &(Server::endereco_servidor), sizeof(Server::endereco_servidor)) == -1){
        cerr << "Erro ao realizar bind do socket!\n";
        exit(-1);
    }else{
        cout << "Bind realizado com sucesso!\n";
    }

    // Marcação do socket para a escuta de MAX_CLIENTS clientes:
    if(listen(Server::fd_servidor, MAX_CLIENTS) == -1){
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

// Função para inicialização da thread
void Server::start_thread(){
    // Setup da thread principal
    Server::self_thread = thread(&Server::self_thread_logic, this);
}

// Função para finalização da thread
void Server::join_thread(){
    Server::self_thread.join();
}

void Server::self_thread_logic(){

    while(!Server::get_shutdown()) {

        Client *curr_client = new Client("");
        curr_client->limpar_buffer();

        // Inicialização de um novo cliente:
        socklen_t tam_endereco = sizeof(curr_client->endereco_cliente);
        curr_client->set_fd_cliente(
            accept(Server::fd_servidor, (struct sockaddr*) &curr_client->endereco_cliente, &tamanho_endereco)
        );

        // Verificação se o aceite foi válido:
        if (curr_client->get_fd_cliente() < 0) {
            cerr << "Conexão de cliente não aceita!";
            continue;
        }

        cout << "Cliente conectado no IP: " << inet_ntoa(curr_client->endereco_cliente.sin_addr)
             << " e porta " << ntohs(curr_client->endereco_cliente.sin_port) << "\n";

        // Verificação se o limite de clientes conectados foi atingido:
        if(Server::clientes.size() == MAX_CLIENTS){
            printf("O numero maximo de usuarios foi atingido!");
            close(curr_client->get_fd_cliente());
            continue;
        }

        // Adição do cliente à lista de clientes:
        pthread_mutex_lock(&clients_mutex);
        Server::clientes.push_back(curr_client);
        pthread_mutex_unlock(&clients_mutex);

        // Gerenciamento da comunicação com o cliente:
        thread client_thread = thread(&Server::handle_client_logic, this, curr_client);
        // pthread_t handle_client_thread;
        // pthread_create(&handle_client_thread, NULL, (void *(*) (void *)) &Server::handle_client_logic, (void*) curr_client);
    }    
}


void Server::handle_client_logic(Client *client) {

    cout << client->get_fd_cliente();

    while (true) {

        // Recebimento da mensagem:
        int bytes_read = recv(
            client->get_fd_cliente(),
            client->get_mensagem(),
            LIMITE_MENSAGEM,
            0
        );

        // Envio da mensagem aos demais clientes:
        if (bytes_read > 0) {
            enviar_mensagem(client->get_mensagem());
        } else {
            cerr << "Erro! Nenhum byte foi lido do cliente!\n";
            break;
        }
    }

    // remover_cliente(client);
    pthread_detach(pthread_self());

}

void Server::enviar_mensagem(char* mensagem){

	pthread_mutex_lock(&clients_mutex);

    for (auto cliente : Server::clientes) {
        if (write(cliente->get_fd_cliente(), mensagem, strlen(mensagem)) < 0){
            cerr << "Erro ao enviar mensagem ao cliente!\n";
            break;
        }
    }

	pthread_mutex_unlock(&clients_mutex);
}


void Server::remover_cliente(Client *cliente) {

    uint i;

    for (i = 0; i < Server::clientes.size(); i++) {
        if (Server::clientes[i]->get_nickname() == cliente->get_nickname())
            break;
    }

    Server::clientes.erase(Server::clientes.begin() + 0);
}


// // Função privada com lógica da thread principal
// void Server::self_thread_logic(){

//     while(!Server::get_shutdown()){
//         FD_ZERO(&readfds);
//         FD_SET(Server::fd_servidor, &readfds);
//         aux = Server::fd_servidor;

//         for(uint i = 0; i < Server::clientes.size(); i++){
//             sd = Server::clientes[i]->get_fd_cliente();
//             if(sd > 0)
//                 FD_SET(sd, &readfds);
//             if(sd > aux)
//                 aux = sd;
//         }

//         use = select(aux + 1, &readfds, NULL, NULL, NULL);
//         if((use < 0) && errno !=  EINTR){
//             cout << "select error" << endl;
//             exit(-1);
//         }

//         // Declaração de promise para averiguar novas conexões
//         int res = 0;

//         // Inicialização da variável que conterá o cliente atual (apenas temporário para teste):
//         Client *curr_client = new Client("");

//         if(FD_ISSET(Server::fd_servidor, &readfds)){
//             // Setup da thread de comunicação
//             Server::new_connection_thread = thread(&Server::connection_waiter_logic, this, curr_client, ref(res));

//             // Inicialização da thread de comunicação
//             Server::start_connection_waiter();
//         }
        

//         // Com base no retorno da promise
//         if(res){
//             // Criação e setup de thread de troca de mensagens entre curr_client e servidor
//             //thread comunicacao = thread(&Server::exchange_logic, this, curr_client);

//             // Inclusão de novo usuário conectado e sua thread
//             Server::clientes.push_back(curr_client);

//             // Inicialização da thread
//             //Server::clientes[int(Server::clientes.size()) - 1].second.join();

//         }

//         for(uint i = 0; i < Server::clientes.size(); i++){
//             sd = Server::clientes[i]->get_fd_cliente();
//             if(FD_ISSET(sd, &readfds)){
//                 //Server::clientes[i].second.join();
//                 Server::clientes[i]->limpar_buffer();

//                 // Recebimento de 1 bloco da mensagem do cliente:
//                 int recv_response = recv (
//                     Server::clientes[i]->get_fd_cliente(),
//                     Server::clientes[i]->get_mensagem(),
//                     LIMITE_MENSAGEM,
//                     MSG_NOSIGNAL
//                 );

//                 if (recv_response == -1) {
//                     cout << "Mensagem não recebida!\n";
//                     exit(-1);
//                 }

//                 char* mensagem = Server::clientes[i]->get_mensagem();
//                 //string str(curr_client->get_mensagem());
//                 // Alocamento da mensagem e cliente remetente na fila
//                 //Server::messages_queue.push(make_pair(curr_client, str));

//                 //if (strlen(mensagem) == 0)
//                     //break;

//                 if (strcmp(mensagem, "/quit") == 0) {
//                     //this->set_shutdown(true);
//                     vector <Client *>::iterator it;
//                     for(it = Server::clientes.begin(); it != Server::clientes.end(); it++){
//                         if((*it)->get_fd_cliente() == Server::clientes[i]->get_fd_cliente())
//                             //Server::clientes.erase(it);
//                             (*it)->set_fd_cliente(0);
//                     }
//                     //break;
//                 }

//                 if (strcmp(mensagem, "/ping") == 0) {
//                     cout << "Ping recebido\n";
//                     char pong[6] = {'P', 'o', 'n', 'g', '!', '\0'};
//                     this->set_mensagem(pong);
//                     send(
//                         Server::clientes[i]->get_fd_cliente(),
//                         this->get_mensagem(),
//                         strlen(this->get_mensagem()) + 1,
//                         MSG_NOSIGNAL
//                     );
//                     //break;
//                 }

//                 cout << mensagem << "\n";
//             }
//         }
            
//     }
// }

// // Função privada para inicialização da thread de aguardo de comunicação
// void Server::start_connection_waiter(){
//     Server::new_connection_thread.join();
// }

// // Função privada com lógica da espera de novas conexões
// void Server::connection_waiter_logic(Client *curr_client, int &res){
//     // Aceite de uma requisição:
//     int client_fd = accept (
//         this->get_fd_servidor(),
//         (sockaddr*) &curr_client->endereco_cliente,
//         &this->tamanho_endereco
//     );

//     // Verificação se o aceite foi válido:
//     if (client_fd < 0) {
//         cerr << "Não foi possível se conectar ao cliente!\n";
//         res = 0;
//     }else{
//         cout << "Cliente conectado no IP: " << inet_ntoa(curr_client->endereco_cliente.sin_addr)
//              << " e porta " << ntohs(curr_client->endereco_cliente.sin_port) << "\n";

//         curr_client->set_fd_cliente(client_fd);
//         res = 1;
//     }
//     return;
// }
