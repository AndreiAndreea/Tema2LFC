#pragma once

#include "NondeterministicFiniteAutomaton.h"

#include <stack>

class AutomatonStack
{
public:
	AutomatonStack();
	
	std::vector<std::string> MergeStates(std::vector<std::string> states1, std::vector<std::string> states2);
	std::string MergeSymbols(std::string symbols1, std::string symbols2);
	
	NondeterministicFiniteAutomaton CreateAutomaton(std::string symbol, int counter);
	NondeterministicFiniteAutomaton AlternateAutomatons(NondeterministicFiniteAutomaton A1, NondeterministicFiniteAutomaton A2, int counter);
	NondeterministicFiniteAutomaton ConcatenateAutomatons(NondeterministicFiniteAutomaton A1, NondeterministicFiniteAutomaton A2);
	NondeterministicFiniteAutomaton KleeneClosingAutomaton(NondeterministicFiniteAutomaton A1, int counter);
	
	std::stack<std::pair<NondeterministicFiniteAutomaton, int>> GetAutomatonStack();
	
private:
	std::stack<std::pair<NondeterministicFiniteAutomaton, int>> m_stack;
};

