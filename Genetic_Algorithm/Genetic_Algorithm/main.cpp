#include "GeneticAlgorithm.h"

int main() {
	float ftarget;
	GeneticAlgorithm GA{ ftarget };
	cout << "Enter a target number: ";
	std::cin >> ftarget;
	GA.solve();
	std::cin.get();
}