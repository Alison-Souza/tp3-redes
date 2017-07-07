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

		// Insere o par chave-valor no dicionário.
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
	std::cout << "DICTIONARY:" << endl;
	for(i = 0; i < dictionary.size(); i++)
		std::cout << dictionary[i].name << " - " << dictionary[i].value << endl;

	std::cout << endl << "NEIGHBORHOOD:" << endl;
	for(i = 0; i < neighborhood.size(); i++)
		std::cout << neighborhood[i].ip << " - " << neighborhood[i].port << endl;
//--------------------------------------------------------------//

	if((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cerr << "Error in trying to opening socket. " << strerror(errno) << endl;
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

	return 0;
}