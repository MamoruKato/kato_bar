/*
 * bar.h
 *
 *  Created on: 12 de out de 2017
 *      Author: bruno
 */

#ifndef SRC_BAR_H_
#define SRC_BAR_H_

#include <iostream>
#include <thread>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <memory>
#include "pedido.h"
#include "garcom.h"
#include "cliente.h"

class Bar {
public:
	Bar(unsigned int, unsigned int, unsigned int, unsigned int);
	virtual ~Bar();
	void barInicio();

	//Metodos estaticos
	static bool novaRodada();
	static void incrementaPedido();
	static void decrementaPedido();
	static bool todosPediram();
	static bool garcomEsperando();
	static bool pedidosPendentes();
	static bool clientesSairam();
	static bool todosEntraram();
	static bool garcomSairam();
	static bool clientesAcabaram();
	static unsigned int rodada();
private:
	//capacidade que o garcom possui de atender os clientes
	unsigned int _atende;


	//Variaveis estaticas
	static unsigned int _garcom;
	static unsigned int _clientes;
	static unsigned int _rodada;
	static unsigned int _rodadas;
	static unsigned int _pedidos;
	//lista de todos os clientes do bar
	static std::deque<std::unique_ptr<Cliente>> _barClientela;
	//lista de todos os garcons que trabalham no bar
	static std::deque<std::unique_ptr<Garcom>>_barGarcom;

	static std::mutex _mutTodos;
	static std::mutex _mut1;
	static std::condition_variable _cv1;
	static std::deque<Pedido> _barProdutos;
	//lista em que serao colocados os pedidos dos clientes
	static std::deque<Pedido> _barPedidos;



};

#endif /* SRC_BAR_H_ */
