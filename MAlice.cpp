#include "idents/Letter.hpp"
#include "idents/Number.hpp"
#include "idents/Sentence.hpp"
#include "TreeWalker.hpp"
#include "Utils.hpp"
#include "MAliceLexer.h"
#include "MAliceParser.h"
#include "InstructionPrinter.hpp"
#include "Label.hpp"
#include <boost/lexical_cast.hpp>
#include "Optimiser.hpp"

using namespace std;

void printError(string message) {
    cerr << message << endl << endl;
    exit(1);
}

void initST(boost::shared_ptr<SymbolTable> top) {
    boost::shared_ptr<Number> numberTypeDef(new Number());
    top->add("number", numberTypeDef);

    boost::shared_ptr<Sentence> sentenceTypeDef(new Sentence());
    top->add("sentence", sentenceTypeDef);

    boost::shared_ptr<Letter> letterTypeDef(new Letter());
    top->add("letter", letterTypeDef);
}

void parseFile(pANTLR3_UINT8 filename, bool doPrintTree) {
    cout << endl << "Parsing File " << filename << "..." << endl << endl;

    // Set up our Lexer and Parser
    pANTLR3_INPUT_STREAM input;
    pMAliceLexer lex;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    pMAliceParser parser;

    // Check to make sure there were no issues creating any of the structs we 
    // need
    input = antlr3AsciiFileStreamNew(filename);
    if (input == NULL)
        printError("Unable to open file.");

    lex = MAliceLexerNew(input);
    if (lex == NULL)
        printError("Unable to create lexer.");

    tokens = 
    	antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
    if (tokens == NULL)
        printError("Unable to create tokenstream.");

    parser = MAliceParserNew(tokens);
    if (parser == NULL)
        printError("Unable to create parser.");

    // Parse the program from the top level rule (named program)
    MAliceParser_program_return r = parser->program(parser);

    // Create the parse tree
    pANTLR3_BASE_TREE tree = r.tree;

    // Check for syntax errors before continuing
    if (parser->pParser->rec->getNumberOfSyntaxErrors(parser->pParser->rec) 
    		> 0) {
        cerr << "Syntax errors found. Stopping." << endl;
        exit(1);
    }

    // A nice debug tool
    if (doPrintTree)
        Utils::printTree(tree);

    // Create the top level (static) symbol table with type info
    boost::shared_ptr<SymbolTable> top(
    	new SymbolTable(boost::shared_ptr<SymbolTable>()));
    initST(top);

    // Create the AST
    boost::shared_ptr<AST> semanticTree(new AST());

    // Build the AST doing semantic checks along the way
    TreeWalker walker(top, tree, semanticTree);

    // Stop if there are any semantic errors
    if (Utils::globalErrorCount > 0) {
        cerr << "Errors found. Stopping." << endl;
        Utils::globalErrorCount = 0;
        exit(1);
    }

    if (boost::shared_ptr<SymbolTable> globalSt 
    										= top->getChildren()[0].lock()) {
    	// Visit the AST to generate code
    	boost::shared_ptr<ASTVisitor> treeVisitor(new ASTVisitor(globalSt));
	    semanticTree->getRoot()->accept(treeVisitor);

	    // Run the code through the optimiser and print to file
	    Optimiser opt(treeVisitor, boost::lexical_cast<string>(filename));
    }

    // Reset the label count for the next file if there are any
    Label::nextUnusedLabel = 0;

    parser->free(parser);
    tokens->free(tokens);
    lex->free(lex);
    input->close(input);

    cout << "Done." << endl;
}

/*
	The main entry point for the program. Loop through all the arguments given
	and attempt to parse each one.
*/
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        parseFile((pANTLR3_UINT8) argv[i], false);
        cout << endl;
    }

    return 0;
}
