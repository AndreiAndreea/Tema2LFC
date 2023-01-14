#pragma once
#include "Transitions.h"

#include <string>

class NondeterministicFiniteAutomaton
{
public:
	NondeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates);

	void PrintAutomaton();

	Transitions GetTransitions();
	std::vector<std::string> GetStates();
	std::string GetInitialState();
	std::vector<std::string> GetFinalStates();
	std::string GetAlphabet();

private:
	std::vector<std::string> m_states; //Q - multime finita nevida de stari = m_VN
	std::string m_alphabet; //sigma - simbolurile / alfabetul de intrare = m_VT
	Transitions m_transitions; //transition function - input:(stare, simbol) -> o stare
	std::string m_initialState; //q0 - stare initiala = m_S
	std::vector<std::string> m_finalStates; //F - multimea starilor finale, inclusa in Q
};
