#include "Label.hpp"

int Label::nextUnusedLabel = 0;

Label::Label() {
	id = nextUnusedLabel;
	nextUnusedLabel++;
}

std::string Label::getLabel() {
	return ".L" + id;
}
