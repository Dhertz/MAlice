#include "Label.hpp"
#include <iostream>
#include <sstream>

int Label::nextUnusedLabel = 0;

Label::Label() {
	id = nextUnusedLabel;
	nextUnusedLabel++;
}

std::string Label::getLabel() {
	std::string lbl(".L");
	std::ostringstream s;
	s << lbl << id;
	return s.str();
}
