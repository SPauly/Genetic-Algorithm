#include "GeneticAlgorithm.h"

//ctor

GeneticAlgorithm::GeneticAlgorithm(float& fTrgt): fTarget{fTrgt}{}


//dtor

GeneticAlgorithm::~GeneticAlgorithm()
{
	v_population.clear();
}


//public member functions

float GeneticAlgorithm::operator=(float fnew_target) {
	fTarget = fnew_target;
	return fTarget;
}

void GeneticAlgorithm::solve() {
	__init__pop(); //initialize the population

	generation_count = 0; //set generation counter back to 0;
	bFoundSolution = false;

	while(!this->bFoundSolution){ //run through the algorithm untill we found a solution or we hit MAX_ALLOWED_GENERATIONS
		ftotal_fitness = 0.0f; //set the total fitness of the current generation back to 0
		
		if (AssignFitness() == true) { //have we already found a solution?
			PrintChromo(s_solution_ptr);
			break;
		}

		int counter = 0; //counter for going through the population
		std::vector<s_chromo_type> temp(POP_SIZE); // temporary objekt to hold our new created generation

		while (counter < POP_SIZE) {
			std::string offspring1 = Roulette(v_population.data()); //choose two offsprings from the last population that survive
			std::string offspring2 = Roulette(v_population.data()); //#survival_of_the_fittest

			Crossover(offspring1, offspring2);

			Mutate(offspring1);
			Mutate(offspring2);

			temp.at(counter++) = s_chromo_type(offspring1, 0.0f);
			temp.at(counter++) = s_chromo_type(offspring2, 0.0f);

			offspring1.clear();
			offspring2.clear();
		}

		for (int i = 0; i < POP_SIZE; i++) {
			v_population.at(i) = temp.at(i); //copy temp to our population
		}

		++generation_count;

		if (generation_count > MAX_ALLOWED_GENERATIONS) {
			cout << "No solutions found this run..." << endl;
			bFoundSolution = true;
			break;
		}
		temp.clear();
	}
	v_population.clear();
}


//private member functions

void GeneticAlgorithm::__init__rand() {
	srand((int)time(nullptr));
}

void GeneticAlgorithm::__init__pop() {  
	__init__rand();
	for (int i = 0; i < POP_SIZE; i++) {
		v_population.at(i).bits = getRandomBits(CHROMO_LENGTH); //Create a random Chromosome
		v_population.at(i).fFitness = 0.0f;
	}
}

void GeneticAlgorithm::PrintChromo(s_chromo_type* chromo_ptr) {
	int buffer[ (int)(CHROMO_LENGTH / GENE_LENGTH) ]; //this buffer holds the decoded chromosome
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

void GeneticAlgorithm::Crossover(std::string& offspring1, std::string& offspring2) {
	if (RANDOM_NUM < CROSSOVER_RATE) { //will crossover be applied?
		int crossover = (int)(RANDOM_NUM * CHROMO_LENGTH); //choose a point after which crossover will be applied

		std::string temp1 = offspring1.substr(0, crossover) += offspring2.substr(crossover, CHROMO_LENGTH); // apply crossover
		std::string temp2 = offspring2.substr(0, crossover) += offspring1.substr(crossover, CHROMO_LENGTH);

		offspring1 = temp1;
		offspring2 = temp2;
	}
}

void GeneticAlgorithm::Mutate(std::string& offspring) {
	for (int i = 0; i < offspring.length(); i++) { //iterate through every bit
		if (RANDOM_NUM < MUTATION_RATE) {  //mutate this gene?
			if (offspring.at(i) == '1') //swap bites
				offspring.at(i) = '0';
			else
				offspring.at(i) = '1';
		}
	}
}

std::string GeneticAlgorithm::getRandomBits(int length) { 
	std::string bits = "";
	for (int i = 0; i < length; i++) {         //choose 1 or 0
		if (RANDOM_NUM > 0.5f)       
			bits += '1';
		else
			bits += '0';
	}
	return bits;
}

std::string GeneticAlgorithm::Roulette(s_chromo_type* pop_ptr){ // pop_ptr is a pointer to the first element in our population vector
	float slice = (float)(RANDOM_NUM * ftotal_fitness); //create a random slice within our total fitness

	float ftns_so_far = .0f; //to messure where our "wheel" is at

	for (int i = 0; i < POP_SIZE; i++) {
		ftns_so_far += pop_ptr[i].fFitness; //turn the wheel
		if (ftns_so_far >= slice) //is the wheel at the slice we have chosen?
			return pop_ptr[i].bits;
	}
}


bool GeneticAlgorithm::AssignFitness() {

	for (int i = 0; i < POP_SIZE; i++) { //Assigne a fitness to each chromosome and add it to the total fitness
		int buffer[(int)(CHROMO_LENGTH / GENE_LENGTH)]; //this buffer holds the decoded chromosome
		int num_elements = ParseGen(v_population.at(i).bits, buffer);  //fill the buffer with elements and store it's size in num_elements
		float fcurrent_score = .0f; // the score of the current chromosome

		for (int i = 0; i < num_elements - 1; i += 2) { //find out how good the chromosome solution is And Important: increase by two cause we are skipping the operator
			switch (buffer[i])
			{
			case 10:  // 10 = +
				fcurrent_score += buffer[i + 1]; // current score + the next number by increasing i we make sure that we always land on an operator 
				break;
			case 11: // 11 = -
				fcurrent_score -= buffer[i + 1];
				break;
			case 12: // 12 = *
				fcurrent_score *= buffer[i + 1];
				break;
			case 13: // 13 = /
				fcurrent_score /= buffer[i + 1];
				break;
			default:
				break;
			}
		}

		if (fcurrent_score == this->fTarget) { //check if we already found a solution and point the ptr to it
			this->bFoundSolution = true;
			this->s_solution_ptr = &v_population.at(i);
			break;
		}
		v_population.at(i).fFitness = 1 / (fcurrent_score - this->fTarget); //assigne fitness
		ftotal_fitness += v_population.at(i).fFitness;
	}

	return bFoundSolution;
}

int GeneticAlgorithm::ParseGen(std::string chromo, int* buffer_ptr) {
	int buffer_index = 0; //index for buffer
	bool bOperator = true; //determines whether we are searching for an operator
	int current_gen = 0; //stores the current gen

	for (int i = 0; i < CHROMO_LENGTH; i += GENE_LENGTH) { //Parse each gene !!!Remember we have to increase by GENE_LENGTH in order to catch all the 4 bit pairs
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

int GeneticAlgorithm::BinToDec(std::string bit ) {
	int decemal = 0; 
	int value_to_add = 1; //holds the value of the positions bit 

	for (int i = bit.length(); i > 0; i--) {    //start at the least important bit
		if (bit.at(i - 1) == '1')     //if the bit is set add the bits value to decemal
			decemal += value_to_add;

		value_to_add *= 2; //increase the value according to the position 
	}

	return decemal;
}
