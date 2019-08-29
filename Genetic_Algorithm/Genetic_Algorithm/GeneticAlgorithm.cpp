#include "GeneticAlgorithm.h"

//ctor

GeneticAlgorithm::GeneticAlgorithm(float& fTrgt): fTarget{fTrgt}
{
	__init__pop();
}


//dtor

GeneticAlgorithm::~GeneticAlgorithm()
{
}


//public member functions

float GeneticAlgorithm::operator=(float& fnew_target) {
	fTarget = fnew_target;
}

void GeneticAlgorithm::solve(GeneticAlgorithm* GA_ptr) {
	__init__pop(); //initialize the population

	generation_count = 0; //set generation counter back to 0;

	while(!bFoundSolution){ //run through the algorithm untill we found a solution or we hit MAX_ALLOWED_GENERATIONS
		ftotal_fitness = 0; //set the total fitness of the current generation back to 0

	}
}


//private member functions

void GeneticAlgorithm::__init__rand() {
	srand((int)time(nullptr));
}

void GeneticAlgorithm::__init__pop() {  
	__init__rand();
	for (int i = 0; i < POP_SIZE; i++) {
		v_population.at(i).bits = getRandomBits(CHROMO_LENGTH); //Create a random Chromosome
		v_population.at(i).fFitness = .0f;
	}
}

std::string GeneticAlgorithm::getRandomBits(int length) { 
	std::string temp = "";
	for (int i = 0; i < length; i++) {         //choose 1 or 0
		if (RANDOM_NUM > 0.5f)       
			temp += '1';
		else
			temp += '0';
	}
	return temp;
}

float& GeneticAlgorithm::AssigneFitness(float& fTrgt, float& tot_ftns) {

}