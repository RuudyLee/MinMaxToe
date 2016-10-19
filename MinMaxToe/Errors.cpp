#include "Errors.h"

void FatalError(const std::string msg) {
	std::cout << "Fatal error encountered: " << msg << std::endl;
	system("pause");
	exit(0);
}