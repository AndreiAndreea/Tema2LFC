#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <stack>

class PolishForm
{
public:
	PolishForm(std::string expressionToProcess);
	
private:
	int GetPriority(char c);
	bool IsCharacter(char element);
	bool IsOperator(char element);
	
public:
	std::vector<char> GetPolishForm();
	
private:
	std::string m_expressionToProcess;
	std::vector<char> m_polishForm;
};

