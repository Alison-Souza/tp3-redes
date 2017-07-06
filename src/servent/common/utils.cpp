#include "utils.h"

// self
void utils::die(char * s)
{
	perror(s);
	exit(1);
}

void utils::print_red(std::string s) {
	std::cout << std::string(FAIL) << s << ENDC << '\n';
}

std::vector<std::string>
utils::split_string (std::string s, char separator) {
	using namespace std;
    istringstream iss { s };

    vector<string> tokens;

    string aux;
    while ( std::getline( iss, aux, separator ) )
    	tokens.push_back(aux);

    return tokens;
}