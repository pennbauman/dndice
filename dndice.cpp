/* Penn Bauman
pennbauman@protonmail.com
*/
#define VERSION 0.1

#include <iostream>
#include <string>
#include <stdlib.h>
#include <random>

#include "node.h"
using namespace std;

bool isNum(string s) {
	//if ((s[0] == '-') || (isdigit(s[0]))) {
		for (int i = 1; i < s.size(); i++) {
			if (!isdigit(s.at(i)))
				return false;
		}
		return true;
	//}
	//return false;
}

void printHelp() {
	cout << "help" << endl;
}

node* parse(string s) {
	//cout << "parse: '" << s << "'" << endl;
	node* fin = new node;
	for (int i = 1; i < s.size(); i++) {
		if (s[i] == '+') {
			//cout << s.substr(0, i) << " + " << s.substr(i+1) << endl;
			fin->data = "+";
			fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			return fin;
		} else if (s[i] == '-') {
			//cout << s.substr(0, i) << " - " << s.substr(i) << endl;
			fin->data = "-";
			fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			return fin;
		}
	}
	for (int i = 1; i < s.size(); i++) {
		if ((s[i] == '*') || (s[i] == 'x')) {
			//cout << s.substr(0, i) << " * " << s.substr(i+1) << endl;
			fin->data = "*";
			fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			return fin;
		}
	}
	for (int i = 0; i < s.size(); i++) {
		if ((s[i] == 'd') || (s[i] == 'D')) {
			//cout << s.substr(0, i) << "d" << s.substr(i+1) << endl;
			fin->data = "d";
			if (i == 0)
				fin->left = parse("1");
			else
				fin->left = parse(s.substr(0,i));
			fin->right = parse(s.substr(i+1));
			return fin;
		}
	}
	if (s == "%") {
		fin->data = "100";
		fin->left = NULL;
		fin->right = NULL;
		return fin;
	}
	if (isNum(s)) {
		fin->data = s;
		fin->left = NULL;
		fin->right = NULL;
		return fin;
	} else {
		cout << "ERROR: Syntax '" << s << "'" << endl;
	}
	return NULL;
}

void printTree(node* root) {
	if (root->left != NULL)
		printTree(root->left);
	if (root->data == "+") {
		cout << " + ";
	} else if (root->data == "-") {
		cout << " - ";
	} else if (root->data == "*") {
		cout << " x ";
	} else
		cout << root->data;
	if (root->right != NULL)
		printTree(root->right);
}

int rollDice(node* root) {
	if (root->data == "+")
		return rollDice(root->left) + rollDice(root->right);
	if (root->data == "-")
		return rollDice(root->left) - rollDice(root->right);
	if (root->data == "*")
		return rollDice(root->left) * rollDice(root->right);
	if (root->data == "d") {
		random_device device;
		default_random_engine random(device()^(long)root);
		int num = rollDice(root->left);
		int size = rollDice(root->right);
		int fin = 0;
		int temp;
		for (int i = 0; i < num; i++) {
			temp = random() % size + 1;
			cout << temp << " ";
			fin += temp;
		}
		return fin;
	}
	return atoi(root->data.c_str());
}

int main(int argc, char* argv[]) {
	string source, current;
	for (int i = 1; i < argc; i++) {
		current = argv[i];
		if (current == "--version") {
			cout << "DnDice version " << VERSION << endl;
			return 0;
		} else if ((current == "--help") || (current == "-h")) {
			printHelp();
			return 0;
		} else {
			source += current;
		}
	}
	//cout << "parse" << endl << source << endl;
	if ((source[0] == '-') || (source[0] == '+'))
		source = "1d20" + source;
	for (int i = 0; i < source.size(); i++) {
		if (source[i] == ' ') {
			source = source.substr(0,i) + source.substr(i+1);
			i--;
		}
	}
	node* tree = parse(source);
	printTree(tree);
	//cout << endl;
	cout << " | ";
	//cout << tree << endl;

	//srand (time(NULL)^(long)tree);
	int result = rollDice(tree);
	cout << endl << "result: " << result << endl;


	return 0;
}
