// Adiciona todos lib padrão do C++
// https://gist.github.com/eduarc/6022859
#include <bits/stdc++.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "common/utils.h"

using namespace std;

// Estruturas gerais:

#define CLIREQ 1
#define QUERY 2
#define RESPONSE 3

#define MAX_KEY_SIZE 201

typedef struct msg
{
	uint16_t type;
	uint16_t ttl;
	struct in_addr ip_client;
	uint16_t port_client;
	uint32_t seq_num;
	char key[MAX_KEY_SIZE];
} msg_t;

// Estruturas apenas do servent:

int NUM_SEQ = 0;

typedef struct keyValue
{
	string name;
	string value;
} keyValue_t;

typedef struct neighbor
{
	char* ip;
	int port;
} neighbor_t;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		std::cerr << "Usage is:\n./serventTP3 <localport> <key-values> <ip1:port1> ... <ipN:portN>\nSystem abort." << endl;
		exit(1);
	}

	unsigned int i;
	int localport, socketFD;
	bool find;
	struct sockaddr_in servAddr;
	string aux, line;
	keyValue_t key;
	std::vector<keyValue_t> dictionary;
	neighbor_t ngbr;
	std::vector<neighbor_t> neighborhood;

	localport = atoi(argv[1]);
	std::ifstream file(argv[2]);
	
	while(getline(file, line))
	{
		// Ignora comentários.
		if(line[0] == '#')
			continue;

		// Transforma a linha lida em fluxo de dados.
		istringstream keyStream(line);
		
		keyStream >> key.name >> key.value;
		// Fazer assim elimina os espaços entre chave e valor.
		getline(keyStream, aux);
		key.value.append(aux);

		// Atualiza dicionario se já existir entrada.
		find = false;
		for(i = 0; i < dictionary.size(); i++)
		{
			if(dictionary[i].name.compare(key.name) == 0)
			{
				dictionary[i].value = key.value;
				find = true;
				break;
			}
		}

		// Insere no dicionário se ainda não existir entrada.
		if(find == false)
			dictionary.push_back(key);
	}

	i = argc;
	while(i > 3)
	{
		i--;
		ngbr.ip = strtok(argv[i], ":");
		ngbr.port = atoi(strtok(NULL, " "));
		
		neighborhood.push_back(ngbr);
	}
//--------------------------------------------------------------//
	//PRINT PRA VER SE TÔ LENDO AS BUDEGA DIREITO.
/*
	std::cout << "DICTIONARY:" << endl;
	for(i = 0; i < dictionary.size(); i++)
		std::cout << dictionary[i].name << " - " << dictionary[i].value << endl;

	std::cout << endl << "NEIGHBORHOOD:" << endl;
	for(i = 0; i < neighborhood.size(); i++)
		std::cout << neighborhood[i].ip << " - " << neighborhood[i].port << endl;
*/
//--------------------------------------------------------------//

	if((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cerr << "Error on trying to opening socket. " << strerror(errno) << endl;
		exit(1);
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(localport);

	if(bind(socketFD, (struct sockaddr *) &servAddr, sizeof(struct sockaddr)) < 0)
	{
		std::cerr << "Error on trying to bind: " << strerror(errno) << endl;
		exit(1);
	}

	std::cout << "Servent on port: " << localport << endl;
	std::cout << "Waiting for data on UDP socket" << endl;

	msg_t msg, query, response;
	socklen_t remoteLength;
	struct sockaddr_in remoteAddr, ngbrAddr, cliAddr;

	// Loop de receber mensagens pelo servidor.
	while(1)
	{
		// Limpando buffer que receberá a mensagem.
		// Cliente sempre tem que por \0, acho que não precisa do memset.
		//memset(msg->key, 0, MAX_KEY_SIZE);

		// Recebendo a mensagem.
		remoteLength = sizeof(remoteAddr);
		if((recvfrom(socketFD, &msg, sizeof(msg_t), 0, (struct sockaddr*) &remoteAddr, &remoteLength)) < 0)
		{
			std::cerr << "Error on trying to receive data: " << strerror(errno) << endl;
			exit(1);
		}

		// Imprimindo mensagem recebida.
		std::cout << "Mensagem recebida: " << msg.key << endl;

		// TODO: Conferir se está realmente recebendo corretamente.

		// Se a mensagem for do tipo CLIREQ.
		if(msg.type == CLIREQ)
		{
			// Incrementa o número de sequência.
			NUM_SEQ++;

			// Construindo a mensagem query.
			query.type = QUERY;
			query.ttl = 3;
			query.ip_client = remoteAddr.sin_addr;
			query.port_client = remoteAddr.sin_port;
			query.seq_num = NUM_SEQ;
			strcpy(query.key, msg.key);

			// Enviando a query a todos os vizinhos.
			for(i = 0; i < neighborhood.size(); i++)
			{
				// Constrói endereço do vizinho.
				ngbrAddr.sin_family = AF_INET;
				ngbrAddr.sin_port = htons(neighborhood[i].port);
				inet_pton(AF_INET, neighborhood[i].ip, &ngbrAddr.sin_addr);
				
				// Envia mensagem para o vizinho.
				if((sendto(socketFD, &query, sizeof(msg_t), 0, (struct sockaddr*) &ngbrAddr, sizeof(ngbrAddr))) < 0)
				{
					std::cerr << "Error on trying to sending data to " << neighborhood[i].ip << ":" << neighborhood[i].port 
							  << endl << "Error: " << strerror(errno) << endl;
					exit(1);
				}
			}

			// Verificando se existe a chave no dicionário.
			for(i = 0; i < dictionary.size(); i++)
			{
				// c_str() converte string em char*. É necessário :(
				
				// Se existir, responder para o cliente.
				if(strcmp(msg.key, dictionary[i].name.c_str()) == 0)
				{
					// Construindo a mensagem response.
					response.type = RESPONSE;
					
					string resp;
					resp = dictionary[i].name;
					resp.append("\t");
					resp.append(dictionary[i].value);
					resp.append("\0");

					strcpy(response.key, resp.c_str());

					// Respondendo ao cliente (nesse caso é o remoteAddr).
					if((sendto(socketFD, &response, sizeof(msg_t), 0, (struct sockaddr*) &remoteAddr, sizeof(remoteAddr))) < 0)
					{
						std::cerr << "Error on trying to sending data to " << neighborhood[i].ip << ":" << neighborhood[i].port 
								  << endl << "Error: " << strerror(errno) << endl;
						exit(1);
					}
				}
			}
		}

		// Se a mensagem for do tipo QUERY.
		else if(msg.type == QUERY)
		{
			// TODO: Fazer algoritmo de alagamento confiável, para evitar
			// que mensagens repetidas espalhem pela rede. Basta conferir
			// o IP:Port, NUM_SEQ e key da mensagem.

			// Procura pela chave no dicionário local.
			for(i = 0; i < dictionary.size(); i++)
			{
				// c_str() converte string em char*. É necessário :(
				
				// Se existir, responder para o cliente.
				if(strcmp(msg.key, dictionary[i].name.c_str()) == 0)
				{
					// Construindo a mensagem response.
					response.type = RESPONSE;
					
					string resp;
					resp = dictionary[i].name;
					resp.append("\t");
					resp.append(dictionary[i].value);
					resp.append("\0");

					strcpy(response.key, resp.c_str());

					// Descobrindo endereço do cliente (já que remoteAddr, nesse caso, é um servent).
					cliAddr.sin_family = AF_INET;
					cliAddr.sin_port = msg.port_client;
					cliAddr.sin_addr = msg.ip_client;

					// Respondendo ao cliente.
					if((sendto(socketFD, &response, sizeof(msg_t), 0, (struct sockaddr*) &cliAddr, sizeof(cliAddr))) < 0)
					{
						std::cerr << "Error on trying to sending data to client. Error: " << strerror(errno) << endl;
						exit(1);
					}
				}
			}

			// Decrementa o TTL.
			msg.ttl--;

			// Se TTL for maior que zero. 
			if(msg.ttl > 0)
			{
				// Descobre IP e porta do remoteAddr
				char ip_remote[INET_ADDRSTRLEN];
				int port_remote;

				inet_ntop(AF_INET, &remoteAddr.sin_addr, ip_remote, INET_ADDRSTRLEN);
				port_remote = ntohs(remoteAddr.sin_port);

				// Retransmite a mensagem para os vizinhos.
				for(i = 0; i < neighborhood.size(); i++)
				{
					// Se vizinho for quem mandou, não envia pra ele.
					if((strcmp(neighborhood[i].ip, ip_remote) == 0) && neighborhood[i].port == port_remote)
						continue;

					// Constrói endereço do vizinho.
					ngbrAddr.sin_family = AF_INET;
					ngbrAddr.sin_port = htons(neighborhood[i].port);
					inet_pton(AF_INET, neighborhood[i].ip, &ngbrAddr.sin_addr);

					// Repassando a mensagem para o vizinho.
					if((sendto(socketFD, &msg, sizeof(msg_t), 0, (struct sockaddr*) &ngbrAddr, sizeof(ngbrAddr))) < 0)
					{
						std::cerr << "Error on trying to sending data to " << neighborhood[i].ip << ":" << neighborhood[i].port 
								  << endl << "Error: " << strerror(errno) << endl;
						exit(1);
					}
				}
			}
		}


		/*
		* TODO:
		*
		* Após receber a mensagem, identificar de qual tipo é a mensagem
		* (CLIREQ ou QUERY) e fazer as ações necessárias.
		*
		* Se CLIREQ, criar uma QUERY e repassar. Após isso, verificar se
		* o dicionário local tem a chave procurada, e se tiver responder
		* para o cliente criando uma RESPONSE. [DONE]
		*
		* Se QUERY, decrementar o TTL e repassar para os vizinhos menos
		* de quem recebeu. Verificar se o dicionário local tem a chave
		* procurada e se tiver, responder para o cliente com uma RESPONSE.
		* [DONE]
		* 
		*/
	}

	return 0;
}