#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <boost/shared_ptr.hpp>
#include <string>

using namespace std;

class Identifier {
	public:
		virtual string getBaseName() = 0;
};

#endif
