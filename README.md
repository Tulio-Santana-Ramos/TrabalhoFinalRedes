# TrabalhoFinalRedes
Repositório destinado ao trabalho final da disciplina de Redes de Computadores 2022-01

## Integrantes:
- 11796531 &nbsp; Israel Felipe da Silva
- 11796893 &nbsp; Luiz Fernando Rabelo
- 11795526 &nbsp; Tulio Santana Ramos

### Confecção
No presente momento, para a primeira entrega parcial do projeto, foram desenvolvidas 2 aplicações representando ambos os lados, Servidor ([Servidor.cpp](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Servidor.cpp)) e Cliente ([Cliente.cpp](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Cliente.cpp)), de uma simples comunicação.
O projeto foi desenvolvido em sistemas Linux (Ubuntu 22.04 LTS) e Windows 11 em conjunto com WSL 2.0 (Ubuntu 22.04 LTS) através da linguagem C++ e suas bibliotecas padrões.

### Compilação
Para fácil compilação dos programas, utiliza-se o [Makefile](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Makefile) presente neste repositório.

### Execução
A execução de ambas as aplicações também fica por parte do [Makefile](https://github.com/Tulio-Santana-Ramos/TrabalhoFinalRedes/blob/main/Makefile) através das respectivas tags de cada uma. Uma vez que ambas as aplicações tenham sido iniciadas e a conexão seja estabelecida, pode-se trocar mensagens de até 4096 caracteres. Mnesagens com uma número maior de caracteres seram divididas e enviadas em mensagens distintas.