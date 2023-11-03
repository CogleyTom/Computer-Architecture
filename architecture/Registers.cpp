
#include "Registers.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;


	registerCPU::registerCPU(int regName, int regValue, int byteSize, int index)
	{
		_regName = regName;
		_regValue = regValue;
		_regByteSize = byteSize;
		_regIndex = index;
	}

	int registerCPU::getRegisterName() { return _regName; }
	int registerCPU::getRegValue() { return _regValue; }
	int registerCPU::getRegIndex() { return _regIndex; }
	int registerCPU::getRegByteSize() { return _regByteSize; }

	void registerCPU::setRegValue(int value) { _regValue = value; }
	void registerCPU::setRegByteSize(int bSize) { _regByteSize = bSize; }

