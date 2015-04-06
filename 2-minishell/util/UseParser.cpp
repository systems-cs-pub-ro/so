#include <iostream>
#include <string>
#include <cstdlib>
#include "parser.h"

#ifdef UNICODE
#  error "Unicode not supported in this source file!"
#endif

#define PROMPT_STRING	"> "


using namespace std;


void parse_error(const char * str, const int where)
{
	cerr << "Parse error near " << where << ": " << str << endl;
}


int main()
{
	cout << PROMPT_STRING;

	string line;
	getline(cin, line);
	if ((line.length() == 0) && !cin.good()) {
		// end of file reached
		cerr << "End of file!" << endl;
		return EXIT_SUCCESS;
	}

	command_t* root = NULL;
	if (parse_line(line.c_str(), &root)) {
		cout << "Command successfully read!" << endl;
		if (root == NULL) {
			cout << "Command is empty!" << endl;
		}
		else {
			//root points to a valid command tree that we can use
		}
	}
	else {
		//there was an error parsing the command
	}
	free_parse_memory();

	return EXIT_SUCCESS;
}
