// Adiciona todos lib padrão do C++
// https://gist.github.com/eduarc/6022859
#include <bits/stdc++.h>
#include "../client/common/utils.h"

using namespace std;

typedef struct keyValue
{
	string name;
	string value;
} keyValue;

int main(int argc, char const *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "Usage is:\n./serventTP3 <localport> <key-values> <ip1:port1> ... <ipN:portN>\nSystem abort.\n");
		exit(EXIT_FAILURE);
	}

	int localport;
	string aux, line;
	keyValue key;
	std::vector<keyValue> keyVector;

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
	
		//std::cout << key.name << " - " << key.value << endl;

		// Insere chave no vetor.
		keyVector.push_back(key);
	}

	return 0;
}