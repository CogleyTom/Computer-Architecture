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
	struct insToken {
		string immOperand;
		string opCode;
		string s = "0";
		string firstOpReg;
		string destReg;
		string operand2;
		string linkBit;
		string offset;
	};

	tokParse(string s);
	tokParse();

	char* setStringASM(string s);
	char* getStringASM();

	char* stringToCharP(string s);
	char* removeComma();
	string b10ToBinary(int b10);

	vector<string> doToken();
	bool jumpCheck();
	vector<string> parseInstruction();

	string getPaddedBinaryNum(string binNum, int width);

	vector<string> makeTempIns(insToken tok);

private:
	char* strASM;
	vector<string> tokASM;
	vector<int> binaryASM;
	bool jumpBool;
};

#endif
