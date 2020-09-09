/*
 * main.cpp
 *
 *  Created on: 11 de out de 2017
 *      Author: bruno
 */
#include <thread>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>
#include "garcom.h"
#include "bar.h"

std::mutex mut;
std::condition_variable cV;

int main(int argc, char *argv[])
{
	unsigned int n_clientes = 0;
	unsigned int n_garcom = 0;
	unsigned int n_rodadas = 0;
	unsigned int n_capacidade = 0;

	if(argc != 5)
	{
		n_clientes = 100;
		n_garcom = 20;
		n_capacidade = 5;
		n_rodadas=5;
	}
	else
	{
		n_clientes = std::stoi(argv[1]);
		n_garcom = std::stoi(argv[2]);
		n_capacidade = std::stoi(argv[3]);
		n_rodadas = std::stoi(argv[4]);
	}

	if(n_clientes <= 0 || n_garcom <= 0 || n_capacidade <= 0 || n_rodadas <= 0)
	{
		std::cout << " Parametros inválidos, o bar sera iniciado com valores padrões: " << std::endl;
		n_clientes = 20;
		n_garcom = 4;
		n_capacidade = 5;
		n_rodadas= 15;

	}

	Bar bar1(n_clientes, n_garcom, n_capacidade, n_rodadas);
	bar1.barInicio();



	return 0;
}

