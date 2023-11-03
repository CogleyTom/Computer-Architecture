#include "instructExec.h"
#include "Architecture.h"


#include<string>
using std::string;
#include<vector>
using std::vector;
#include<iostream>
using std::cout;
using std::stoi;
using std::endl;
#include <stdlib.h>
using std::exit;

instructExec::instructExec(architecture ARM) {
	persistARM = ARM;
}

instructExec::instructExec(vector<vector<string>> allBinaryIns) {
	allIns = allBinaryIns;
	programCounter = 0;
}

void instructExec::setBinary(vector<string> binaryIns) {
	currentIns = binaryIns;
}

vector<string> instructExec::getBinary() {
	return currentIns;
}

void instructExec::setInstructFormat() {
	instructFormat = currentIns.size();
}

void instructExec::updateIns(vector<string> newBinaryIns) { //Updates the vector and instructFormat PMV
	setBinary(newBinaryIns);
	setInstructFormat();
	determineExec();
}

int instructExec::binaryToB10(string binaryIns) {
	int result = 0;
	int i;
	int pos = -1;
	
	for (i = binaryIns.length(); i >= 0; i--) {
		if (binaryIns[i] == '1') {
			result |= 1 << pos;
		}
		pos++;
	}
	return result;
}

architecture instructExec::getArch() {
  return persistARM;
}

bool instructExec::getProgramComplete() {
  return programComplete;
}

void instructExec::determineExec() {
	
	if (instructFormat == 8) {		//another switch based on the opcode
		if (currentIns[4] == "0100") {
			addExec();
		}
		else if (currentIns[4] == "1101") {
			movExec();
		}
		else if (currentIns[4] == "1010") {
			cmpExec();
		}
	}
	if (instructFormat == 11)	{
		ldrExec();
	}
	if (instructFormat == 3) {
		swiExec();
	}
	if (instructFormat == 4) {
		if (currentIns[1] == "1") {
			beqExec();
		}
		else if (currentIns[1] == "0") {
			bExec();
		}
	}
}

void instructExec::moveProgramCounter(int indexToMove, bool jump) {
	if (jump == true) {
		programCounter = indexToMove;
		if (programCounter >= allIns.size()) {
      programComplete = true;
		}
	}
	else if (jump == false) {
		programCounter++;
		if (programCounter >= allIns.size()) {
      programComplete = true;
		}
	}
}

void instructExec::doNextInstruction() {
  updateIns(allIns[programCounter]);
}

void instructExec::addExec() {

	int Rn = -1;
	int Rd = -1;
	int Imm = stoi(currentIns[5]);
	int Operand2 = -1;
	
	if (Imm == 0) {
		Operand2 = binaryToB10(currentIns[0]);
		Operand2 = persistARM.getRegisterValue(Operand2);
		
		Rn = binaryToB10(currentIns[1]);
		Rn = persistARM.getRegisterValue(Rn);
		
		Rd = binaryToB10(currentIns[1]);

		int tempALU = Operand2 + Rn;
		
		persistARM.setRegisterValue(Rd, tempALU);
		moveProgramCounter(1, false);
	}

	else if (Imm == 1) {
		Operand2 = binaryToB10(currentIns[0]);
		
		Rn = binaryToB10(currentIns[2]);
		Rn = persistARM.getRegisterValue(Rn);
		
		Rd = binaryToB10(currentIns[1]);

		int tempALU = Operand2 + Rn;
		
		persistARM.setRegisterValue(Rd, tempALU);
		moveProgramCounter(1, false);
	}

	else
	{
		cout << "Error occurred with add - Imm value not valid\n\n";
	}
}

void instructExec::movExec() {

	int Rd = -1;
	int Operand2 = -1;

	Operand2 = binaryToB10(currentIns[0]);
	Rd = binaryToB10(currentIns[1]);

	persistARM.setRegisterValue(Rd, Operand2);
	moveProgramCounter(1, false);
}

void instructExec::ldrExec() {

	int Rd = -1;
	int Imm = -1;
	int Operand2 = -1;

	Imm = binaryToB10(currentIns[8]);

	if (Imm == 0) {   //Immediate value
		Operand2 = binaryToB10(currentIns[0]);
		Rd = binaryToB10(currentIns[1]);
		persistARM.setRegisterValue(Rd, Operand2);
		moveProgramCounter(1, false);
	}
}

void instructExec::cmpExec() { //Operand2 is always a register
	
	int Rn = -1;
	int Operand2 = -1;

	Operand2 = binaryToB10(currentIns[0]);
	Operand2 = persistARM.getRegisterValue(Operand2);

	Rn = binaryToB10(currentIns[2]);
	Rn = persistARM.getRegisterValue(Rn);

	if (Operand2 != Rn) {
		currentIns[7] = "0001"; //Means they are equal
		compareResults = 0;
		moveProgramCounter(1, false);
	}
	else if (Operand2 == Rn)
	{
		compareResults = 1;
		moveProgramCounter(1, false);
	}
}

void instructExec::beqExec() { 

	if (compareResults == false) {
		int tempValue = binaryToB10(currentIns[0]);
		compareResults = 0;
		moveProgramCounter(1, false);
	}

	else if (compareResults == true) {
		compareResults = 0;
		int tempValue = binaryToB10(currentIns[0]);
		moveProgramCounter(tempValue, true);
	}
}

void instructExec::bExec() { 
	
	int tempValue = binaryToB10(currentIns[0]);
	moveProgramCounter(tempValue, true);
}

void instructExec::swiExec() {
    //swi means program complete
    programComplete = true;
}


void instructExec::updateConsole() {
	persistARM.displayRegArch();
	cout << endl << "DISPLAYING STACK BELOW" << endl;
	persistARM.displayStackArch();
}

void instructExec::displayBinary() {
	cout << endl << endl << "Binary to be executed: ";
	int temp = currentIns.size() - 1;
	for (int i = temp; i >= 0; i--) {
		cout << currentIns[i];
	}
	cout << endl;
}

string instructExec::getBinaryStr() {
  string ret = "Instruction " + std::to_string(programCounter) + ":\n";
	int temp = currentIns.size() - 1;
	for (int i = temp; i >= 0; i--) {
		ret += currentIns[i];
	}
  return ret;
}
