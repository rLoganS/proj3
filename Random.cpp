/***************************************************************
<Logan Snyder>
<Random.cpp>
<Program 3>
<Class contains functions for generating random values of various different types>
***************************************************************/

#include <iostream>
#include <chrono>
#include <random>
#include "Random.h"

// Creates a new random number generator using an optional long seed.
Random::Random(long seed)
{
	_generator.seed(seed);
}

// Returns the next pseudorandom, uniformly distributed boolean value from this random number generator's sequence.
bool Random::nextBoolean() const
{
	// generates a random integer
	int value = _uniformIntDistribution(_generator);

	// Turns the integer into a boolean. If the integer generated is even (thus value % 2 = 0) the function returns true, otherwise, false.
	if (value % 2 == 0)
	{
		return true;
	}

	return false;
}

// Generates random bytes and places them into a user - supplied byte array
void Random::nextBytes(char bytes[], int numBytes) const
{
	// declares uniform distribution of integers from 0 to 255.
	std::uniform_int_distribution<int> distribution(0, 255);

	// assigns random bytes to each index in the supplied array
	for (int i = 0; i < numBytes; i++)
	{
		bytes[i] = distribution(_generator);
	}
}

// Returns the next pseudorandom, uniformly distributed double value between 0.0 and 1.0 from this random number generator's sequence.
double Random::nextDouble() const
{
	// declares a uniform real distribution of type double with bounds of 0.0 and 1.0
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	return distribution(_generator);
}

// Returns the next pseudorandom, uniformly distributed float value between 0.0 and 1.0 from this random number generator's sequence.
float Random::nextFloat() const
{
	// declares a uniform real distribution of type double with bounds of 0.0 and 1.0
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	return distribution(_generator);
}

// Returns the next pseudorandom, Gaussian("normally") distributed double value with mean 0.0 and standard deviation 1.0 from this random number generator's sequence.
double Random::nextGaussian() const
{
	// declares a normal / gaussian distribution with mean 0.0 and std dev 1.0
	std::normal_distribution<double> distribution(0.0, 1.0);

	return distribution(_generator);
}

// Returns the next pseudorandom, uniformly distributed int value from this random number generator's sequence.
int	Random::nextInt() const
{
	// assigns a random integer to the identifier "randomInt"
	int randomInt = _uniformIntDistribution(_generator);

	return randomInt;
}

// Returns a pseudorandom, uniformly distributed int value between 0 (inclusive) and the specified value(exclusive), drawn from this random number generator's sequence.
int	Random::nextInt(int n) const
{
	// declares a uniform int distribution between 0 inclusive and n exclusive.
	// The values given in parentheses are both inclsuive, so to make n exclsuive, 1 is subtracted
	std::uniform_int_distribution<int> distribution(0, n - 1);

	return distribution(_generator);
}

// Returns a pseudorandom, uniformly distributed int value between low (inclusive) and high(inclusive), drawn from this random number generator's sequence.
int	Random::nextInt(int low, int high) const
{
	// declares a unform int distribution between low (inclsuvie) and high (inclusive).
	std::uniform_int_distribution<int> distribution(low, high);

	return distribution(_generator);
}

// Returns the next pseudorandom, uniformly distributed long value from this random number generator's sequence.
long Random::nextLong() const
{
	// defines a uniform integer distribution of type long
	std::uniform_int_distribution<long> distribution;

	return distribution(_generator);
}

// Sets the seed of this random number generator using a single long seed.
void Random::setSeed(long seed)
{
	// Sets the seed for the random value generator with whatever was passed to the function.
	_generator.seed(seed);
}