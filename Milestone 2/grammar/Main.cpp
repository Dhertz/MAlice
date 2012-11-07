
#include "MAliceLexer.hpp"
#include "MAliceParser.hpp"
#include <iostream>;

using namespace MAlice;

int main() {
	std::cout << "Starting parse bro." << std::endl;
	ANTLR_UINT8* fName = (ANTLR_UINT8*) "../../malice_examples/valid/test01.alice";

	MAliceLexer::InputStreamType input(fName, ANTLR_ENC_8BIT);
	MAliceLexer lexer(&input);

	MAliceParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lexer.get_tokSource());
	MAliceParser parser(&tstream);

	parser.program();
	std::cout << "Done, yo." << std::endl;

	return 0;
}
