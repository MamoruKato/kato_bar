/*
 * garcom.h
 *
 *  Created on: 11 de out de 2017
 *      Author: bruno
 */

#ifndef SRC_GARCOM_H_
#define SRC_GARCOM_H_

#include <iostream>
#include <deque>
#include <mutex>
#include <string>
#include <vector>
#include <condition_variable>
#include "pedido.h"
#include "cliente.h"

class Garcom {
public:
	//Quantidade de clientes que foram atendidos no total
	Garcom(unsigned int cAtender = 1, unsigned int id = 0);
	virtual ~Garcom();

	void run(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
			std::deque<Pedido> &barProdutos);
	void comeca(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barProdutos);

	void runGarcom(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
					std::deque<Pedido> &barProdutos);

	void imprimeEstado();
	int estado();

	enum class Estados{Esperando,Produzindo,Coletando,Entregando,Saiu};

private:

	void pegaMaxPedidos(std::mutex &mut1, std::condition_variable &cv1, std::deque<Pedido> &barPedidos);
	//Vai produzir as bebidas atendidas apos atender os C clientes
	void produzPedidos(std::mutex &mut1, std::condition_variable &cv1,std::deque<Pedido> &barProdutos);

	bool bufferMaior(std::mutex &_mut1,std::condition_variable &_cv1,
						std::deque<Pedido> &barPedido);

	std::vector<std::string> _estados;


	//Quantidade de clientes que o garcom pode atender (C)
	unsigned int _cAtender;

	//Quantidade de clientes que esta consumindo e nao pediu na rodada
	unsigned int _estado;
	//buffer de pedidos do garcom
	std::deque<Pedido> bufferPedidos;
	//Identificacao do garcom
	unsigned int _id;

	std::thread t1;

};

#endif /* SRC_GARCOM_H_ */
