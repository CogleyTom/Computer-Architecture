#include "TokParse.h"
#include <algorithm>
using std::find;
#include <iostream>
using std::endl;
#include <sstream>
using std::cout;
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


//Constructor - also stores the string as a char* in strASM for tokenizing and removes all the commas
tokParse::tokParse(string s) {
	strASM = stringToCharP(s);
	removeComma();
}

//Setter
void tokParse::setStringASM(string s) {
	int length = s.length();

	char* char_array = new char[length + 1];

	strcpy(char_array, s.c_str());
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
	
	strASM = stringToCharP(temp);	//Store temp back into the private data member (pretty ganky)
}

//Tokenizes the rest of the string
vector<string> tokParse::doToken() {

	vector<string> v;	//Create a new vector<string>
	string str = strASM; //Create another compy of strASM
	stringstream ss(str); //String buffer ss that contains str
	string s; // New string used to write to. 
	while (getline(ss,s,' ')) { //Get from the buffer ss, write to s, and make a new line at every space.
		v.push_back(s); //Once we run into a space, store that string in s
	}

	tokASM = v; //Store v in tokASM private member (pretty janky as well)

	for (int i = 0; i < tokASM.size(); i++) {	//For loop that prints to the console. Just for testing
		cout << tokASM[i] << endl;
	}
	return v;	//We do nothing with this return yet.
}

//Checks if the program is a jump and then updates the bool private data member
bool tokParse::jumpCheck() {

	for (const auto& str : tokASM) {	// for the entire size of tokASM, but each element in str
		if (str.find('_') != std::string::npos) { //If the element string contains a '_', execute the brackets
			cout << "Has a jump\n";		
			jumpBool = true;
			return true;	//return true to exit the function. not used yet.
		}
	}	
	cout << "Has no jump\n"; //Else if no element in the vector<string> has a '_', execute this
	jumpBool = false;
	return false;
}

void tokParse::parseFirstElement() {

	vector<int> tempBinary;

	if (tokASM.empty() == false) {
		if (tokASM[0].compare("MOV") == 0) {
			tempBinary.push_back(1101); //OPCode for Data Processing Instruction Format
		}
		else if (tokASM[0].compare("LDR") == 0) {
			tempBinary.push_back(49); //Made up myself - will signal to create a single data structure instruction set. Will need a junction that checks if binary or not.
			//Number corresponds to the section in the ARM Instruction Set that has these format.
		}
		else if (tokASM[0].compare("B") == 0) {
			tempBinary.push_back(44);
		}
		else if (tokASM[0].compare("BEQ") == 0) {
			tempBinary.push_back(44); //Should create Branch Instruction Format
		}
		else if (tokASM[0].compare("ADD") == 0) {
			tempBinary.push_back(0100); //OPCode for Data Processing Instruction Format
		}
		else if (tokASM[0].compare("SWI") == 0) {
			tempBinary.push_back(1111); //I believe this means exit the program?? Unclear
		}
		else if (tokASM[0].compare("CMP") == 0) {
			tempBinary.push_back(1010); //OPCode for Data Processing Instruction Format 
		}
		else {
			tempBinary.push_back(0000);		//If no matches then do this. 
		}

		cout << tempBinary[0] << endl << endl;
	}
}