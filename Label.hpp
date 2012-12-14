#ifndef LABEL
#define LABEL

#include <string>

/*
	Label
	-----
	Each time this class is instantiated it holds a unique number N 
	corresponding to a unique label of the form ".LN:". This can be used to 
	avoid label conflicts in the final assembly file.
*/
class Label {
	int id;
public:
	static int nextUnusedLabel;
	Label();
	std::string getLabel();
};

#endif