#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

#define LIMITE_MENSAGEM 4097
#define MAX_CLIENTS 10

using namespace std;
struct Client{
    struct sockaddr_in address;
    int fd_cliente;
    char nickname[50];
    bool adm;
    bool muted;
    bool shutdown;
};

struct Channel{
    string name;
    int fd_admin;
    vector <Client *> usuarios;
};

vector <Client *> conectados;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

void queue_remove(int fd){
    pthread_mutex_lock(&clients_mutex);

    for(uint i = 0; i < conectados.size(); i++){
        if(conectados[i]->fd_cliente == fd){
            conectados.erase(conectados.begin() + i);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_message(char mensagem[], int fd){
    pthread_mutex_lock(&clients_mutex);

    for(uint i = 0; i < conectados.size(); i++){
        if(conectados[i]->fd_cliente != fd){
            if(write(conectados[i]->fd_cliente, mensagem, strlen(mensagem)) < 0){
                perror("Erro no wirte");
                break;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg){
    char mensagem[LIMITE_MENSAGEM];
    char name[50];
    int leave_flag = 0;

    Client *cliente = (Client *)arg;

    if(recv(cliente->fd_cliente, name, 50, 0) <= 0 || strlen(name) < 2 || strlen(name) >= 50 - 1){
        printf("N inseriu nome");
        leave_flag = 1;
    }else{
        strcpy(cliente->nickname, name);
        sprintf(mensagem, "%s entrou\n", cliente->nickname);
        printf("%s\n", mensagem);
        send_message(mensagem, cliente->fd_cliente);
    }

    bzero(mensagem, LIMITE_MENSAGEM);

    while(!leave_flag){
        int receive = recv(cliente->fd_cliente, mensagem, LIMITE_MENSAGEM, 0);
        if(receive > 0){
            if(strlen(mensagem) > 0){
                char buffer[LIMITE_MENSAGEM]; 
                strcpy(buffer, cliente->nickname);
                strcat(buffer, ": ");
                strcat(buffer, mensagem);
                send_message(buffer, cliente->fd_cliente);
                cout << buffer << "\n";
            }
        }else if(receive == 0 || strcmp(mensagem, "/quit") == 0){
            sprintf(mensagem, "%s has left\n", cliente->nickname);
            cout << mensagem << endl;
            send_message(mensagem, cliente->fd_cliente);
            leave_flag = 1;
        }else{
            cout << "Erro -1" << endl;
            leave_flag = 1;
        }
        bzero(mensagem, LIMITE_MENSAGEM);
    }
    close(cliente->fd_cliente);
    queue_remove(cliente->fd_cliente);
    //free(cliente);
    pthread_detach(pthread_self());

    return NULL;
}

int main(void) {

    int fd_servidor;  //? fd_cliente será um array?
    int option = 1;
    sockaddr_in endereco_servidor, endereco_cliente;
    pthread_t tid;

    // Criação do socket do servidor:
    fd_servidor = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi devidamente criado:
    if (fd_servidor == -1) {
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\n";

    // Configuração do endereço do servidor:
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_port = htons(2000);
    endereco_servidor.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Handler 
    signal(SIGPIPE, SIG_IGN);

    if(setsockopt(fd_servidor, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
        return -1;
	}

    // Atribuição do endereço ao socket:
    if (bind(fd_servidor, (sockaddr *) &endereco_servidor, sizeof(endereco_servidor)) == -1) {
        cerr << "Erro ao realizar bind do socket!\n";
        exit(-1);
    } else {
        cout << "Bind realizado com sucesso!\n";
    }

    // Marcação do socket para a escuta de um único cliente:
    if (listen(fd_servidor, MAX_CLIENTS) == -1) {
        cerr << "Erro na marcação de escuta por clientes!\n";
        exit(-1);
    } else {
        cout << "Servidor escutando por requisições!\n";
    }

    // Inicialização de variável de controle de estado do servidor:
    /*bool shut_down = false;

    // Aceite de uma requisição:
    tamanho_endereco = sizeof(endereco_cliente);
    fd_cliente = accept(fd_servidor, (sockaddr*) &endereco_cliente, &tamanho_endereco);

    // Verificação se o aceite foi válido:
    if (fd_cliente < 0){
        cerr << "Não foi possível se conectar ao cliente!\n";
        exit(-1);
    } 
    cout << "Cliente conectado no IP: " << inet_ntoa(endereco_cliente.sin_addr) << " e porta " << ntohs(endereco_cliente.sin_port) << "\n";

    while(!shut_down) {

        // Recepção da(s) mensagem(s) do cliente (pois podem ser quebradas se forem maiores que LIMITE_MENSAGEM):
        while (true) {
            bzero(mensagem_cliente, LIMITE_MENSAGEM);

            // Recebimento de 1 bloco da mensagem do cliente:
            if (recv(fd_cliente, mensagem_cliente, sizeof(mensagem_cliente), 0) == -1) {
                cout << "Mensagem não recebida!\n";
                exit(-1);
            }

            if (strlen(mensagem_cliente) == 0)
                break;

            if (strcmp(mensagem_cliente, "/quit") == 0) {
                shut_down = true;
                break;
            }

            if (strcmp(mensagem_cliente, "/ping") == 0) {
                cout << "Ping recebido\n";
                strcpy(mensagem_servidor, "pong!");
                send(fd_cliente, mensagem_servidor, strlen(mensagem_servidor) + 1, 0);
                break;
            } 

            cout << mensagem_cliente << "\n";
        }
    }

    // Fechamento do socket:
    close(fd_servidor);
    cout << "Socket fechado!\n";*/
    int connfd = 0;
    while(1){
        socklen_t clilen = sizeof(endereco_cliente);
        connfd = accept(fd_servidor, (struct sockaddr*)&endereco_cliente, &clilen);
    
        if(int(conectados.size()) == MAX_CLIENTS){
            printf("Max clients reched.\n");
            close(connfd);
            continue;
        }

        Client *cliente = (Client *)malloc(sizeof(Client));
        cliente->address = endereco_cliente;
        cliente->fd_cliente = connfd;
        pthread_mutex_lock(&clients_mutex);
        conectados.push_back(cliente);
        pthread_mutex_unlock(&clients_mutex);
        pthread_create(&tid, NULL, &handle_client, (void *)cliente);
        sleep(1);
    }

    return 0;
}


// #include <iostream>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <bits/stdc++.h>

// #define LIMITE_MENSAGEM 4097

// using namespace std;


// int main(void) {

//     int fd_servidor, fd_cliente;  //? fd_cliente será um array?
//     sockaddr_in endereco_servidor, endereco_cliente;
//     socklen_t tamanho_endereco;

//     // Criação de buffer para as mensagens:
//     char mensagem_cliente[LIMITE_MENSAGEM], mensagem_servidor[LIMITE_MENSAGEM];

//     // Criação do socket do servidor:
//     fd_servidor = socket(AF_INET, SOCK_STREAM, 0);

//     // Verificação se o socket foi devidamente criado:
//     if (fd_servidor == -1) {
//         cerr << "Erro ao criar o socket!\n";
//         exit(-1);
//     }
//     cout << "Socket criado com sucesso!\n";

//     // Configuração do endereço do servidor:
//     endereco_servidor.sin_family = AF_INET;
//     endereco_servidor.sin_port = htons(2000);
//     endereco_servidor.sin_addr.s_addr = INADDR_ANY;

//     // Atribuição do endereço ao socket:
//     if (bind(fd_servidor, (sockaddr *) &endereco_servidor, sizeof(endereco_servidor)) == -1) {
//         cerr << "Erro ao realizar bind do socket!\n";
//         exit(-1);
//     } else {
//         cout << "Bind realizado com sucesso!\n";
//     }

//     // Marcação do socket para a escuta de um único cliente:
//     if (listen(fd_servidor, 1) == -1) {
//         cerr << "Erro na marcação de escuta por clientes!\n";
//         exit(-1);
//     } else {
//         cout << "Servidor escutando por requisições!\n";
//     }

//     // Inicialização de variável de controle de estado do servidor:
//     bool shut_down = false;

//     // Aceite de uma requisição:
//     tamanho_endereco = sizeof(endereco_cliente);
//     fd_cliente = accept(fd_servidor, (sockaddr*) &endereco_cliente, &tamanho_endereco);

//     // Verificação se o aceite foi válido:
//     if (fd_cliente < 0){
//         cerr << "Não foi possível se conectar ao cliente!\n";
//         exit(-1);
//     } 
//     cout << "Cliente conectado no IP: " << inet_ntoa(endereco_cliente.sin_addr) << " e porta " << ntohs(endereco_cliente.sin_port) << "\n";

//     while(!shut_down) {

//         // Recepção da(s) mensagem(s) do cliente (pois podem ser quebradas se forem maiores que LIMITE_MENSAGEM):
//         while (true) {
//             bzero(mensagem_cliente, LIMITE_MENSAGEM);

//             // Recebimento de 1 bloco da mensagem do cliente:
//             if (recv(fd_cliente, mensagem_cliente, sizeof(mensagem_cliente), 0) == -1) {
//                 cout << "Mensagem não recebida!\n";
//                 exit(-1);
//             }

//             if (strlen(mensagem_cliente) == 0)
//                 break;

//             if (strcmp(mensagem_cliente, "/quit") == 0) {
//                 shut_down = true;
//                 break;
//             }

//             if (strcmp(mensagem_cliente, "/ping") == 0) {
//                 cout << "Ping recebido\n";
//                 strcpy(mensagem_servidor, "pong!");
//                 send(fd_cliente, mensagem_servidor, strlen(mensagem_servidor) + 1, 0);
//                 break;
//             } 

//             cout << mensagem_cliente << "\n";
//         }
//     }

//     // Fechamento do socket:
//     close(fd_servidor);
//     cout << "Socket fechado!\n";

//     return 0;
// }
