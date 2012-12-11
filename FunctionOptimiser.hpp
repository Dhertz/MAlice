#ifndef FUNCTION_OPTIMISER
#define FUNCTION_OPTIMISER

#include "AssemFunc.hpp"
#include <boost/shared_ptr.hpp>
#include <map>

class FunctionOptimiser {

public:
	static void optimise(boost::shared_ptr<AssemFunc>& f, 
						   std::map<string, int>& calls,
						   std::map<int, int> duplicateLabels);

};

#endif