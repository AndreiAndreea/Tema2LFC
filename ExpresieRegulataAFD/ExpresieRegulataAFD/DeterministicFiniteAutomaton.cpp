#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates)
	:m_states(states),
	m_alphabet(alphabet),
	m_transitions(transitions),
	m_initialState(initialState),
	m_finalStates(finalStates)
{
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(NondeterministicFiniteAutomaton nfa)
{
	int count = 0;
	std::vector<std::string> states = nfa.GetStates();
	Transitions transitions = nfa.GetTransitions();

	//vector with the name of the new transition and the lambda closure
	std::vector<std::pair<std::string, LambdaClosure>> newStates;

	m_initialState = "q" + std::to_string(count) + "'";
	m_states.push_back(m_initialState);
	m_alphabet = nfa.GetAlphabet();

	//verificat situatie cand nu are lambda-inchidere simbolul de start!!!!
	newStates.push_back(std::make_pair(m_initialState, transitions.GetLambdaClosure(nfa.GetInitialState())));

	//for each state in the newStates vector
	for (int i = 0; i < newStates.size(); i++)
	{
		//for each symbol in the alphabet
		for (int j = 0; j < m_alphabet.size(); j++)
		{
			auto crtState = newStates[i];
			auto crtSymbol = m_alphabet[j];
			//get the resuslting states from the transition function
			std::vector<std::string> resultingStates = transitions.GetTransitionResultStates(crtState.second, std::string(1, crtSymbol));
			//create the new lambda closure by getting the lambda closure of each resulting state and combining the results
			LambdaClosure newLambdaClosure;
			//get the lambda closure of each state
			for (auto state : resultingStates) {
				LambdaClosure lambdaClosure = transitions.GetLambdaClosure(state);
				newLambdaClosure.insert(lambdaClosure.begin(), lambdaClosure.end());
			}

			if (newLambdaClosure.empty()) {
				m_transitions.InsertTransition(crtState.first, std::string(1, crtSymbol), { "NULL" });
				continue;
			}

			auto foundState = std::find_if(newStates.begin(), newStates.end(),
				[&newLambdaClosure](const std::pair<std::string, LambdaClosure>& element)
				{
					return element.second == newLambdaClosure;
				});

			if (foundState == newStates.end()) {
				//add the new state to the newStates vector
				auto newState = std::make_pair("q" + std::to_string(++count) + "'", newLambdaClosure);
				newStates.push_back(newState);
				m_states.push_back(newState.first);
				//add the new transition to the new transitions
				m_transitions.InsertTransition(crtState.first, std::string(1, crtSymbol), { newState.first });

				//check if the new state is final - if it contains a final state from the old automaton then it is final
				for (auto state : newLambdaClosure) {
					std::vector<std::string> finalStates = nfa.GetFinalStates();
					if (std::find(finalStates.begin(), finalStates.end(), state) != finalStates.end()) {
						m_finalStates.push_back(newState.first);
						break;
					}
				}
			}
			else {
				//get the name of the state that has the same lambda closure
				std::string newStateName = foundState->first;
				//add the new transition to the newTransitions vector
				m_transitions.InsertTransition(crtState.first, std::string(1, crtSymbol), { newStateName });
			}
		}
	}

}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	//1. multimea starilor Q sa fie finita si nevida
	if (m_states.size() == 0)
		return false;

	//2. alfabetul de intrare Sigma sa fie o multime finita si nevida
	if (m_alphabet.size() == 0)
		return false;

	//3. avem o singura stare initiala + starea initiala trebuie sa apartina Q
	if (m_initialState.size() == 0 || m_initialState.size() == 1 || m_initialState.size() > 2) //initial state ex: q0 - size 2
		return false;
	if (std::find(m_states.begin(), m_states.end(), m_initialState) == m_states.end())
		return false;

	//4. multimea starilor finale F este finita si nevida + F este inclusa in Q
	if (m_finalStates.size() == 0)
		return false;
	for (auto& finalState : m_finalStates)
		if (std::find(m_states.begin(), m_states.end(), finalState) == m_states.end())
			return false;

	//5. tranzitii definite corect = stari si simboluri din starile si simbolurile mele
	for (auto& state : m_transitions.GetUsedStates())
		if (std::find(m_states.begin(), m_states.end(), state) == m_states.end())
			return false;
	for (const auto& symbol : m_transitions.GetUsedSymbols())
		if (m_alphabet.find(symbol) == std::string::npos)
			return false;

	return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton()
{
	std::cout << "Elementele automatului sunt: " << std::endl;
	std::cout << "Stari - Q:{";
	for (int i = 0; i < m_states.size(); i++)
	{
		if (i != (m_states.size() - 1))
			std::cout << m_states[i] << ", ";
		else
			std::cout << m_states[i] << "}" << std::endl;
	}

	std::cout << "Alfabet de intrare - Sigma:{";
	for (int i = 0; i < m_alphabet.size(); i++)
	{
		if (i != (m_alphabet.size() - 1))
			std::cout << m_alphabet[i] << ", ";
		else
			std::cout << m_alphabet[i] << "}" << std::endl;
	}

	std::cout << "Stare initiala: " << m_initialState << std::endl;

	std::cout << "Multimea starilor finale - F:{";
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		if (i != (m_finalStates.size() - 1))
			std::cout << m_finalStates[i] << ", ";
		else
			std::cout << m_finalStates[i] << "}" << std::endl;
	}

	std::cout << "Functia de tranzitie:" << std::endl;
	m_transitions.PrintTransitions();
}

bool DeterministicFiniteAutomaton::CheckWord(std::string word)
{
	std::cout << "Cuvantul verificat este: " << word << std::endl;
	std::string transitionInputState = m_initialState;
	std::string transitionResultState;
	std::string symbol;

	bool isAccepted = false;

	for (int i = 0; i < word.size(); i++)
	{
		symbol = "";
		symbol += word[i];

		transitionResultState = m_transitions.GetTransitionResultStates(transitionInputState, symbol)[0];

		if (transitionResultState.size() == 0)
		{
			std::cout << "Nu avem stari rezultate! BLOCAJ!" << std::endl;
			return false;
		}

		std::cout << "delta(" << transitionInputState << ", " << symbol << ") = " << transitionResultState << std::endl;

		transitionInputState = transitionResultState;
	}
	std::cout << "Multimea starilor finale este: {";
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		if (i != (m_finalStates.size() - 1))
			std::cout << m_finalStates[i] << ", ";
		else
			std::cout << m_finalStates[i] << "}" << std::endl;
	}

	if (std::find(m_finalStates.begin(), m_finalStates.end(), transitionResultState) != m_finalStates.end()) {
		std::cout << "Multimea de stari rezultate contine cel putin o stare finala!" << std::endl;
		return true;
	}
	
	std::cout << "Multimea de stari rezultate NU contine nicio stare finala!" << std::endl;
	return false;

}

bool DeterministicFiniteAutomaton::IsDeterministic()
{
	Unordered_map transitions = m_transitions.GetDeltaFunction();
	for (const auto& element : transitions)
		if (element.second.size() > 2) //o stare are doua caractere
			return false;
	return true;
}