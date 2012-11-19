#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <boost/shared_ptr.hpp>
#include <string>

using namespace std;

class Identifier {
	public:
		virtual string getID() = 0;
};

#endif
