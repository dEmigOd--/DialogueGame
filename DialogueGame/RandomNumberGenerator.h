#pragma once

#include <random>

template<typename Type = int>
class IRNG
{
public:
	virtual Type get(Type from, Type to) = 0;
};

//from http://stackoverflow.com/questions/33420781/random-number-generators-from-c-random-library-in-realistic-programs?rq=1
/**
* (P)seudo (R)andom (N)umber (G)enerator
*/
template<typename Type = int>
class PRNG : public IRNG<Type>
{
	// easier to use param_type
	using param_type = typename std::uniform_int_distribution<Type>::param_type;

	// store an instance of the generator/distribution in the class object
	std::mt19937 gen;
	std::uniform_int_distribution<Type> dis;

public:
	// seed generator when creating
	PRNG() : gen(std::random_device()()) {}

	Type get(Type from, Type to)
	{
		// only need to create a light weigt param_type each time
		return dis(gen, param_type{ from, to });
	}
};
