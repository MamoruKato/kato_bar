/*
 * cliente.h
 *
 *  Created on: 11 de out de 2017
 *      Author: bruno
 */

#ifndef SRC_CLIENTE_H_
#define SRC_CLIENTE_H_

#include <deque>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include "pedido.h"
#include "garcom.h"

class Cliente {
public:
	Cliente(unsigned int id, unsigned int n_desejo = 1);
	virtual ~Cliente();

	void run(std::mutex &mut1, std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
			std::deque<Pedido> &barProdutos,unsigned int &barRodadas);

	unsigned int consumiu();
	int estado();
	void imprimeEstado();
	void sorteiaPedido();
	bool pede();

	//metodos estaticos
	static unsigned int pediu();
	//metodo para incrementar a variavel pediu
	static void incrementa();
	//metodo para decrementar a variavel pediu
	static void decrementa();


	 // Enumera os desejo que o cliente pode possuir
	enum class Desejo{Passar_Tempo,Encher_a_cara,Dia_de_Maldade};
	//Estados que o cliente pode estar
	enum class Estados{Esperando,Consumindo, Pedindo, Aguardando,Acabou,Saiu};

private:
	void comeca(std::mutex &mut1, std::condition_variable &cv1);
	void runCliente(std::mutex &mut1, std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
						std::deque<Pedido> &barProdutos,unsigned int &barRodadas);
	void realizaPedido(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barPedidos);
	void esperaPedido(std::mutex &mut1, std::condition_variable &cv1,std::deque<Pedido> &barProdutos);
	void consomePedido(std::mutex &mut1, std::condition_variable &cv1,
						std::deque<Pedido> &barProdutos, unsigned int &barRodadas);
	void sair(std::mutex &mut1, std::condition_variable &cv1);


	unsigned int _estado;
	unsigned int _desejo;
	unsigned int _id;
	unsigned int _consumiu;
	bool _pede;
	std::vector<std::string> _estados;
	std::thread t1;

	static unsigned int _pediu;
};

#endif /* SRC_CLIENTE_H_ */
