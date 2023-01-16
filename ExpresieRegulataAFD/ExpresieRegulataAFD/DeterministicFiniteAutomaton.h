#pragma once
#include "Transitions.h"

#include <string>

class DeterministicFiniteAutomaton
{
public:
	DeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates);

	bool VerifyAutomaton();
	void PrintAutomaton();
	std::vector<std::string> GetLambdaClosure(std::vector<std::string> states);
	bool CheckWord(std::string word); //de modificat
	bool IsDeterministic();
	
private:
	std::vector<std::string> m_states; //Q - multime finita nevida de stari = m_VN
	std::string m_alphabet; //sigma - simbolurile / alfabetul de intrare = m_VT
	Transitions m_transitions; //transition function - input:(stare, simbol) -> o stare
	std::string m_initialState; //q0 - stare initiala = m_S
	std::vector<std::string> m_finalStates; //F - multimea starilor finale, inclusa in Q
};

