#ifndef DISTN_GUARD
#define DISTN_GUARD

#include <sstream>
#include <utility>
#include <vector>

#include "Random.h"
#include "Types.h"

template<typename KEY_T>
class DiscreteDistribution
{
protected:
	// Map of keys to cumulative weights
	std::vector<std::pair<KEY_T, WEIGHT_T> > _valueToWeightMap;

	// Vector to hold individual weights
	// Not maintained as elements are added, but populated / cleared in add method for weight calculation.
	std::vector<WEIGHT_T> _individualWeights;

	// The random number generator
	Random _rng;

	// Helper function for add function to find and return he index of the element being searched for
	// @param value - The value to find in the _valueToWeightMap vector
	// @return the index of the value in the _valueToWeightMap vector (or -1 if it is not present)
	int find(const KEY_T& value) const
	{
		// Index default value represents not found; Only updated if it is found.
		int index = -1;

		for (int i = 0; i < _valueToWeightMap.size(); i++)
		{
			if (_valueToWeightMap.at(i).first == value)
			{
				// if the two values are equal, i is the index of the value being searched for, so it is assigned here.
				index = i;
			}
		}

		return index;
	}

	// Helper function for add function - Anytime a new value is added to the distribution, this will
	// Update the cumulative weights accordingly.
	// @ param startingIndex where to start updating weights (process "goes down the list")
	void updateWeights(int startingIndex)
	{
		
		// the weight at position 0 is inherintely its cumulative weight. So if position 0 is passed to the func, 
		// the weight at that position is not updated, but the value is udpate to 1 so the loop below it works.
		if (startingIndex == 0)
		{
			startingIndex = 1;
		}
		
		// counter variable for _initialWeights vector.
		int j = 1;
		
		// adds weight of previous element to current one
		for (int i = startingIndex; i < _valueToWeightMap.size(); i++)
		{
			_valueToWeightMap.at(i).second = _individualWeights.at(j) + _valueToWeightMap.at(i - 1).second;
			j++;
		}
	}

public:

	// Default constructor - constructs empty object (values added with add function.)
	DiscreteDistribution()
	{

	}

	// @param key -- key value for mapping
	// @param weight -- weight of the particular key
	//    If the key is not in the distribution, add it, otherwise,  
	// @throws std::string object for negative weights (when the @param weight is cast to a standard int)
	void add(const KEY_T& key, const WEIGHT_T& weight)
	{
		
		// Exception thrown if weight < 0
		if (weight < 0)
		{
			throw std::string("Weight must be >= 0");
		}
		
		int index = find(key);

		// -1 means key is not in vector, so it is added here
		if (index == -1)
		{
			
			// If you're adding the first element to the distribution, simply put it at the end and you're done.
			// It weight is inherintely its cumulative weight as it's the only element, so updateWeights() isn't called
			if (_valueToWeightMap.size() == 0)
			{
				_valueToWeightMap.push_back(std::make_pair(key, weight));
			}

			// If you're adding at 2nd or higher element to the distribution, add it, then update the weight to the last
			// element to be the cumulative weight. Updating the cumulative weight is done in one line, so updateWeights()
			// isn't called
			else
			{
				_valueToWeightMap.push_back(std::make_pair(key, weight));
				_valueToWeightMap.at(_valueToWeightMap.size() - 1).second += _valueToWeightMap.at(_valueToWeightMap.size() - 2).second;
			}
		}

		else
		{
			// clears the individualWeights vector of prior contents to be repoulated.
			_individualWeights.clear();

			_individualWeights.push_back(_valueToWeightMap.at(index).second);

			// Populating _individualWeights
			for (int i = index + 1; i < _valueToWeightMap.size(); i++)
			{
				_individualWeights.push_back(_valueToWeightMap.at(i).second - _valueToWeightMap.at(i - 1).second);
			}
			
			// Need to ensure weight is immediately updated to cumulative. For psition 0, that's already the case, but
			// for >= 0, the value of the preceding weight needs to be added (done in else block)
			if (index == 0)
			{
				_valueToWeightMap.at(index).second = weight;
			}
			
			else
			{
				_valueToWeightMap.at(index).second = weight + _valueToWeightMap.at(index - 1).second;
			}

			// Updates the weights, starting at index + 1 and going down the list
			// It's index + 1 because the cumulative weight of the element at position "index" is properly updated in this function
			updateWeights(index+1);
		}
	}

	// returns the individual weight of a given key
	// @param key the given key
	// @return the weight that corresponds to the provided key
	WEIGHT_T weight(const KEY_T& key) const
	{
		// Key at position 0 has no proceeding key to subtract, so it's weight is just returned here.
		if (_valueToWeightMap.at(0).first == key)
		{
			return _valueToWeightMap.at(0).second;
		}

		//// returns the individual weight that corresponds to the key by subtracting the weight of the proceeding key.
		//for (/*int i = 0*/ int i = 1; i < _valueToWeightMap.size(); i++)
		//{
		//	return _valueToWeightMap.at(i).second - (_valueToWeightMap.at(i - 1).second);
		//}

		int index = find(key);
		return _valueToWeightMap.at(index).second - _valueToWeightMap.at(index-1).second;
	}

	// Returns the ex[ected probability of a given weight
	// @param wegith the given weight
	// @return the exepected probability that corresponds to the provided weight
	double expectedProb(WEIGHT_T weight) const
	{
		// probability is a part of whole / the whole.
		// Here, part is provided weight, whole is cumulative weight of last element to _valueToWeightMap
		
		double prob = (double)weight / (double)_valueToWeightMap.at(_valueToWeightMap.size() - 1).second;

		return prob;

		//return (double)weight / (double)_valueToWeightMap.at(_valueToWeightMap.size() - 1).second;
	}
	
	//
	// @return Based on the given weights, "roll a die" to return the resulting key value return a 
	//
	KEY_T operator()() const
	{
		// The upperbound for random integer distribution should be the max cumulative weight,
		// which is the cumulative weight of the last element in the vector, which is set here.
		int upperBound = _valueToWeightMap.at(_valueToWeightMap.size() - 1).second;

		// Random integer generated for random key selection (based on weights)
		int random = _rng.nextInt(upperBound);

		// vector to store the ranges of each key
		// Example: A is 1st element, has cumulative weight of 12. Its range is 0 - 12. This vector would store 0 and 12
		// at position 0.
		std::vector< std::pair<int, int> > ranges;

		// special case of adding first pair
		ranges.push_back(std::make_pair(0, _valueToWeightMap.at(0).second));
		
		// populates rest of vector
		for (int i = 1; i < _valueToWeightMap.size(); i++)
		{
			ranges.push_back(std::make_pair(((_valueToWeightMap.at(i - 1).second) + 1), _valueToWeightMap.at(i).second));
		}

		// returns key based on the range the random # generated falls in.
		for (int i = 0; i < ranges.size(); i++)
		{
			if (random >= ranges.at(i).first && random <= ranges.at(i).second)
			{
				return _valueToWeightMap.at(i).first;
			}
		}
	}
	
	//
	// @return string-based representation of DiscreteDistribution object
	//
	std::string toString() const
	{
		std::ostringstream oss;

		oss << "\t\t\t\t Cumulative" << std::endl;

		// for loop subtracts previous weight to show non-cumulative weights, but elem at position 0 has no previous
		// elements, so it's printed seperately here
		oss << _valueToWeightMap.at(0).first << ": " << _valueToWeightMap.at(0).second;
		oss << "\t" << expectedProb(weight(_valueToWeightMap.at(0).first));

		oss << "\t\t" << _valueToWeightMap.at(0).second;
		oss << "\t" << expectedProb(weight(_valueToWeightMap.at(0).first)) << "\n";
		
		for (unsigned i = 1; i < _valueToWeightMap.size(); i++)
		{
			// Non-cumulative part
			oss << _valueToWeightMap.at(i).first << ": " << ((_valueToWeightMap.at(i).second) - (_valueToWeightMap.at(i-1).second));
			oss << "\t" << expectedProb(weight(_valueToWeightMap.at(i).first));

			// cumulative part
			oss << "\t\t" << _valueToWeightMap.at(i).second;
			oss << "\t" << expectedProb(_valueToWeightMap.at(i).second) << "\n";
		}

		return oss.str();
	}
};

#endif