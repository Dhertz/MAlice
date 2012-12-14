#ifndef LABEL
#define LABEL

#include <string>

class Label {
	int id;
public:
	static int nextUnusedLabel;
	Label();
	std::string getLabel();
};

#endif