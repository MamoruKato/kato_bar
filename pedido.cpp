/*
 * pedido.cpp
 *
 *  Created on: 13 de out de 2017
 *      Author: bruno
 */

#include "pedido.h"

Pedido::Pedido(unsigned int npedido, unsigned int ncliente) {
	_pedido = npedido;
	_idCliente = ncliente;
	_idGarcom = 0;
}

Pedido::~Pedido() {
}

/*
 * Retorna a id do cliente que realizou o pedido
 */
unsigned int Pedido::cliente()
{
	return _idCliente;
}

/*
 * retorna a id do garcom que coletou o pedido
 */
unsigned int Pedido::garcom()
{
	return _idGarcom;
}

void Pedido::garcom(unsigned int id)
{
	_idGarcom = id;
}
/*
 * retorna o conteudo do pedido
 */
unsigned int Pedido::pedido()
{
	return _pedido;
}

