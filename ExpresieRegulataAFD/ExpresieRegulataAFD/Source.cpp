#include <iostream>
#include <fstream>
#include <string>

#include "DeterministicFiniteAutomaton.h"
#include "PolishForm.h"
#include "AutomatonStack.h"

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

bool DisplayRegularExpressionIfValid(std::vector<char> PF)
{
    if (PF.size() != 0)
    {
        std::cout << "Forma poloneza este: ";
        for (int i = 0; i < PF.size(); i++)
        {
            std::cout << PF[i];
        }
        std::cout << std::endl;
        return true;
    }
    else
    {
        std::cout << "Expresia regulata nu este valida!";
        return false;
    }
}

DeterministicFiniteAutomaton ConversionFromRegularExpressionToAFD(std::vector<char> polishForm)
{
    int counter = 0, index = 1;
    AutomatonStack handler;
    std::stack<std::pair<NondeterministicFiniteAutomaton, int>> automatonStack = handler.GetAutomatonStack();
    
    for (auto& c : polishForm)
    {
        if (isalpha(c))
        {
            std::cout << "Pentru caracterul: '" << c << "' se va crea automatul A" << index << std::endl;
			std::string symbol(1, c);
            
            //adaugam in stiva automatul nou creat
            NondeterministicFiniteAutomaton automaton = handler.CreateAutomaton(symbol, counter);
            automaton.PrintAutomaton();
            std::cout << "------------------------------------------------------------" << std::endl;

            automatonStack.push({ automaton, index });
            counter += 2;
            index += 1;
        }
        else if (c == '|')
        {
            std::cout << "Pentru caracterul '|' :" << std::endl;
            std::cout << "Extragem din varful stivei doua automate si le alternam!" << std::endl;
            
            NondeterministicFiniteAutomaton B = automatonStack.top().first;
            int indexB = automatonStack.top().second;
            automatonStack.pop();

            NondeterministicFiniteAutomaton A = automatonStack.top().first;
            int indexA = automatonStack.top().second;
            automatonStack.pop();

            std::cout << "Alternam automatele:" << std::endl;
            std::cout << "A" << indexA << std::endl;
            A.PrintAutomaton();
            std::cout << std::endl;

            std::cout << "A" << indexB << std::endl;
            B.PrintAutomaton();
            std::cout << std::endl;
            
            NondeterministicFiniteAutomaton C = handler.AlternateAutomatons(A, B, counter);
            std::cout << "Obtinem automatul A" << index << std::endl;
            C.PrintAutomaton();
            std::cout << "------------------------------------------------------------" << std::endl;

            automatonStack.push({ C, index });
			counter += 2;
            index += 1;
        }
        else if (c == '.')
        {
            std::cout << "Pentru caracterul '.' :" << std::endl;
            std::cout << "Extragem din varful stivei doua automate si le concatenam!" << std::endl;
            
			NondeterministicFiniteAutomaton B = automatonStack.top().first;
			int indexB = automatonStack.top().second;
            automatonStack.pop();
            
			NondeterministicFiniteAutomaton A = automatonStack.top().first;
			int indexA = automatonStack.top().second;
            automatonStack.pop();

            std::cout << "Concatenam automatele:" << std::endl;
			std::cout << "A" << indexA << std::endl;
			A.PrintAutomaton();
            std::cout << std::endl;
            
			std::cout << "A" << indexB << std::endl;
			B.PrintAutomaton();
			std::cout << std::endl;
            
			NondeterministicFiniteAutomaton C = handler.ConcatenateAutomatons(A, B);
            std::cout << "Obtinem automatul A"<<index << std::endl;
            C.PrintAutomaton();
            std::cout << "------------------------------------------------------------" << std::endl;
            
            automatonStack.push({ C, index });
            index += 1;
        }
        else if (c == '*')
        {
            std::cout << "Pentru caracterul '*' :" << std::endl;
            std::cout << "Extragem din varful stivei un automat si aplicam inchiderea Kleene!" << std::endl;
            
			NondeterministicFiniteAutomaton A = automatonStack.top().first;
			int indexA = automatonStack.top().second;
			automatonStack.pop();

			std::cout << "Aplicam inchiderea Kleene automatului:" << std::endl;
			std::cout << "A" << indexA << std::endl;
			A.PrintAutomaton();
			std::cout << std::endl;
            
            NondeterministicFiniteAutomaton B = handler.KleeneClosingAutomaton(A, counter);
			std::cout << "Obtinem automatul A" << index << std::endl;
			B.PrintAutomaton();
			std::cout << "------------------------------------------------------------" << std::endl;
			automatonStack.push({ B, index });
            
			counter += 2;
			index += 1;
        }
    }
    
	auto nfAutomaton = automatonStack.top().first;
    
    return DeterministicFiniteAutomaton(nfAutomaton);
   
}

void printMenu() {
    std::cout << "---MENU---\n";
    std::cout << "0. Exit\n";
    std::cout << "1. Print Automaton.\n";
    std::cout << "2. Print the regular expression read from file.\n";
    std::cout << "3. Check a word.\n";
}

int main()
{
    int option;
    const uint8_t exitOptionNo = 0;
    const uint8_t printAutomatonOptionNo = 1;
    const uint8_t printRegularExpressionOptionNo = 2;
    const uint8_t checkWordOptionNo = 3;

    std::string expressionToProcess = ReadExpressionFromFile();

    PolishForm polishForm(expressionToProcess);
    std::vector<char> PF = polishForm.GetPolishForm();

    if (DisplayRegularExpressionIfValid(PF)) {
        auto AFD = ConversionFromRegularExpressionToAFD(PF);
        
        //menu implementation
        std::string wordToCheck;

        std::cout << "^^^ Steps taken when creating the AFN from the regular expression ^^^\n\n";

        do {
            printMenu();
            std::cout << "Please enter an option: ";
            std::cin >> option;
            switch (option)
            {
            case printAutomatonOptionNo:
                AFD.PrintAutomaton();
                break;
            case printRegularExpressionOptionNo:
                std::cout << "Expresia regulata este: ";
                std::cout << expressionToProcess << std::endl;
                break;
            case checkWordOptionNo:
                std::cout << "Please enter the word you want to check: ";
				std::cin >> wordToCheck;
                if (AFD.CheckWord(wordToCheck)) {
                    std::cout << "\nThe word is accepted by the Automaton!\n";
                }
                else {
                    std::cout << "\nThe word is NOT accepted by the Automaton!\n";
                }
                break;
            default:
                std::cerr << "Invalid option. Try again." << std::endl;
                break;
            }
        } while (option != exitOptionNo);
    }
    
	return 0;
}