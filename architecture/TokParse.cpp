#include "TokParse.h"
#include "instructExec.h"
#include <algorithm>
using std::find;
#include <iostream>
using std::endl;
#include <sstream>
#include <cstring>
using std::strtok;
#include <vector>
using std::vector;
#include <string>
using std::string;
using std::getline;
#include <iterator>
using std::stringstream;
#include <istream>
#include <iomanip>
using std::setfill;
using std::setw;
using std::stoi;


//Constructor - also stores the string as a char* in strASM for tokenizing and removes all the commas
tokParse::tokParse(string s) {
	strASM = stringToCharP(s);
	removeComma();
}

tokParse::tokParse() { //Temp for creating the ins vector<vector<string>>
	strASM = stringToCharP("just a test");
}

//Setter
char* tokParse::setStringASM(string s) {
	int length = s.length();

	char* char_array = new char[length + 1];

	strcpy(char_array, s.c_str());
	return char_array;
}

//Getter
char* tokParse::getStringASM() {
	return strASM;
}

//Makes a string into a char* - tried to use the std library but it gave me hassles. 
char* tokParse::stringToCharP(string s) {
	int length = s.length();

	char* char_array = new char[length + 1];

	strcpy(char_array, s.c_str());
	return char_array;
}

//Tokenizer that removes the commas
char* tokParse::removeComma() {
	string temp = strASM;	//Creating a new copy of temp to alter

	//Long embedded function that says erase anything in temp starting with begin, ending with end, and that contains ','.
	temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
	
	strASM = stringToCharP(temp);	//Store temp back into the private data member
}

string tokParse::b10ToBinary(int b10) {

	int num = b10;
	int remainder = NULL;
	string binary{};

	while (num > 0) {
		if (num % 2 == 0) {
			binary.insert(binary.begin(), '0');
		}
		else
		{
			binary.insert(binary.begin(), '1');
		}
		num >>= 1;
	}
	return binary;
}

//Tokenizes the rest of the string
vector<string> tokParse::doToken() {

	vector<string> v;	//Create a new vector<string>
	string str = strASM; //Create another copy of strASM
	stringstream ss(str); //String buffer ss that contains str
	string s; // New string used to write to. 
	while (getline(ss,s,' ')) { //Get from the buffer ss, write to s, and make a new line at every space.
		v.push_back(s); //Once we run into a space, store that string in s
	}

	tokASM = v; //Store v in tokASM private member (pretty janky as well)

	return v;	//We do nothing with this return yet.
}

//Checks if the program is a jump and then updates the bool private data member
bool tokParse::jumpCheck() {

	for (const auto& str : tokASM) {	// for the entire size of tokASM, but each element in str
		if (str.find('_') != std::string::npos) { //If the element string contains a '_', execute the brackets
			jumpBool = true;
			return true;	//return true to exit the function. not used yet.
		}
	}	
	jumpBool = false;
	return false;
}

vector<string> tokParse::parseInstruction() {

	insToken tempToken;

	auto stripR = [](string in) {
		string out;

		for (int i = 1; i < in.length(); i++) {
			out.push_back(in.at(i));
		}
		return out;
		};

	if (tokASM.empty() == false) {
		if (tokASM[0].compare("MOV") == 0) {
			tempToken.opCode = "1101"; //OPCode for Data Processing Instruction Format
			tempToken.destReg = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[1]))), 4);
			tempToken.operand2 = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[2]))), 12);
			tempToken.firstOpReg = "0000";
			tempToken.immOperand = "1";
		}
		else if (tokASM[0].compare("B") == 0) {
			tempToken.opCode = "B"; //Should create Branch Instruction Format
			tempToken.offset = getPaddedBinaryNum(b10ToBinary(stoi(tokASM[1])), 24);
			tempToken.linkBit = "0";
		}
		else if (tokASM[0].compare("BEQ") == 0) {
			tempToken.opCode = "BEQ"; //Should create Branch Instruction Format
			tempToken.offset = getPaddedBinaryNum(b10ToBinary(stoi(tokASM[1])), 24);
			tempToken.linkBit = "1";
		}
		else if (tokASM[0].compare("ADDR") == 0) {
			tempToken.opCode = "0100"; //OPCode for Data Processing Instruction Format
			tempToken.destReg = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[1]))), 4);
			tempToken.firstOpReg = tempToken.destReg;
			tempToken.operand2 = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[2]))), 12);
			tempToken.immOperand = "0";
		}
		else if (tokASM[0].compare("ADDI") == 0) {
			tempToken.opCode = "0100"; //OPCode for Data Processing Instruction Format
			tempToken.destReg = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[1]))), 4);
			tempToken.firstOpReg = tempToken.destReg;
			tempToken.operand2 = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[2]))), 12);
			tempToken.immOperand = "1";
		}
		else if (tokASM[0].compare("SWI") == 0) {
			tempToken.opCode = "1111"; //I believe this means exit the program
		}
		else if (tokASM[0].compare("CMP") == 0) {
			tempToken.opCode = "1010"; //OPCode for Data Processing Instruction Format 
			tempToken.firstOpReg = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[1]))), 4);
			tempToken.operand2 = getPaddedBinaryNum(b10ToBinary(stoi(stripR(tokASM[2]))), 12);
			tempToken.destReg = "0000";
			tempToken.immOperand = "0";
		}
		else {
			tempToken.opCode = "0000";		//If no matches then do this. 
		}
	}
	return makeTempIns(tempToken);
}

string tokParse::getPaddedBinaryNum(string binNum, int width) {
	stringstream ss;
	ss << std::setfill('0') << std::setw(width);
	ss << binNum;
	return ss.str();
}


vector<string> tokParse::makeTempIns(insToken tok) {
	vector<string> newIns;

	if (tok.opCode == "B" || tok.opCode == "BEQ") {
		newIns.push_back(tok.offset);
		newIns.push_back(tok.linkBit);
		newIns.push_back("101");
		newIns.push_back("0000");
	}
	else if (tok.opCode == "1111") {
		// SWI
		newIns.push_back("00000000000000000000000");
		newIns.push_back(tok.opCode);
		newIns.push_back("0000");
	}
	else {
		newIns.push_back(tok.operand2); //3) Take immediate value
		newIns.push_back(tok.destReg);			//5) and store it in this register
		newIns.push_back(tok.firstOpReg);		//4) Add it with whatever is in the register
		newIns.push_back(tok.s);
		newIns.push_back(tok.opCode);		//1) ADD opcode
		newIns.push_back(tok.immOperand);		//2) Immediate value
		newIns.push_back("00");
		newIns.push_back("0000");
	}
	return newIns;
}