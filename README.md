# Trabalho Final de Redes de Computadores
Repositório destinado ao trabalho final da disciplina de Redes de Computadores 2022-01
Neste primeiro momento (07/06/2022), o código satisfaz as condições da primeira entrega parcial.

## Integrantes:
- 11796531 &nbsp; Israel Felipe da Silva
- 11796893 &nbsp; Luiz Fernando Rabelo
- 11795526 &nbsp; Tulio Santana Ramos

### Confecção
No presente momento, para a primeira entrega parcial do projeto, foram desenvolvidas 2 aplicações representando ambos os lados, Servidor ([Servidor.cpp](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Servidor.cpp)) e Cliente ([Cliente.cpp](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Cliente.cpp)), de uma simples comunicação.
O projeto foi desenvolvido em sistemas Linux (Ubuntu 22.04 LTS) e Windows 11 em conjunto com WSL 2.0 (Ubuntu 22.04 LTS) através da linguagem C++ e suas bibliotecas padrões.

### Compilação
Para fácil compilação dos programas, utiliza-se o [Makefile](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Makefile) presente neste repositório. A diretiva `make` compila tanto o código do cliente quanto o código do servidor.

### Execução
A execução de ambas as aplicações também fica por parte do [Makefile](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Makefile) através das respectivas tags de cada uma (`make srun` executa o servidor e `make crun` executa o cliente). Uma vez que ambas as aplicações tenham sido iniciadas e a conexão seja estabelecida, pode-se trocar mensagens de até 4096 caracteres. Mensagens com um número maior de caracteres serão divididas e enviadas em mensagens distintas.


### Comandos
A aplicação, para esta primeira entrega do trabalho, conta com os comandos de '/send', '/quit', '/connect' e '/ping' por parte do Cliente. 
- /connect tenta efetuar a conexão entre o cliente e o servidor configurado. Pode ser efetuado na mesma máquina ou em máquinas diferentes na mesma rede (alterando o IP do servidor dentro do arquivo de Cliente)
- /send permite ao cliente enviar mensagens ao servidor. Caso a mensagem ultrapasse os 4096 caracteres, ela será particionada em diferentes mensagens para satisfazer o limite e não haver perda de informações.
- /ping é utilizado para verificar o envio de mensagens por ambas as aplicações, fazendo com que, ao receber este comando, o Servidor envie uma nova mensagem ('pong!') para o Cliente conectado.
- /quit fecha o socket da conexão, finalizando portanto a execução de ambas as aplicações.
