/*
 * cliente.cpp
 *
 *  Created on: 11 de out de 2017
 *      Author: bruno
 */

#include "cliente.h"
#include "bar.h"


unsigned int Cliente::_pediu = 0;

Cliente::Cliente(unsigned int id, unsigned int n_desejo)
{
	_estado = int(Cliente::Estados::Aguardando);
	_estados = {"Esperando","Consumindo","Pedindo","Aguardando nova rodada","Acabou","Saiu"};
	_consumiu = 0;
	_id = id;
	if(_desejo != (int)Cliente::Desejo::Encher_a_cara ||
			_desejo != (int)Cliente::Desejo::Passar_Tempo ||
			_desejo != (int)Cliente::Desejo::Dia_de_Maldade)
	{
		_desejo = 1;
	}
	else
	{
		_desejo = n_desejo;
	}
	sorteiaPedido();
}

Cliente::~Cliente()
{
	t1.join();
}


/*
 * Metodo que inicializa a thread do cliente e funcao que este vai executar
 */
void Cliente::run(std::mutex &mut1, std::condition_variable &cv1,std::deque<Pedido> &barPedidos,
						std::deque<Pedido> &barProdutos,unsigned int &barRodadas)
{
	t1 = std::move(std::thread(&Cliente::runCliente,this,std::ref(mut1),std::ref(cv1),
			std::ref(barPedidos),std::ref(barProdutos),std::ref(barRodadas)));


	//espera com que todos entrem
	{
		std::unique_lock<std::mutex> lm(mut1);
		cv1.wait(lm,[&]{return Bar::todosEntraram();});
	}
	cv1.notify_all();
}

/*
 * Metodo que retorna a quantidade consumida pelo cliente
 */
unsigned int Cliente::consumiu()
{
	return _consumiu;
}

/*
 * Metodo que imprime o estado atual do cliente
 */
void Cliente::imprimeEstado()
{
	std::cout << "Cliente " << _id << ": ";
	std::cout << _estados[_estado] << "\n";
}
/*
 * metodo que retorna o estado do cliente
 */

int Cliente::estado()
{
	return _estado;
}


/*
 * metodo para sortear se o cliente ira pedir ou nao
 * Diferentes metricas serao utilizadas para determinar a chance que o cliente
 * possuira de pedir baseado em seu desejo
 */

void Cliente::sorteiaPedido()
{
	std::random_device rd;
	std::mt19937 g(rd());
	if(_desejo == (int)Cliente::Desejo::Passar_Tempo)
	{
		std::uniform_int_distribution<int> x(0,10);
		int dice_roll = x(g);

		if(dice_roll % 2 == 0) _pede = false;
		else _pede = true;
	}
	else if(_desejo == (int)Cliente::Desejo::Encher_a_cara)
	{
		std::uniform_int_distribution<int> x(0,10);
		int dice_roll = x(g);

		if(dice_roll  == 1 || dice_roll == 2 || dice_roll == 3) _pede = false;
		else _pede = true;

	}
	else if(_desejo == (int)Cliente::Desejo::Dia_de_Maldade)
	{
		std::uniform_int_distribution<int> x(0,10);
		int dice_roll = x(g);

		if(dice_roll  == 1) _pede = false;
		else _pede = true;

	}

}

bool Cliente::pede()
{
	return _pede;
}




//Metodos Privados da classe Cliente
/*
 * Metodo que determina se o cliente podera realizar o pedido
 */

void Cliente::comeca(std::mutex &mut1, std::condition_variable &cv1)
{
	{
		std::unique_lock<std::mutex> lm(mut1);
		//Barreira para os clientes
		_estado = int(Cliente::Estados::Aguardando);
		cv1.wait(lm,[&]{return Bar::novaRodada() && _pede;});
	}
	cv1.notify_all();
}

/*
 * Metodo rotina do cliente:
 * Enquanto as rodadas nao acabarem o cliente continua executando a rotina
 */
void Cliente::runCliente(std::mutex &mut1,std::condition_variable &cv1, std::deque<Pedido> &barPedidos,
							std::deque<Pedido> &barProdutos,unsigned int &barRodadas)
{
	do
	{
		comeca(mut1,cv1);
		realizaPedido(mut1,cv1,barPedidos);
		esperaPedido(mut1,cv1,barProdutos);
		consomePedido(mut1,cv1,barProdutos, barRodadas);
	}while((Bar::rodada() < barRodadas));
	sair(mut1,cv1);

	//Espera com que todos os clientes saim do bara para poder emitir o feedback
	//Age como uma barreira ateh que todos acabem
	{
		std::unique_lock<std::mutex> lm(mut1);
		cv1.wait(lm, [&]{return Bar::clientesAcabaram();});
		_estado = (int)Cliente::Estados::Saiu;
		std::cout << "\n as rodadas acabaram e ";
		std::cout << " o cliente " << _id << " consumiu " << _consumiu << " itens \n";
	}
	cv1.notify_all();
}

/*
 * Metodo de espera a producao do pedido pelo garcom:
 * Espera o garcom produzir o seu pedido verificando o tamanho do buffer de produtos do bar
 * e checando se o pedido que chegou eh o seu
 */
void Cliente::esperaPedido(std::mutex &mut1, std::condition_variable &cv1,
						std::deque<Pedido> &barProdutos)
{
	{
		std::unique_lock<std::mutex> lm(mut1);

		if(_estado != int(Cliente::Estados::Esperando))
		{
			_estado = int(Cliente::Estados::Esperando);
			imprimeEstado();
		}

		
		cv1.wait(lm,[&]{return !barProdutos.empty() && barProdutos.front().cliente() == _id;});

		std::cout << "Garcom " << barProdutos.front().garcom();
		std::cout << " entrega pedido para cliente " << _id << " \n";
		barProdutos.pop_front();

		if(_estado != int(Cliente::Estados::Consumindo))
		{
			_estado = int(Cliente::Estados::Consumindo);
			imprimeEstado();
		}
		_pede = false;
		Bar::decrementaPedido();
	}
	cv1.notify_all();
}

/*
 * Metodo de consumo do pedido:
 * consome o pedido e demora um tempo aleatorio
 */
void Cliente::consomePedido(std::mutex &mut1,std::condition_variable &cv1,
							std::deque<Pedido> &barProdutos,unsigned int &barRodadas)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> x(1,10);
	int dice_roll = x(g);
	std::this_thread::sleep_for(std::chrono::milliseconds(dice_roll));
	{
		std::unique_lock<std::mutex> lm(mut1);
		_consumiu++;

		if(Bar::rodada() == barRodadas )
		{
			_estado = (int)Cliente::Estados::Acabou;
			_pede = false;
		}


	}
	cv1.notify_all();

}

void Cliente::sair(std::mutex &mut1, std::condition_variable &cv1)
{
	{
		std::unique_lock<std::mutex> lm(mut1);
		_estado = (int)Cliente::Estados::Acabou;
		_pede = false;
	}
	cv1.notify_all();

}
/*
 * Metodo que realiza o pedido do cliente
 * Espera que o cliente termine de consumir
 * realiza o pedido adicionando-o no buffer de pedidos do bar
 */

void Cliente::realizaPedido(std::mutex &mut1,std::condition_variable &cv1,
								std::deque<Pedido> &barPedidos)
{
	Pedido _pedido(_id,_id);
	{
		std::unique_lock<std::mutex> lm(mut1);

		if(_estado != int(Cliente::Estados::Pedindo))
		{
			_estado = int(Cliente::Estados::Pedindo);
			imprimeEstado();
		}

		barPedidos.push_back(_pedido);
		Cliente::decrementa();
	}
	cv1.notify_all();
}

//metodos estaticos

/*
 * retorna a quantidade de clientes que quer pedir
 */
unsigned int Cliente::pediu()
{
	return _pediu;
}

void Cliente::incrementa()
{
	_pediu++;
}

void Cliente::decrementa()
{
	_pediu--;
}

