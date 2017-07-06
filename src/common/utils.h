#ifndef _UTILS_H_
#define _UTILS_H_

#define HEADER "\033[95m"
#define OKBLUE "\033[94m"
#define OKGREEN "\033[92m"
#define WARNING "\033[93m"
#define FAIL "\033[91m"
#define ENDC "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#include <bits/stdc++.h>

namespace utils {
	void die(char * s);

	void print_red(std::string s);

	std::vector<std::string> split_string (std::string s, char separator);
}

#endif