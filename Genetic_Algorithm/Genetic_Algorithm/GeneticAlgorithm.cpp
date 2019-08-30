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
	return fTarget = fnew_target;
}

void GeneticAlgorithm::solve() {
	__init__pop(); //initialize the population

	generation_count = 0; //set generation counter back to 0;

	while(!bFoundSolution){ //run through the algorithm untill we found a solution or we hit MAX_ALLOWED_GENERATIONS
		ftotal_fitness = 0; //set the total fitness of the current generation back to 0
		if (AssigneFitness()) { //found a solution already?
			cout << "Found Solution: " << endl;
			PrintChromo(this->s_solution_ptr);
		}
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

bool GeneticAlgorithm::AssigneFitness() {
	for (int x = 0; x < POP_SIZE; x++) {
		int buffer[ (int)(CHROMO_LENGTH / GENE_LENGTH) ]; //this buffer holds the decoded chromosome
		int num_elements = ParseGen(v_population.at(x).bits, buffer);  //fill the buffer with elements and store it's size in num_elements
		float fcurrent_score = .0f; // the score of the current chromosome

		for (int i = 0; i < num_elements; i++) { //find out how good the chromosome solution is
			switch (buffer[i])
			{
			case 10:  // 10 = +
				fcurrent_score += buffer[++i]; // current score + the next number by increasing i we make sure that we always land on an operator 
				break;
			case 11: // 11 = -
				fcurrent_score -= buffer[++i];
				break;
			case 12: // 12 = *
				fcurrent_score *= buffer[++i];
				break;
			case 13: // 13 = /
				fcurrent_score /= buffer[++i];
				break;
			default:
				break;
			}
		}

		if (fcurrent_score == this->fTarget) { //check if we already found a solution and point the ptr to it
			this->bFoundSolution = true;
			this->s_solution_ptr = &v_population.at(x);
			this->ftotal_fitness += v_population.at(x).fFitness;
			return this->bFoundSolution;
		}

		v_population.at(x).fFitness = 1 / (fcurrent_score - this->fTarget); //assigne fitness
	}
	return this->bFoundSolution;
}

int GeneticAlgorithm::ParseGen(const std::string& chromo, int* buffer_ptr) {
	int buffer_index = 0; //index for buffer
	bool bOperator = true; //determines whether we are searching for an operator
	int current_gen = 0; //stores the current gen

	for (int i = 0; i < chromo.length(); i++) { //Parse each gen
		current_gen = BinToDec(chromo.substr(i, GENE_LENGTH));  //decode the current bit. substr(start,end) returns the part of the string from start to end

		if (bOperator) { // Make sure that we follow operator number operator number...
			if (current_gen < 10 || current_gen > 13) // continue if it is not an operator
				continue;
			else {
				bOperator = false; // we are no longer looking for an operator
				buffer_ptr[buffer_index++] = current_gen; //store current gen in the buffer and increase the buffers index
				continue;
			}
		}
		else {
			if (current_gen > 9) // make sure that it is a number
				continue;
			else {
				bOperator = true; // we are searching for an operator 
				buffer_ptr[buffer_index++] = current_gen; //store current gen in the buffer and increase the buffers index
				continue;
			}
		}
	}

	for (int i = 0; i < buffer_index; i++) { //check that there is no '/' by 0 and if there is turn '/' into '+'
		if (buffer_ptr[i] == 13 && buffer_ptr[i + 1] == 0)
			buffer_ptr[i] = 10;
	}

	return buffer_index; //so that we know the buffers size
}

int GeneticAlgorithm::BinToDec(const std::string& bit ) {
	int decemal = 0; 
	int value_to_add = 1; //holds the value of the positions bit 

	for (int i = bit.length(); i > 0; i--) {    //start at the least important bit
		if (bit.at(i - 1) == '1')     //if the bit is set add the bits value to decemal
			decemal += value_to_add;

		value_to_add *= 2; //increase the value according to the position 
	}

	return decemal;
}

void GeneticAlgorithm::PrintChromo(const s_chromo_type* chromo_ptr) {
	int buffer[(int)(CHROMO_LENGTH / GENE_LENGTH)]; //this buffer holds the decoded chromosome
	int num_elements = ParseGen(chromo_ptr->bits, buffer); //fill the buffer with elements and store it's size in num_elements
	cout << "\n Gene: ";
	for (int i = 0; i < num_elements; i++) {
		if (buffer[i] <= 9) //cout the number
			cout << buffer[i];
		else
			PrintSymbol(buffer[i]); //cout symbol
	}
	cout << "Its fitness: ";
	cout << chromo_ptr->fFitness << endl;
}

void GeneticAlgorithm::PrintSymbol(int symbol) {
	switch (symbol)
	{
	case 10:
		cout << " + ";
		break;
	case 11:
		cout << " - ";
		break;
	case 12:
		cout << " * ";
		break;
	case 13:
		cout << " / ";
		break;
	default:
		break;
	}
}