#ifndef _ITERABLE_DISCRETE_DISTRIBUTION_GUARD
#define _ITERABLE_DISCRETE_DISTRIBUTION_GUARD

#include <utility>
#include <vector>

#include "DiscreteDistribution.hpp"
#include "Types.h"

//
// Map-based distribution <KEY, WEIGHT>
//     where the weight is cumulative (allowing random number generation)
//     to return the key value
//

template<typename KEY_T>
class IterableDiscreteDistribution : public DiscreteDistribution<KEY_T>
{
public:
	
	Iterator begin ()
	{
		return Iterator(_valueToWeightMap.begin());
	}

	Iterator end ()
	{
		return Iterator(_valueToWeightMap.end());
	}
	
	// constructor for IDD (simply calls DD constructor)
	IterableDiscreteDistribution()
	{
		DiscreteDistribution<KEY_T>();
	}

	// Standard Iteration mechanism using vector iterators
	class Iterator
	{
	private:
		// instance variable to maintain the current position in iteration
		typename std::vector<std::pair<KEY_T, WEIGHT_T> >::iterator _current;
	
	public:

		// Create an empty Iterator
		Iterator(typename std::vector<std::pair<KEY_T, WEIGHT_T> >::iterator it) : _current(it) { }

		KEY_T& operator*()
		{
			return *_current.first;
		}

		const Iterator& operator=(typename const std::vector<std::pair<KEY_T, WEIGHT_T> >::iterator& that)
		{
			this->_current = that._current;
		}

		friend class IterableDiscreteDistribution;
	};

};

#endif