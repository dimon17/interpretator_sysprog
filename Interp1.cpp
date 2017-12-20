// Poliz.cpp: определяет точку входа для консольного приложения.
//
#include <stdafx.h>
#include <iostream>
#include "calculator.h"
#include <string>

#include "Interpretator.h"

int main()
{
	std::cout << "It is my parser!\n\n";
	std::cout << "Type an expression or q to quit\n\n";

	std::string expression;
	calculator<std::string::iterator> calc;

	Interpretator intpr;

	std::string err;
	while (true)
	{
		std::getline(std::cin, expression);
		if (expression == "q" || expression == "Q") break;

		intpr.Exec(expression, Interpretator::ET_STRING, err);

		//if (!err.empty) std::cout << err << std::endl;
	}
}