#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/vector.hpp>

#include <vector>
#include <string>

/* для удобного пользования*/
namespace qi = boost::spirit::qi;
namespace repository = boost::spirit::repository;

template <typename Iterator>
struct skip_grammar : public qi::grammar<Iterator>
{
	skip_grammar()
		: skip_grammar::base_type(skip)
	{
		skip
			= qi::space
			| repository::confix("//", qi::eol)
			[*(qi::char_ - qi::eol)]
		;
	}

	qi::rule<Iterator> skip;
};


template <typename Iterator>
/* template <typename Iterator, typename T1, typename T2, typename T3, typename T4> struct grammar*/
struct calculator : qi::grammar<Iterator, std::vector< std::pair< std::string, std::string > >(), skip_grammar<Iterator> >
{
#define Q_NUM ( qi::char_("0-9") >> * qi::char_("0-9") )
#define Q_SPACEBAR_LEX ( qi::space >> *( qi::space ) )
#define Q_BOOL_OPERS (qi::char_('>') | qi::char_('<') | qi::string("==") | qi::char_('=') | qi::string("!=") | qi::string(">=") | qi::string("<="))
#define Q_NUM_TYPES  qi::hold[qi::lexeme[Q_NUM >> qi::string(".") >> Q_NUM]] | Q_NUM
#define Q_EXPR_TYPE ( qi::string("if") | qi::string("while") )

	/* объявляем правила */
	qi::rule < Iterator, std::vector < std::pair < std::string, std::string > >(), skip_grammar<Iterator> > main_expr;
	/* */
	qi::rule<Iterator, std::pair< std::string, std::string >(), skip_grammar<Iterator>> pair, fpair, lpair;
	/* */
	qi::rule<Iterator, std::string(), skip_grammar<Iterator>> left_oper, right_oper, bool_expr, expr, numExpr, strExpr;
	/* для идентификаторов*/
	qi::rule<Iterator, std::string(), skip_grammar<Iterator>> variable = qi::lexeme[(qi::alnum) >> *(qi::alnum | qi::uint_ | qi::char_("_"))];

	calculator() : calculator::base_type(main_expr)
	{
		main_expr = (fpair >> (pair % ';' >> ';') >> lpair) | (pair % ';' >> ';');//разбираем логическое выражение с последующими { и парами внутри блока и закрывающей }, или
		//или просто выражения (объявления переменных), которые разделены точкой с запятой

		fpair = (bool_expr >> qi::string("{"));//логическое выражение и первая открывающая блок скабка
		lpair = qi::string("}") >> qi::string("");//последняя закрывающая блок скобка
		pair = left_oper >> -('=' >> right_oper);//объявление переменной с возможной последующей инициализацией

		bool_expr = qi::hold[Q_EXPR_TYPE >> '(' >> right_oper >> Q_BOOL_OPERS >> right_oper >> ')'];//разбираем логические выражения в скобках
		left_oper = qi::lexeme[-((qi::string("var")) >> Q_SPACEBAR_LEX) >> -(qi::string("const") >> Q_SPACEBAR_LEX)] >> variable;//разбираем объявление переменной

		/* для выражений */
		right_oper = (term >> *(qi::string("+") >> term | qi::string("-") >> term));//2. так же как и 1ое, только знаки + и -
		term = factor >> *(qi::string("*") >> factor | qi::string("/") >> factor);//1. тут учитываются операции умножения и деления. Формат: выражение, затем знак * или / и снова какое-то выражение
		factor = Q_NUM_TYPES | qi::string("(") >> right_oper >> qi::string(")") | qi::string("+") >> factor | qi::string("-") >> factor | variable;//разбираем скобки в операциях
	}

	qi::rule<Iterator, std::string(), skip_grammar<Iterator>> term, factor;

#undef  Q_SPACEBAR_LEX
#undef	Q_BOOL_OPERS
#undef  Q_NUM_TYPES
#undef  Q_NUM
#undef	Q_EXPR_TYPE
};
