#ifndef LABEL
#define LABEL

#include <string>

class Label {
	static int nextUnusedLabel;
	int id;
public:
	Label();
	std::string getLabel();
};

#endif