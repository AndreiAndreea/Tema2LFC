#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates)
	:m_states(states),
	m_alphabet(alphabet),
	m_transitions(transitions),
	m_initialState(initialState),
	m_finalStates(finalStates)
{
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

std::vector<std::string> DeterministicFiniteAutomaton::GetLambdaClosure(std::vector<std::string> states)
{
	std::vector<std::string> lambdaClosure;
	Unordered_map delta = m_transitions.GetDeltaFunction();

	for (auto& state : states)
	{
		auto it = delta.find(std::make_pair(state, "-"));
		if (it != delta.end())
		{
			std::cout << '(' << it->first.first << "," << it->first.second << ") = " << '{';
			for (auto& state : it->second)
			{
				std::cout << state << " ";
				lambdaClosure.push_back(state);
			}
			std::cout << '}' << "\n";
		}
	}
	return lambdaClosure;
}

bool DeterministicFiniteAutomaton::CheckWord(std::string word)
{
	std::string currentState = m_initialState;
	std::vector<std::string> currentLambdaClosure = GetLambdaClosure({ currentState });
	Unordered_map delta = m_transitions.GetDeltaFunction();
	std::vector<std::string> resultStates;

	for (int i = 0; i < word.size(); i++)
	{
		resultStates.clear();
		for (auto& state : currentLambdaClosure)
		{
			auto it = delta.find(std::make_pair(state, "-"));
			if (it != delta.end())
			{
				for (auto& result : it->second)
				{
					resultStates.push_back(result);
				}
			}
		}
		currentLambdaClosure = GetLambdaClosure(resultStates);
	}

	for (auto& currentState : currentLambdaClosure)
		if (std::find(m_finalStates.begin(), m_finalStates.end(), currentState) != m_finalStates.end())
			return true;

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