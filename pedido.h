/*
 * pedido.h
 *
 *  Created on: 13 de out de 2017
 *      Author: bruno
 */

#ifndef SRC_PEDIDO_H_
#define SRC_PEDIDO_H_

class Pedido {
public:
	Pedido(unsigned int npedido, unsigned int ncliente);
	virtual ~Pedido();
	unsigned int cliente();
	unsigned int garcom();
	void garcom(unsigned int id);
	unsigned int pedido();
private:
	unsigned int _idCliente;
	unsigned int _idGarcom;
	unsigned int _pedido;

};

#endif /* SRC_PEDIDO_H_ */
