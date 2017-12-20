#pragma once

#include <string>
#include <vector>
#include <stack>
#include "calculator.h"
#include <fstream>
#include <streambuf>

#include <boost/algorithm/string.hpp>

class Interpretator
{
	std::string expression;
	calculator<std::string::iterator> calc;

	std::vector< std::pair< std::string, std::string> > vector;
	skip_grammar<std::string::iterator> sk;
	std::vector< std::pair< std::string, bool> > vars;

	std::string str;
	//std::string opers = "new&static&const&if&while";

	bool checkOper();
	bool checkVars(std::string, bool);

	bool analysLeft(std::string);
	bool analysRight(std::string);
public:
	enum ExecType
	{
		ET_STRING = 0,
		ET_FILE
	};

	ExecType type = ET_STRING;
	Interpretator() {};
	int Exec(std::string, const ExecType, std::string &);
	int Reset();
	~Interpretator() {};
};

