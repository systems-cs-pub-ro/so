#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cassert>
#include "parser.h"

#ifdef UNICODE
#  error "Unicode not supported in this source file!"
#endif

using namespace std;

const int indent = 4;


void parse_error(const char * str, const int where)
{
	fprintf(stderr, "Parse error near %d: %s\n", where, str);
}

static void displayWord(word_t * w)
{
	assert(w != NULL);
	word_t * crt = w;

	while (crt != NULL) {
		if (crt->expand)
			cout << "expand(";
		cout << "'" << crt->string << "'";
		if (crt->expand)
			cout << ")";

		crt = crt->next_part;
		if (crt != NULL) {
			cout << ";    ";
			assert(crt->next_word == NULL);
		}
	}

	cout << endl;
}


static void displayList(word_t * w, int level)
{
	assert(w != NULL);
	word_t * crt = w;

	while (crt != NULL) {
		cout << setw(2 * indent * level) << "";
		displayWord(crt);
		crt = crt->next_word;
	}
}


static void displaySimple(simple_command_t * s, int level, command_t * father)
{
	assert(s != NULL);
	assert(father == s->up);

	cout << setw(2 * indent * level) << "" << "simple_command_t (" << endl;

	cout << setw(2 * indent * level + indent) << "" << "verb (" << endl;
	displayList(s->verb, level + 1);
	assert(s->verb->next_word == NULL);
	cout << setw(2 * indent * level + indent) << "" << ")" << endl;

	if (s->params != NULL) {
		cout << setw(2 * indent * level + indent) << "" << "params (" << endl;
		displayList(s->params, level + 1);
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
	}

	if (s->in != NULL) {
		cout << setw(2 * indent * level + indent) << "" << "in (" << endl;
		displayList(s->in, level + 1);
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
	}

	if (s->out != NULL) {
		cout << setw(2 * indent * level + indent) << "" << "out (" << endl;
		displayList(s->out, level + 1);
		if (s->io_flags & IO_OUT_APPEND)
			cout << setw(2 * indent * (level+1)) << "" << "APPEND" << endl;
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
	}

	if (s->err != NULL) {
		cout << setw(2 * indent * level + indent) << "" << "err (" << endl;
		displayList(s->err, level + 1);
		if (s->io_flags & IO_ERR_APPEND)
			cout << setw(2 * indent * (level+1)) << "" << "APPEND" << endl;
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
	}

	cout << setw(2 * indent * level) << "" << ")" << endl;
}


static void displayCommand(command_t * c, int level, command_t * father)
{
	assert(c != NULL);
	assert(c->up == father);

	cout << setw(2 * indent * level) << "" << "command_t (" << endl;
	if (c->op == OP_NONE) {
		assert(c->cmd1 == NULL);
		assert(c->cmd2 == NULL);
		cout << setw(2 * indent * level + indent) << "" << "scmd (" << endl;
		displaySimple(c->scmd, level + 1, c);
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
	}
	else {
		assert(c->scmd == NULL);
		cout << setw(2 * indent * level + indent) << "" << "op == ";

		switch (c->op) {
		case OP_SEQUENTIAL:
			cout << "OP_SEQUENTIAL";
			break;
		case OP_PARALLEL:
			cout << "OP_PARALLEL";
			break;
		case OP_CONDITIONAL_ZERO:
			cout << "OP_CONDITIONAL_ZERO";
			break;
		case OP_CONDITIONAL_NZERO:
			cout << "OP_CONDITIONAL_NZERO";
			break;
		case OP_PIPE:
			cout << "OP_PIPE";
			break;
		default:
			assert(false);
		}

		cout << endl;
		cout << setw(2 * indent * level + indent) << "" << "cmd1 (" << endl;
		displayCommand(c->cmd1, level + 1, c);
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
		cout << setw(2 * indent * level + indent) << "" << "cmd2 (" << endl;
		displayCommand(c->cmd2, level + 1, c);
		cout << setw(2 * indent * level + indent) << "" << ")" << endl;
	}

	cout << setw(2 * indent * level) << "" << ")" << endl;
}


int main(void)
{
	for (;;) {
		cout << "> ";

		string line;
		getline(cin, line);
		if ((line.length() == 0) && !cin.good()) {
			// end of file reached
			cerr << "End of file!" << endl;
			return EXIT_SUCCESS;
		}

		command_t* root = NULL;
		cout << line << endl;

		if (parse_line(line.c_str(), &root)) {
			cout << "Command successfully read!" << endl;
			if (root == NULL) {
				cout << "Command is empty!" << endl;
			}
			else {
				// root points to a valid command tree
				// that we can use
				displayCommand(root, 0, NULL);
			}
		}
		else {
			// there was an error parsing the command
			cout << "Error parsing!" << endl;
		}

		cout << endl << endl;
		free_parse_memory();
	}
}
