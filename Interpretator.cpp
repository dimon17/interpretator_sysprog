#include "stdafx.h"
#include "Interpretator.h"

#include <iostream>

bool is_digits(const std::string &str)
{
	if (str.find('.') != std::string::npos) return true;
	return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

bool Interpretator::checkOper()//проверка операций
{
	bool result[2];

	std::string str;
	for (const auto& p : this->vector)
	{
		result[0] = analysLeft(p.first);//анализируем левую часть
		result[1] = analysRight(p.second);//анализируем правую часть

		if (!result[0] || !result[1])//если есть хот€ бы одна ошибка
			throw std::runtime_error("Error in parsing at '" + (!result[0] && !result[1] ? (p.first + p.second) : !result[0] ? p.first : p.second) + "'.");
	}

	return true;
}
bool Interpretator::checkVars(std::string str, bool type = false)//проверка переменных на повторное использование
{
	bool error = true;
	for (const auto& p : this->vars)
	{
		switch (type)
		{
		case false: if (p.first == str && !p.second) error = false; break;
		default: if (p.first == str) error = false; break;
		}
	}
	return error;
}
bool Interpretator::analysLeft(std::string p)//анализируем левую часть
{
	bool add = false, _const = false, opers = false;
	int const addLen = strlen("var") + 1;//длина объ€влени€ переменной
	int const constLen = strlen("const") + 1;//длина объ€влени€ константы

	if (p == "}") return true;//если попали на закрытие блока - выходим из функции

	bool res = false;

	_const = (p.find("const") == addLen || !p.find("const"));
	add = !p.find("var");
	opers = (!p.find("if") || !p.find("while"));

	boost::trim(p);
	if (add || _const)
	{
		str = p.substr((add ? addLen : 0) + (_const ? constLen : 0), p.length());
		boost::trim(str);
		if (str == "if" || str == "var" || str == "while" || str == "const")		return false; // Command name

		this->vars.push_back(make_pair(str, _const));
		return true;
	}

	if (opers)
	{
		std::string l, r;
		size_t en;
		en = p.find_first_of(">") != std::string::npos ? p.find_first_of(">") : (p.find_first_of("<") != std::string::npos ? p.find_first_of("<") : p.find_first_of("="));

		size_t ln = !p.find("if") ? 2 : strlen("while");
		l = p.substr(ln, en - ln);

		en = p[en + 1] == '=' ? 2 : 1;
		r = p.substr(ln + l.length() + en, p.length());

		if (checkVars(l, true) || checkVars(r, true)) return false; // переменна€ уже объ€влена
		return true;
	}

	if (checkVars(p)) return false; // переменна€ уже объ€влена

	return true;
}
bool  Interpretator::analysRight(std::string p)//анализируем правую часть
{
	boost::trim(p);//обрезаем строку p
	if (p == "{" || !p.length()) return true;
	size_t len = p.length();
	std::string str = "";
	for (size_t i = 0; i < len; i++)
	{
		if (p[i] == '+' || p[i] == '*' || p[i] == '/' || p[i] == '-')
		{
			if (!str.empty() && !is_digits(str) && checkVars(str, true)) return false;
			str = "";
			continue;
		}

		str += p[i];

	}

	if (!str.empty() && !is_digits(str) && checkVars(str, true)) return false;
	return true;
}
int Interpretator::Reset()//очищаем состо€ние интерпретатора
{
	this->vars.clear();
	this->str = "";
	this->type = ET_STRING;
	this->vector.clear();
	return 0;
}
int Interpretator::Exec(std::string str, const ExecType type, std::string &ret)//главна€ функци€ класса (как main)
{
	this->str = str;
	this->type = type;

	std::string analys;
	analys = str;

	vector.clear();

	std::string::iterator begin = analys.begin(), end = analys.end();

	auto const success = boost::spirit::qi::phrase_parse(begin, end, this->calc, this->sk, this->vector);

	std::cout << "---------------------\n";
	if (success && begin == end)
	{
		std::cout << "Syntax is OK! \n";
		try
		{
			this->checkOper();
		}
		catch (std::runtime_error& e)
		{
			std::cout << e.what() << std::endl;
		}

		/*for (const auto& p : vector)
		{
		std::cout << p.first  << ", " << p.second << std::endl;
		}  */
	}
	else
	{
		std::cout << "Error in parsing.\n stopped at: \""
			<< std::string(begin, end) << "\"\n";
	}
	std::cout << "---------------------\n";
	return success;
}
