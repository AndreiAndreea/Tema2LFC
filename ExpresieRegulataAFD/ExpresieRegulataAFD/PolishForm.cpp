#include "PolishForm.h"

PolishForm::PolishForm(std::string expressionToProcess)
	:m_expressionToProcess(expressionToProcess)
{
}

int PolishForm::GetPriority(char c)
{
	switch (c)
	{
	case '(':
		return 0;
	case '|': 
		return 1;
	case '.': 
		return 2;
	case '*': 
		return 3;
	default:
		return 0;
	}
}

bool PolishForm::IsCharacter(char element)
{
	if ((element >= 65 && element <= 90) || (element >= 97 && element <= 122))
		return true;
	return false;
}

bool PolishForm::IsOperator(char element)
{
	if (element == '|' || element == '.' || element == '*')
		return true;
	return false;
}

std::vector<char> PolishForm::GetPolishForm()
{
	bool suntem_in_numar = false;
	std::stack<char> OP;
	std::vector<char> FP, vid;
	char previousCharacter = '0';
	for (char element : m_expressionToProcess)
	{
		if (!IsCharacter(element) && !IsOperator(element) && element != '(' && element != ')' && element != ' ')
		{
			std::cout << "EROARE! Expresia nu a fost introdusa corect!" << std::endl;
			std::cout << "Expresia contine un caracter nepermis!" << std::endl;
			return vid;
		}
		if (element == ' ')
		{
			if (IsOperator(previousCharacter) || previousCharacter == '(' || previousCharacter == ')')
			{
				std::cout << "EROARE! Expresia nu a fost introdusa corect!" << std::endl;
				std::cout << "Expresia contine spatii incorect!" << std::endl;
				return vid;
			}
			previousCharacter = ' ';
			continue;
		}
		if (element == '|' && previousCharacter == '|')
		{
			std::cout << "EROARE! Expresia nu a fost introdusa corect!" << std::endl;
			std::cout << "Expresia contine operatorul '|' in plus!" << std::endl;
			return vid;
		}
		if (IsCharacter(element))
		{
			suntem_in_numar = true;
			if (IsCharacter(previousCharacter))
			{
				FP.push_back(element);
			}
			else
			{
				FP.push_back(' ');
				FP.push_back(element);
			}
		}
		else
		{
			suntem_in_numar = false;
			if (element == '(')
			{
				OP.push('(');
			}
			else if (element == ')')
			{
				while (!OP.empty() && OP.top() != '(')
				{
					FP.push_back(' ');
					FP.push_back(OP.top());
					OP.pop();
				}
				if (OP.empty())
				{
					std::cout << "EROARE! Expresia nu a fost introdusa corect!" << std::endl;
					std::cout << "Expresia contine caracterul ')', fara ca paranteza sa fie deschisa!" << std::endl;
					return vid;
				}
				OP.pop(); //eliminam si paranteza deschisa
			}
			else
			{
				while (!OP.empty() && GetPriority(element) <= GetPriority(OP.top()))
				{
					FP.push_back(' ');
					FP.push_back(OP.top());
					OP.pop();
				}
				OP.push(element);
			}
		}
		previousCharacter = element; //actualizam
	}
	while (!OP.empty()) //golim ce a ramas in stiva
	{
		if (OP.top() == '(')
		{
			std::cout << "EROARE! Expresia nu a fost introdusa corect!" << std::endl;
			std::cout << "Expresia contine caracterul '(', fara ca paranteza sa fie inchisa!" << std::endl;
			return vid;
		}
		FP.push_back(' ');
		FP.push_back(OP.top());
		OP.pop();
	}

	if (FP[0] == ' ')
	{
		for (unsigned int i = 0; i < FP.size() - 1; i++)
		{
			FP[i] = FP[i + 1];
		}
		FP.resize(FP.size() - 1);
	}
	
	m_polishForm = FP;
	
	return FP;
}