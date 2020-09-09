/*
 * garcom.cpp
 *
 *  Created on: 11 de out de 2017
 *      Author: bruno
 */

#include "garcom.h"
#include "bar.h"


Garcom::Garcom(unsigned int cAtender,unsigned int id):t1(){
	_estado = int(Garcom::Estados::Esperando);
	_cAtender = cAtender;
	_id = id;
	_estados = {"esperando","produzindo", "coletando","entregando","saiu"};

	std::cout << "Garcom: " << _id << " iniciando o trabalho" << std::endl;

}

Garcom::~Garcom() {
	t1.join();
	std::cout<< "Garcom: " << _id << " parou de trabalhar" << std::endl;

}

/*
 * Metodo que inicializa a tread do garcom e a funcao que este vai executar
 */

void Garcom::run(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
		std::deque<Pedido> &barProdutos)
{
	t1 = std::move(std::thread(&Garcom::runGarcom,this,std::ref(mut1),std::ref(cv1),
					std::ref(barPedidos),std::ref(barProdutos)));


	//espera com que todos entrem
	{
		std::unique_lock<std::mutex> lm(mut1);
		cv1.wait(lm,[&]{return Bar::todosEntraram();});
	}
	cv1.notify_all();
}
/*
 * Metodo que ira realizar o papel de barreira
 * Espera até que todos os clientes tenham pedido
 */
void Garcom::comeca(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barProdutos)
{
	{
		std::unique_lock<std::mutex> lm(mut1);
		_estado = int(Garcom::Estados::Esperando);
		//Barreira para os garcons
		cv1.wait(lm,[&]{ return Bar::todosPediram() || Bar::clientesSairam();});
	}
	cv1.notify_all();
}


/*
 *Rotina do garcom, este continuara executando até que todos os clientes tenham
 *saido do bar
 */
void Garcom::runGarcom(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
						std::deque<Pedido> &barProdutos)
{
	while(!Bar::clientesSairam())
	{
		{
			comeca(mut1,cv1,barProdutos);
			{
				std::unique_lock<std::mutex> lm(mut1);
				while(Bar::pedidosPendentes())
				{
					pegaMaxPedidos(mut1,cv1,barPedidos);
					produzPedidos(mut1,cv1, barProdutos);
				}

			}
			cv1.notify_all();
		}
	}
	//Espera o que todos os clientes saiam
	{
		std::unique_lock<std::mutex> lm(mut1);
		cv1.wait(lm, [&]{return Bar::clientesSairam();});
		_estado = (int)Garcom::Estados::Saiu;
	}
	cv1.notify_all();
}

//metodos privados

//Pega todos os pedidos possiveis
void Garcom::pegaMaxPedidos(std::mutex &mut1, std::condition_variable &cv1, std::deque<Pedido> &barPedidos)
{

		if(! Bar::clientesSairam())
		{
			_estado = int(Garcom::Estados::Coletando);
			imprimeEstado();
			
			std::cout << "Garcom: " << _id <<" pega pedidos: ";

			while(bufferPedidos.size() < _cAtender && !barPedidos.empty())
			{
				std::cout<< barPedidos.front().cliente() << " ";
				barPedidos.front().garcom(_id);
				bufferPedidos.push_back(barPedidos.front());
				barPedidos.pop_front();
			}
			std::cout << "\n\n";
		}
}

/*
 * metodo para produzir os pedidos apos estes serem coletados:
 * Espera que todos os pedidos do garcom sejam coletados
 * Enquanto houver pedidos no buffer de pedidos do garcom este produz o pedido
 * Coloca o pedido no buffer de produtos do bar e retira do buffer de pedidos do garcom
 */
void Garcom::produzPedidos(std::mutex &mut1,std::condition_variable &cv1,std::deque<Pedido> &barProdutos)
{
	{
		//Entra na regiao crítica
		//Se a quantidade de pedidos no buffer for maior que 0

		if(! Bar::clientesSairam())
		{

			_estado = int(Garcom::Estados::Produzindo);
			imprimeEstado();

			std::cout << "Garcom: " << _id << " produz pedidos: ";

			while(!bufferPedidos.empty())
			{
				std::cout << bufferPedidos.front().cliente() << " ";
				//produz o pedido
				barProdutos.push_back(bufferPedidos.front());
				//retira o pedido do buffer de pedidos
				bufferPedidos.pop_front();
			}

			_estado = int(Garcom::Estados::Entregando);
			std::cout<< "\n\n";
		}
	}

}

void Garcom::imprimeEstado()
{
	std::cout << "Estado Garcom " << _id << " :";
	std::cout << _estados[_estado];
	std::cout << "\n";
}

//retorna o estado atual do garcom
int Garcom::estado()
{
	return _estado;
}
