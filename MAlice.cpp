#include "idents/Letter.hpp"
#include "idents/Number.hpp"
#include "idents/Sentence.hpp"
#include "TreeWalker.hpp"
#include "Utils.hpp"
#include "MAliceLexer.h"
#include "MAliceParser.h"
#include "InstructionPrinter.hpp"

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

    pANTLR3_INPUT_STREAM input;
    pMAliceLexer lex;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    pMAliceParser parser;

    input = antlr3AsciiFileStreamNew(filename);
    if (input == NULL)
        printError("Unable to open file.");

    lex = MAliceLexerNew(input);
    if (lex == NULL)
        printError("Unable to create lexer.");

    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
    if (tokens == NULL)
        printError("Unable to create tokenstream.");

    parser = MAliceParserNew(tokens);
    if (parser == NULL)
        printError("Unable to create parser.");

    MAliceParser_program_return r = parser->program(parser);
    pANTLR3_BASE_TREE tree = r.tree;

    if (parser->pParser->rec->getNumberOfSyntaxErrors(parser->pParser->rec) > 0) {
        cerr << "Syntax errors found. Stopping." << endl;
        return;
    }

    if (doPrintTree)
        Utils::printTree(tree);

    boost::shared_ptr<SymbolTable> top(new SymbolTable(boost::shared_ptr<SymbolTable>()));
    initST(top);

    boost::shared_ptr<AST> semanticTree(new AST());

    TreeWalker walker(top, tree, semanticTree);

    if (Utils::globalErrorCount > 0) {
        cerr << "Errors found. Stopping." << endl;
        return;
    }

    boost::shared_ptr<ASTVisitor> treeVisitor(new ASTVisitor(top));
    semanticTree->getRoot()->accept(treeVisitor);

	InstructionPrinter::printList(treeVisitor->getInstrs());

    parser->free(parser);
    tokens->free(tokens);
    lex->free(lex);
    input->close(input);

    cout << endl << "Done." << endl;
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        parseFile((pANTLR3_UINT8) argv[i], false);
        cout << endl;
    }

    return 0;
}
