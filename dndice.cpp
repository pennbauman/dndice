// Penn Bauman
// pennbauman@protonmail.com
#include <iostream>
#include <string>
#include <stdlib.h>
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

default_random_engine rands;
// Parse-tree nodes
struct node {
	string data;
	node* left;
	node* right;
};



// Print help information
void printHelp() {
	cout << "Usage: dndice [options] [dice]" << endl;
	cout << "  --version          Prints version information" << endl;
	cout << "  --help, -h         Prints this help menu" << endl;
	cout << "  --quiet, -q         Prints minimal information, only roll results" << endl;
	cout << "  -n <number>        Rolls dice <number> times instead of once" << endl;
	cout << "  --stats <method>,  Generates character ability scores, methods available:" << endl;
	cout << "  -s <method>         'std' Uses standard ability score array" << endl;
	cout << "                      'd20' Rolls 1d20 for each score" << endl;
	cout << "                      '4d6' Rolls 4d6 and sums the highest 3 for each score" << endl;
	cout << endl;
	cout << "Dice Formatting:" << endl;
	cout << "  Dice are formatted in the same style as in D&D and many other RPGs." << endl;
	cout << "  NdM means N M sided dice are rolled and their results summed." << endl;
	cout << "  Multiple set of dice and constants can be added with '+', subtracted with '-', " << endl;
	cout << "  and multiplied with '*' or 'x' placed between them." << endl;
	cout << "  +N or -N will be assumed to be 1d20+N or 1d20-N" << endl;
}

// Check if string is a valid number
bool checkNum(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (!isdigit(s.at(i))) {
			return false;
		}
	}
	return true;
}
// Recusively parse the input string into a parse-tree
node* parse(string s) {
	//cout << "parse: '" << s << "'" << endl;
	node* fin = new node;
	// Check addition and subtraction
	for (int i = 1; i < s.size(); i++) {
		if (s[i] == '+') {
			//cout << s.substr(0, i) << " + " << s.substr(i+1) << endl;
			fin->data = "+";
			fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			if ((fin->left == NULL) || (fin->right == NULL))
				return NULL;
			return fin;
		} else if (s[i] == '-') {
			//cout << s.substr(0, i) << " - " << s.substr(i) << endl;
			fin->data = "-";
			fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			if ((fin->left == NULL) || (fin->right == NULL))
				return NULL;
			return fin;
		}
	}
	// Check multiplication
	for (int i = 1; i < s.size(); i++) {
		if ((s[i] == '*') || (s[i] == 'x')) {
			//cout << s.substr(0, i) << " x " << s.substr(i+1) << endl;
			fin->data = "*";
			fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			if ((fin->left == NULL) || (fin->right == NULL))
				return NULL;
			return fin;
		}
	}
	// Check for dice operations
	for (int i = 0; i < s.size(); i++) {
		if ((s[i] == 'd') || (s[i] == 'D')) {
			//cout << s.substr(0, i) << "d" << s.substr(i+1) << endl;
			fin->data = "d";
			if (i == 0)
				fin->left = parse("1");
			else
				fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			if ((fin->left == NULL) || (fin->right == NULL))
				return NULL;
			return fin;
		}
	}
	// Check percentile dice
	if (s == "%") {
		fin->data = "100";
		fin->left = NULL;
		fin->right = NULL;
		return fin;
	}
	// Check remaining value is a number
	if (!checkNum(s)) {
		cout << "ERROR: Syntax, '" << s << "' is not interpretable" << endl;
		return NULL;
	}
	fin->data = s;
	fin->left = NULL;
	fin->right = NULL;
	return fin;
}

// Print out formatted parse-tree (looks like neat expression)
void printTree(node* root) {
	if (root == NULL)
		return;
	printTree(root->left);
	if (root->data == "+") {
		cout << " + ";
	} else if (root->data == "-") {
		cout << " - ";
	} else if (root->data == "*") {
		cout << " x ";
	} else {
		cout << root->data;
	}
	printTree(root->right);
}

// Recursively calculate outcome of dice rolls form parse-tree
int rollDice(node* root, bool loud) {
	int fin = 0;
	if (root->data == "+") {
		fin = rollDice(root->left, loud) + rollDice(root->right, loud);
	} else if (root->data == "-") {
		fin = rollDice(root->left, loud) - rollDice(root->right, loud);
	} else if (root->data == "*") {
		fin = rollDice(root->left, loud) * rollDice(root->right, loud);
	} else if (root->data == "d") {
		int num = rollDice(root->left, loud);
		int size = rollDice(root->right, loud);
		int temp;
		for (int i = 0; i < num; i++) {
			temp = rands() % size + 1;
			if (loud)
				cout << temp << " ";
			fin += temp;
		}
	} else {
		fin = atoi(root->data.c_str());
	}
	return fin;
}

// Delete tree
void deleteTree(node* root) {
	if (root == NULL)
		return
	deleteTree(root->left);
	deleteTree(root->right);
	delete root;
}

// Run program
int main(int argc, char* argv[]) {
	string source, current, stats;
	int number = 1;
	bool options[] = {
		true, // loud (--quiet, -q)
		false, // stats (--stats, -s)
	};

	// Check parameters are given
	if (argc < 2) {
		cerr << "ERROR: Requires parameters" << endl;
		cerr << endl;
		printHelp();
		return 1;
	}
	// Iterate over command line input
	for (int i = 1; i < argc; i++) {
		current = argv[i];
		if (current == "--version") {
			cout << "DnDice version 0.2" << endl;
			return 0;
		} else if ((current == "--help") || (current == "-h")) {
			printHelp();
			return 0;
		} else if ((current == "--quiet") || (current == "-q")) {
			options[0] = false;
		} else if (current == "-n") {
			i++;
			if (checkNum(argv[i])) {
				number = atoi(argv[i]);
			} else {
				cout << "ERROR: Invalid number '" << argv[i] << "'" << endl;
				return 1;
			}
		} else if ((current == "--stats") || (current == "-s")) {
			options[1] = true;
			i++;
			stats = argv[i];
		} else {
			source += current;
		}
	}

	// Setup random source
	random_device device;
	rands.seed(device());

	if (options[1]) {
		// Stats
		if (options[0])
			cout << "Stats:" << endl;
		// Standard Array
		if ((stats == "std") || (stats == "standard")) {
			cout << "15 14 13 12 10  8" << endl;
			return 0;
		}
		// 1d20
		if ((stats == "d20") || (stats == "1d20")) {
			node* d20 = parse("1d20");
			int scores[6];
			for (int i = 0; i < number; i++) {
				for (int j = 0; j < 6; j++) {
					scores[j] = rollDice(d20, false);
				}
				sort(scores, scores + 6);
				for (int j = 5; j >= 0; j--) {
					if (scores[j] < 10)
						cout << " ";
					cout << scores[j] << " ";
				}
				cout << endl;
			}
			deleteTree(d20);
			return 0;
		}
		// Highest 3 of 4d6
		if ((stats == "4d6") || (stats == "3d6")) {
			node* d6 = parse("1d6");
			int scores[6];
			int rolls[4];
			for (int i = 0; i < number; i++) {
				for (int j = 0; j < 6; j++) {
					for (int k = 0; k < 4; k++) {
						rolls[k] = rollDice(d6, false);
					}
					scores[j] = 0;
					int min = 21;
					for (int k = 0; k < 4; k++) {
						if (rolls[k] < min)
							min = rolls[k];
						scores[j] += rolls[k];
					}
					scores[j]-= min;
				}
				sort(scores, scores + 6);
				for (int j = 5; j >= 0; j--) {
					if (scores[j] < 10)
						cout << " ";
					cout << scores[j] << " ";
				}
				cout << endl;
			}
			deleteTree(d6);
			return 0;
		}
		//
		cout << "ERROR: Unknown statistics generation method '" << stats << "'" << endl;
		cout << "  Try 'std', '1d20', or '3d6'" << endl;
		cout << "  Use --help for more information" << endl;
		return 1;
	} else {
		// Check that dice have been provided
		if (source == "") {
			cout << "ERROR: Dice to roll are required" << endl;
			return 1;
		}
		// Check for ability check format
		if ((source[0] == '-') || (source[0] == '+'))
			source = "1d20" + source;
		// Remove spaces from dice expression
		for (int i = 0; i < source.size(); i++) {
			if (source[i] == ' ') {
				source = source.substr(0,i) + source.substr(i+1);
				i--;
			}
		}
		// Parse dice expression to parse-tree
		node* tree = parse(source);
		// Check for error during parsing
		if (tree == NULL)
			return 1;
		for (int i = 0; i < number; i++) {
			// Print out formatted expression if applicable
			if (options[0]) {
				printTree(tree);
				cout << " | ";
			}

			// Compute and print out dice output
			int result = rollDice(tree, options[0]);
			if (options[0])
				cout << endl << "Result: ";
			cout << result << endl;
		}
		deleteTree(tree);
	}
	return 0;
}
