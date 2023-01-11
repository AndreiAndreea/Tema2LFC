#include <iostream>
#include <fstream>
#include <string>

#include "PolishForm.h"

std::string ReadExpressionFromFile()
{
    std::string expression;
    char character;
    std::ifstream input;
    input.open("inputExpression.txt");
    if (input.is_open())
    {
		//cand citim din fisier trebuie sa facem operatia de concatenare
        while (input.get(character))
        {
            if (character != '(' && character != '|' && character != ')')
            {
                expression += character;
                expression += ".";
            } 
            else
                expression += character;

            if ((character == '|' || character == ')') && expression[expression.size() - 2] == '.')
                expression.erase(expression.end() - 2);
        }
    }
    if(expression[expression.size() - 1] == '.')
        expression.erase(expression.end() - 1);

    input.close();

    return expression;
}

void DisplayRegularExpressionIfValid(std::vector<char> PF)
{
    if (PF.size() != 0)
    {
        std::cout << "Forma poloneza este: ";
        for (int i = 0; i < PF.size(); i++)
        {
            std::cout << PF[i];
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Expresia regulata nu este valida!";
    }
}

int main()
{
    std::string expressionToProcess = ReadExpressionFromFile();
    std::cout << "Expresia regulata este: ";
    std::cout << expressionToProcess << std::endl;

    PolishForm polishForm(expressionToProcess);
    std::vector<char> PF = polishForm.GetPolishForm();
    
    DisplayRegularExpressionIfValid(PF);

	return 0;
}