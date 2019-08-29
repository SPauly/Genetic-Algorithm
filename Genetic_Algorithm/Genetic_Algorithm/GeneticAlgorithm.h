#pragma once
#include <iostream> //Input and Output
#include <string> //string
#include <ctime>  //time to initialize rand()
#include <cstdlib> //substr()
#include <cmath> //rand()
#include <vector> //we use a vector to hold the population

using std::cout;            //for better readability
using std::endl;            //for better readability

#define CROSSOVER_RATE 0.07 //Rate after which crossover will be applied
#define MUTATION_RATE 0.001 //The chance the a bit gets flipped
#define GENE_LENGTH 4 //The length of each gene 
#define CHROMO_LENGTH 300 //The length of each chromosome generated
#define POP_SIZE 300 //Size of the population. 
#define MAX_ALLOWED_GENERATIONS 1000 //The algorithm will stop after reaching this number

#define RANDOM_NUM ((float)rand()/(RAND_MAX +1))


class GeneticAlgorithm      //Class to hold the genetic Algorithm
{
	float fTarget = .0f; //The target 
	float ftotal_fitness = .0f; //the total fitness of the chromosomes to 

	int generation_count = 0; //how many generations have we made
	int solutions_pos = 0; //The index where we found our solution

	bool bFoundSolution = false; //have we found a solution yet

	struct s_chromo_type {   //A struct to hold the actual chromosome
		std::string bits{ "" }; //Holds the encoded Chromosome
		float fFitness = .0f; //Holds the fitness of this Chromosome

		s_chromo_type() : bits{ "" }, fFitness{ .0f }{};
		s_chromo_type(std::string bts, float ftns) : bits{ bts }, fFitness{ ftns }{};
	};

	std::vector<s_chromo_type> v_population{ POP_SIZE }; //this is our population 

	void __init__rand(); //initialize rand() with the time
	void __init__pop(); //initialize a new random population

	std::string getRandomBits(int); //create a random chromosome

	float& AssigneFitness(float&, float&);

public:

	GeneticAlgorithm() = delete;
	GeneticAlgorithm(float&);

	~GeneticAlgorithm();

	float operator=(float&); //use '=' to set new target
	void solve(GeneticAlgorithm*); //member function to find a way to solve our problem
};
