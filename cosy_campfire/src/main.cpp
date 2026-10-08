#include <iostream> // std::cerr

#include "Application.hpp"

int main() {
	try {
		Application app{};
		app.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}