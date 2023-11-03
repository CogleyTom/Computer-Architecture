#ifndef INSTRUCTEXEC_H
#define INSTRUCTEXEC_H

#include<string>
using std::string;
#include<vector>
using std::vector;
#include "Registers.h"
#include "Stack.h"
#include "Architecture.h"
#include "TokParse.h"


class instructExec {

public: 
	instructExec(architecture ARM);
	instructExec(vector<vector<string>> allBinaryIns);

	void updateIns(vector<string> binaryIns);

	void setBinary(vector<string>);
	vector<string> getBinary();
	string getBinaryStr();
	void setInstructFormat();

	int binaryToB10(string binaryIns);

	void determineExec();

	void moveProgramCounter(int indexToMove, bool jump);
  void doNextInstruction();

	void addExec();
	void movExec();
	void ldrExec();
	void beqExec();
	void bExec();
	void cmpExec();
	void swiExec();

	void updateConsole();
	void displayBinary();

  architecture getArch();
  bool getProgramComplete();

private:
	architecture persistARM;
	vector<string> currentIns;
	vector<vector<string>> allIns;
	int instructFormat;
	bool compareResults;
	int programCounter;
  bool programComplete = false;
};




#endif