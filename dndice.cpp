/* Penn Bauman
pennbauman@protonmail.com
*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <random>
using namespace std;

// Parse-tree nodes
struct node {
	string data;
	node* left;
	node* right;
};


// Print help infomation
void printHelp() {
	cout << "help" << endl;
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
	for (int i = 0; i < s.size(); i++) {
		if (!isdigit(s.at(i))) {
			cout << "ERROR: Syntax, '" << s << "' is not interpretable." << endl;
			return NULL;
		}
	}
	fin->data = s;
	fin->left = NULL;
	fin->right = NULL;
	return fin;
}

// Print out formated parse-tree (looks like neat expression)
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
		random_device device;
		default_random_engine random(device()^(long)root);
		int num = rollDice(root->left, loud);
		int size = rollDice(root->right, loud);
		int temp;
		for (int i = 0; i < num; i++) {
			temp = random() % size + 1;
			if (loud)
				cout << temp << " ";
			fin += temp;
		}
	} else {
		fin = atoi(root->data.c_str());
	}
	delete root;
	return fin;
}

// Run program
int main(int argc, char* argv[]) {
	string source, current;
	bool options[] = {
		true, // loud (--quiet, -q)
	};

	// Iterate over command line input
	for (int i = 1; i < argc; i++) {
		current = argv[i];
		if (current == "--version") {
			cout << "DnDice version 0.1" << endl;
			return 0;
		} else if ((current == "--help") || (current == "-h")) {
			printHelp();
			return 0;
		} else if ((current == "--quiet") || (current == "-q")) {
			options[0] = false;
		} else {
			source += current;
		}
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
	// Print out formated expression if applicable
	if (options[0]) {
		printTree(tree);
		cout << " | ";
	}

	// Compute and print out dice output
	int result = rollDice(tree, options[0]);
	if (options[0])
		cout << endl << "Result: ";
	cout << result << endl;

	return 0;
}
