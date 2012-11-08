#include "MAliceLexer.h"
#include "MAliceParser.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

void print(pANTLR3_BASE_TREE tree, int level) {
	for (int i = 0; i < level; ++i) {
		cout << "--";
	}

	cout << " " << tree->getText(tree)->chars << endl;

	int childCount = tree->getChildCount(tree);
	for (int i = 0; i < childCount; ++i) {
		print((pANTLR3_BASE_TREE) tree->getChild(tree, i), level + 1);
	}
}

void printTree(pANTLR3_BASE_TREE ast) {
	print(ast, 0);
}

int main(int argc, char* argv[]) {
	pANTLR3_INPUT_STREAM input;
	pMAliceLexer lex;
	pANTLR3_COMMON_TOKEN_STREAM tokens;
	pMAliceParser parser;

	assert(argc > 1);
	input = antlr3AsciiFileStreamNew((pANTLR3_UINT8) argv[1]);
	lex = MAliceLexerNew(input);
	tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	parser = MAliceParserNew(tokens);

	MAliceParser_program_return r = parser->program(parser);
	pANTLR3_BASE_TREE tree = r.tree;

	printTree(tree);

	parser->free(parser);
	tokens->free(tokens);
	lex->free(lex);
	input->close(input);

	return 0;
}
