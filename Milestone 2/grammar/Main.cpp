#include "MAliceLexer.hpp"
#include "MAliceParser.hpp"
#include <iostream>
#include <dirent.h>

using namespace MAlice;
using namespace std;

// So I can put main at the top:
static void processDir(const char* dir);

int main(int argc, char* argv[]) {
	cout << "Parsing..." << endl << endl;

	processDir("../../malice_examples/valid/");

	cout << "Done" << endl;

	return 0;
}

static void parseFile(ANTLR_UINT8* fName) {
	cout << "Parsing file " << fName << endl;

	MAliceLexer::InputStreamType input(fName, ANTLR_ENC_8BIT);
	MAliceLexer lexer(&input);

	MAliceParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lexer.get_tokSource());
	MAliceParser parser(&tstream);

	parser.program();

	cout << "Done file " << fName << endl;
}

static void processDir(const char* dir) {
	DIR* Hdir;
	DIR* Hdirs;
	struct dirent* dirfill;

	Hdir = opendir(dir);

	if (Hdir == NULL) {
		// It's a file
		parseFile((ANTLR_UINT8*) dir);
	} else {
		cout << "Parsing directory " << dir << endl;

		do {
			std::string temp;
			dirfill = readdir(Hdir);

			if(dirfill != NULL && dirfill->d_name[0] != '.') {
				temp = dirfill->d_name;
				Hdirs = opendir(temp.c_str());

				if (Hdirs != NULL) {
					// Another directory
					processDir(temp.c_str());
					closedir(Hdirs);
				} else {
					// A file to parse
					char cat[strlen(dir) + temp.size()];
					strcpy(cat, dir);
					strcat(cat, temp.c_str());
					parseFile((ANTLR_UINT8*) cat);
				}
			}
		} while (dirfill != NULL);

		cout << "Done directory " << dir << endl << endl;

		closedir(Hdir);
	}
}
