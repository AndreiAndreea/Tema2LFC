#include "AutomatonStack.h"

AutomatonStack::AutomatonStack()
{
}

std::vector<std::string> AutomatonStack::MergeStates(std::vector<std::string> states1, std::vector<std::string> states2)
{
	for (int i = 1; i < states2.size(); i++)
		states1.push_back(states2[i]);
	return states1;
}

std::string AutomatonStack::MergeSymbols(std::string symbols1, std::string symbols2)
{
	for (auto& s : symbols2)
		if (symbols1.find(s) == std::string::npos)
			symbols1 += s;
	return symbols1;
}

NondeterministicFiniteAutomaton AutomatonStack::CreateAutomaton(std::string symbol, int counter)
{
	Transitions transitions;
	std::string state = "q";
	std::string transitionState = state + std::to_string(counter);
	std::string transitionResultState = state + std::to_string(counter + 1);
	
	transitions.InsertTransition(transitionState, symbol, { transitionResultState });
	
	return NondeterministicFiniteAutomaton({ transitionState, transitionResultState }, symbol, transitions, transitionState, { transitionResultState });
}

NondeterministicFiniteAutomaton AutomatonStack::AlternateAutomatons(NondeterministicFiniteAutomaton A1, NondeterministicFiniteAutomaton A2, int counter)
{
	//pentru operatia "|" (sau / alternare)	
	Transitions newTransitions;
	std::string state = "q";
	std::string newInitialState = state + std::to_string(counter);
	std::string newFinalState = state + std::to_string(counter + 1);
	std::vector<std::string> newStates = { newInitialState };
	
	for (auto& state : A1.GetStates())
		newStates.push_back(state);
	for (auto& state : A2.GetStates())
		newStates.push_back(state);
	newStates.push_back(newFinalState);

	newTransitions.InsertTransition(newInitialState, "-", { A1.GetInitialState(), A2.GetInitialState() });
	
	for (auto& t : A1.GetTransitions().GetDeltaFunction())
		newTransitions.InsertTransition(t.first.first, t.first.second, t.second);

	for(auto& t: A2.GetTransitions().GetDeltaFunction())
		newTransitions.InsertTransition(t.first.first, t.first.second, t.second);

	for (auto& oldfinalState : A1.GetFinalStates())
		newTransitions.InsertTransition(oldfinalState, "-", { newFinalState });

	for (auto& oldfinalState : A2.GetFinalStates())
		newTransitions.InsertTransition(oldfinalState, "-", { newFinalState });

	return NondeterministicFiniteAutomaton(newStates, MergeSymbols(A1.GetAlphabet(), A2.GetAlphabet()), newTransitions, newInitialState, { newFinalState });
}

NondeterministicFiniteAutomaton AutomatonStack::ConcatenateAutomatons(NondeterministicFiniteAutomaton A1, NondeterministicFiniteAutomaton A2)
{
	//pentru operatia "." (concatenare)
	//mai intai inseram toate tranzitiile din A1
	Transitions newTransitions = A1.GetTransitions();
	
	//modificam starea initiala din A2 in starea finala din A1 acolo unde este cazul
	for (const auto& transition : A2.GetTransitions().GetDeltaFunction())
	{
		std::string transitionState = transition.first.first;
		std::string transitionSymbol = transition.first.second;
		std::vector<std::string> transitionResultState = transition.second;

		if (transitionState == A2.GetInitialState())
		{
			std::vector<std::string> newTransitionState = A1.GetFinalStates();
			newTransitions.InsertTransition(newTransitionState[0], transitionSymbol, transitionResultState);
		}
		else
		{
			newTransitions.InsertTransition(transitionState, transitionSymbol, transitionResultState);
		}
	}
	
	return NondeterministicFiniteAutomaton(MergeStates(A1.GetStates(), A2.GetStates()), MergeSymbols(A1.GetAlphabet(), A2.GetAlphabet()), newTransitions, A1.GetInitialState(), A2.GetFinalStates());
}

std::stack<std::pair<NondeterministicFiniteAutomaton, int>> AutomatonStack::GetAutomatonStack()
{
	return m_stack;
}
