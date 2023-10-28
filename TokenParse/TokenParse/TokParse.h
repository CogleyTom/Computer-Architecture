#ifndef TOKPARSE_H
#define TOKPARSE_H

#include <cstring>
using std::strtok;
#include <vector>
using std::vector;
#include <string>
using std::string;

class tokParse {
	
public:
	tokParse(string s);

	void setStringASM(string s);
	char* getStringASM();

	char* stringToCharP(string s);
	char* removeComma();
	vector<string> doToken();
	bool jumpCheck();
	void parseFirstElement();


private:
	char* strASM;
	vector<string> tokASM;
	vector<int> binaryASM;
	bool jumpBool;
};

#endif
