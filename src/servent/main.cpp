// Adiciona todos lib padrão do C++
// https://gist.github.com/eduarc/6022859
#include <bits/stdc++.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "common/utils.h"

using namespace std;

typedef struct keyValue
{
	string name;
	string value;
} keyValue_t;

typedef struct neighbor
{
	string ip;
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

	char* msg[100];
	socklen_t cliLen;
	struct sockaddr_in cliAddr;

	// Loop de receber mensagens pelo servidor.
	while(1)
	{
		// Limpando buffer que receberá a mensagem.
		memset(msg, 0, 100);

		// Recebendo a mensagem.
		cliLen = sizeof(cliAddr);
		if((recvfrom(socketFD, msg, 100, 0, (struct sockaddr*) &cliAddr, &cliLen)) < 0)
		{
			std::cerr << "Error on trying to receive data: " << strerror(errno) << endl;
			continue;
		}

		// Imprimindo mensagem recebida.
		std::cout << "Mensagem recebida: " << msg << endl;

		// TODO: Conferir se está realmente recebendo corretamente.

		/*
		* TODO:
		*
		* Após receber a mensagem, identificar de qual tipo é a mensagem
		* (CLIREQ ou QUERY) e fazer as ações necessárias.
		*
		* Se CLIREQ, criar uma QUERY e repassar. Após isso, verificar se
		* o dicionário local tem a chave procurada, e se tiver responder
		* para o cliente criando uma RESPONSE.
		*
		* Se QUERY, decrementar o TTL e repassar para os vizinhos menos
		* de quem recebeu. Verificar se o dicionário local tem a chave
		* procurada e se tiver, responder para o cliente com uma RESPONSE.
		*
		*/

		/*
		// Algo assim funcionaria ??
		// Acho que sim, mas temos que definir o modelo das mensagens...
		if(msg[0] == 1)
			// mensagem do tipo CLIREQ
		else if(msg[0] == 2)
			// mensagem do tipo QUERY
		*/
	}

	return 0;
}