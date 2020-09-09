# Controle e simulaçoes de rodadas gratis de um Bar: Bar das threads:
  O trabalho em questão tem como objetivo explorar o uso de theads e seus mecanismos de
  sincronização e controle de acesso a regiões críticas.
  O objetivo do trabalho em questão foi a implementação utilizando threads e
  ferramentas auxiliares ao multithreading ( como mutexes e variáveis de condição),
  de um bar na linguagem de programação C++ .
  Há neste uma determinada quantidade de clientes que deverão ser servidos,
  assim como uma quantidade de garçons, capacidade máxima de atendimento de
  cada garçom e número total de rodadas que serão servidas. Adicionalmente foi
  utilizada uma nova variável que determina a taxa de pedidos de um cliente, se este
  irá pedir ou não na rodada.
  Optou-se pela programação orientada a objetos devido a uma maior clareza
  e facilidade de entendimento do código. Criou-se classes para representar cada um
  dos seguintes componentes, bar, garçons, clientes e pedidos, onde cada uma
  possui suas determinadas características e funções.
	
1. Como utilizar:
		Para executar o código siga os seguintes passos:
		1.1 - Abra no terminal o diretório onde se encontra o Makefile
		1.2 - make
		Para utilizar argumentos padrões do bar:
		1.3 - ./exec
		Para passar argumentos ao código:
		1.3 - ./exec 20 4 5 8 , onde os parâmetros representam,  a quantidade de clientes que entrarão no bar, quantidade de garçons no bar,
		capacidade de atendimento que cada garçom possuirá e o numero de rodadas que o bar irá liberar respectivamente.

