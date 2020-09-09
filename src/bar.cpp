/*
 * bar.cpp
 *
 *  Created on: 12 de out de 2017
 *      Author: bruno
 */

#include "bar.h"

//mutex principal utilizado no programa
std::mutex Bar::_mut1;
std::condition_variable Bar::_cv1;
std::deque<std::unique_ptr<Cliente>> Bar::_barClientela;
std::deque<std::unique_ptr<Garcom>> Bar::_barGarcom;
unsigned int Bar::_pedidos = 0;
unsigned int Bar::_rodadas = 0;
unsigned int Bar::_clientes;
unsigned int Bar::_rodada = 0;
unsigned int Bar::_garcom;

std::deque<Pedido> Bar::_barPedidos;
std::deque<Pedido> Bar::_barProdutos;

Bar::Bar(unsigned int clientes, unsigned int garcons, unsigned int atende,
			unsigned int rodadas) {
	std::cout << "\n\n";
	std::cout << " ---------------- BEM VINDO AO KATO BAR ---------------- \n\n";
	std::cout << "                         -_-           \n\n";

	_clientes = clientes;
	_garcom = garcons;
	_atende = atende;
	_rodadas = rodadas;

	//Sorteia o desejo dos clientes
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> x(0,2);
	int dice_roll = x(g);

	 for(unsigned int i = 0; i < clientes; i++)
	 {
		 _barClientela.push_back(std::make_unique<Cliente>(i,dice_roll));
	 }

	 for(unsigned int i = 0; i < garcons; i++)
	 {
		 _barGarcom.push_back(std::make_unique<Garcom>(atende,i));
	 }

}

Bar::~Bar(){

	std::cout << "\n";
	std::cout<< " -------------- O Bar esta fechando, muito obrigado ----------------" << std::endl;
}

/*
 * Metodo que inicia o bar
 * Inicia as threads garcons e clientes do bar
 */
void Bar::barInicio()
{
	for(std::size_t i = 0; i < _garcom; i++)
	{
		_barGarcom[i]->run(_mut1,_cv1, _barPedidos,_barProdutos);
	}
	for(std::size_t i = 0; i < _clientes; i++)
	{
		_barClientela[i]->run(_mut1,_cv1, _barPedidos,_barProdutos,_rodadas);
	}

	{
		std::unique_lock<std::mutex> lm(_mut1);
		_cv1.wait(lm, [&]{return clientesSairam() && garcomSairam();});
	}

}


//Metodos Estaticos

/*
 * Checa se uma nova rodada sera lancada
 * caso uma nova rodada comece, as variaveis necessarias sao resetadas
 * e novos valores sao sorteados para os cliente, se estes vao pedir ou nao
 */

bool Bar::novaRodada()
{
	bool cond = false;
	if(garcomEsperando())
	{
		cond = true;
		if(_pedidos == 0 && Cliente::pediu() == 0)
		{

			for(size_t i = 0; i < _barClientela.size(); i++)
			{
				if(_barClientela[i]->estado() != (int)Cliente::Estados::Consumindo &&
						_barClientela[i]->estado() != (int)Cliente::Estados::Acabou)
				{
					_barClientela[i]->sorteiaPedido();
				}
			}
			for(size_t i = 0; i < _barClientela.size(); i++)
			{
				if(_barClientela[i]->pede()) Cliente::incrementa();
			}
			_pedidos = Cliente::pediu();
			if(_pedidos > 0 && _rodada < _rodadas)
			{
				std::cout << "\n\n------------------- Iniciando Rodada " << _rodada << " --------------------------- \n\n";
				_rodada++;
			}
		}
	}
	return cond;
}


void Bar::decrementaPedido()
{

	if(_pedidos > 0)
	{
		_pedidos--;
	}

}

/*
 * metodo para checar se todos os clientes realizaram seus pedidos
 */
bool Bar::todosPediram()
{
	bool valor = false ;

	if(Cliente::pediu() == 0)
	{
		valor = true;
	}
	return valor;
}

/*
 * Metodo para checar se todos os garcons estao esperando
 * caso estejam retorna true, caso contrario false
 */
bool Bar::garcomEsperando()
{
	bool valor = true;
	for(unsigned int i = 0; i < _barGarcom.size(); i++)
	{
		if(_barGarcom[i]->estado() != (int)Garcom::Estados::Esperando)
		{
			valor= false;
		}
	}
	return valor;
}

/*
 * Metodo para checar se há pedidos pendentes a serem atendidos
 */

bool Bar::pedidosPendentes()
{
	bool valor = false;
	if(!_barPedidos.empty()) valor = true;
	return valor;
}

/*
 * Checa se ainda ha clientes no bar
 * Caso haja, retorna true e false caso contrario
 */
bool Bar::clientesSairam()
{
	bool cond = true;

	if(!_barClientela.empty())
	{
		for(unsigned int i = 0; i < _barClientela.size(); i++)
		{
			if(_barClientela[i]->estado() != (int)(Cliente::Estados::Saiu)) cond = false;
		}
	}
	return cond;

}


/*
 *Metodo para esperar com que todos entrem
 */
bool Bar::todosEntraram()
{
	bool cond = false;
	if(_barClientela.size() == _clientes && _barGarcom.size() == _garcom) cond = true;
	return cond;
}

//Metodo para saber se todos os garcons sairam do bar
bool Bar::garcomSairam()
{
	bool cond = true;
	{
		for(unsigned int i = 0; i < _barGarcom.size(); i++)
		{
			if(_barGarcom[i]->estado() != (int)(Garcom::Estados::Saiu)) cond = false;
		}

	}
	return cond;
}

/*
 * Metodo para saber se todos os clientes acabaram e podem sair do bar
 */
bool Bar::clientesAcabaram()
{
	bool cond = true;
	{
		for(unsigned int i = 0; i < _barClientela.size(); i++)
		{
			if(_barClientela[i]->estado() != (int)(Cliente::Estados::Acabou) &&
				_barClientela[i]->estado() != (int)Cliente::Estados::Saiu) cond = false;
		}

	}
	return cond;

}

//metodo para retornar a rodada atual do bar
unsigned int Bar::rodada()
{
	return _rodada;
}
