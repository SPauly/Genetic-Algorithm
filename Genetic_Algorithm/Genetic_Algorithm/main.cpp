#include "GeneticAlgorithm.h"

int main() {
	float ftarget;
	GeneticAlgorithm GA{ ftarget };
	while (1) {
		cout << "Enter a target number: ";
		std::cin >> ftarget;
		GA = ftarget;
		GA.solve();
	}
}