#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <../lib/Server.hpp>

using namespace std;

int opt = 1, sd, aux, use;
fd_set readfds;

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

    if(setsockopt(Server::fd_servidor, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){
        cerr << "Erro no set socket option" << endl;
        exit(-1);
    }

    // Configuração do endereço do servidor:
    Server::endereco_servidor.sin_family = AF_INET;
    Server::endereco_servidor.sin_port = htons(2000);
    Server::endereco_servidor.sin_addr.s_addr = INADDR_ANY;

    // Atribuição do endereço ao socket:
    if(bind(Server::fd_servidor, (sockaddr *) &(Server::endereco_servidor), sizeof(Server::endereco_servidor)) == -1){
        cerr << "Erro ao realizar bind do socket!\n";
        exit(-1);
    }else{
        cout << "Bind realizado com sucesso!\n";
    }

    // TODO : Definir quantos clientes colocar aqui a princípio, ou se há forma de alterar dinamicamente
    // Marcação do socket para a escuta de um único cliente:
    if(listen(Server::fd_servidor, 10) == -1){
        cerr << "Erro na marcação de escuta por clientes!\n";
        exit(-1);
    }else{
        cout << "Servidor escutando por requisições!\n";
    }

    // Setup da thread principal
    Server::self_thread = thread(&Server::self_thread_logic, this);
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
    Server::self_thread.join();
}

// Função privada com lógica da thread principal
void Server::self_thread_logic(){
    // Ids recebidos pelos usuários, atualizado a cada nova entrada no sistema
    int ids = 0;

    while(!Server::get_shutdown()){
        FD_ZERO(&readfds);
        FD_SET(Server::fd_servidor, &readfds);
        aux = Server::fd_servidor;

        for(uint i = 0; i < Server::clientes.size(); i++){
            sd = Server::clientes[i].first->get_fd_cliente();
            if(sd > 0)
                FD_SET(sd, &readfds);
            if(sd > aux)
                aux = sd;
        }

        use = select(aux + 1, &readfds, NULL, NULL, NULL);
        if((use < 0) && errno !=  EINTR){
            cout << "select error" << endl;
            exit(-1);
        }

        // Declaração de promise para averiguar novas conexões
        int res = 0;

        // Inicialização da variável que conterá o cliente atual (apenas temporário para teste):
        Client *curr_client = new Client("");
        curr_client->set_id(ids);

        if(FD_ISSET(Server::fd_servidor, &readfds)){
            // Setup da thread de comunicação
            Server::new_connection_thread = thread(&Server::connection_waiter_logic, this, curr_client, ref(res));

            // Inicialização da thread de comunicação
            Server::start_connection_waiter();
        }
        

        // Com base no retorno da promise
        if(res){
            // Criação e setup de thread de troca de mensagens entre curr_client e servidor
            thread comunicacao = thread(&Server::exchange_logic, this, curr_client);

            // Inclusão de novo usuário conectado e sua thread
            Server::clientes.push_back(make_pair(curr_client, move(comunicacao)));

            // Inicialização da thread
            //Server::clientes[int(Server::clientes.size()) - 1].second.join();

            // Atualização dos ids
            ids++;
            cout << "Porta dos conectados: \n";
            for(uint i = 0; i < Server::clientes.size(); i++){
                cout << Server::clientes[i].first->endereco_cliente.sin_port << endl;
            }
        }

        for(uint i = 0; i < Server::clientes.size(); i++){
            sd = Server::clientes[i].first->get_fd_cliente();
            if(FD_ISSET(sd, &readfds))
                Server::clientes[i].second.join();
        }
            
    }
}

// Função privada para inicialização da thread de aguardo de comunicação
void Server::start_connection_waiter(){
    Server::new_connection_thread.join();
}

// Função privada com lógica da espera de novas conexões
void Server::connection_waiter_logic(Client *curr_client, int &res){
    // Aceite de uma requisição:
    int client_fd = accept (
        this->get_fd_servidor(),
        (sockaddr*) &curr_client->endereco_cliente,
        &this->tamanho_endereco
    );

    // Verificação se o aceite foi válido:
    if (client_fd < 0) {
        cerr << "Não foi possível se conectar ao cliente!\n";
        res = 0;
    }else{
        cout << "Cliente conectado no IP: " << inet_ntoa(curr_client->endereco_cliente.sin_addr)
             << " e porta " << ntohs(curr_client->endereco_cliente.sin_port) << "\n";

        curr_client->set_fd_cliente(client_fd);
        res = 1;
    }
    return;
}

// Função privada com lógica para troca de mensagens
void Server::exchange_logic(Client *curr_client){
    // Recepção da(s) mensagem(s) do cliente (pois podem ser quebradas se forem maiores que LIMITE_MENSAGEM):
    while (true) {
        curr_client->limpar_buffer();

        // Recebimento de 1 bloco da mensagem do cliente:
        int recv_response = recv (
            curr_client->get_fd_cliente(),
            curr_client->get_mensagem(),
            LIMITE_MENSAGEM,
            MSG_NOSIGNAL
        );

        if (recv_response == -1) {
            cout << "Mensagem não recebida!\n";
            exit(-1);
        }

        char* mensagem = curr_client->get_mensagem();
        //string str(curr_client->get_mensagem());
        // Alocamento da mensagem e cliente remetente na fila
        //Server::messages_queue.push(make_pair(curr_client, str));

        if (strlen(mensagem) == 0)
            break;

        if (strcmp(mensagem, "/quit") == 0) {
            //this->set_shutdown(true);
            vector <pair <Client *, thread>>::iterator it;
            for(it = Server::clientes.begin(); it != Server::clientes.end(); it++){
                if(it->first->get_fd_cliente() == curr_client->get_fd_cliente())
                    Server::clientes.erase(it);
            }
            break;
        }

        if (strcmp(mensagem, "/ping") == 0) {
            cout << "Ping recebido\n";
            char pong[6] = {'P', 'o', 'n', 'g', '!', '\0'};
            this->set_mensagem(pong);
            send(
                curr_client->get_fd_cliente(),
                this->get_mensagem(),
                strlen(this->get_mensagem()) + 1,
                MSG_NOSIGNAL
            );
            break;
        }

        cout << mensagem << "\n";
        /*if(!Server::messages_queue.empty()){
            cout << "Nova mensagem de " << Server::messages_queue.front().first->get_nickname() << ":" << endl;
            cout << Server::messages_queue.front().second << endl;
            Server::messages_queue.pop();
        }*/
    }
}