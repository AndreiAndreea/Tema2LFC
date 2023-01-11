#include "Transitions.h"

Transitions::Transitions()
{
}

Transitions::Transitions(Unordered_map delta, std::string states, std::string symbols)
	:m_delta(delta), m_states(states), m_symbols(symbols)
{
}

void Transitions::InsertTransition(std::string transitionState, std::string transitionSymbol, std::string transitionResultStates)
{
	if (!ExistsState(transitionState))
		m_states += transitionState;
	if (!ExistsState(transitionResultStates))
		m_states += transitionResultStates;
	if (!ExistsSymbol(transitionSymbol))
		m_symbols += transitionSymbol;

	if (!ExistsTransition(transitionState, transitionSymbol))
	{
		std::pair<std::string, std::string> p = std::make_pair(transitionState, transitionSymbol);
		m_delta.insert({ p, transitionResultStates });
	}
	else
	{
		std::pair<std::string, std::string> p = std::make_pair(transitionState, transitionSymbol);
		auto resultState = m_delta.find(p);
		resultState->second += transitionResultStates;
	}
}

bool Transitions::ExistsTransition(std::string transitionState, std::string transitionSymbol)
{
	if (m_delta.find(std::make_pair(transitionState, transitionSymbol)) != m_delta.end())
		return true;
	return false;
}

std::string Transitions::GetTransitionResultStates(std::string transitionState, std::string transitionSymbol)
{
	//find returneazza un iterator
	auto resultState = m_delta.find(std::make_pair(transitionState, transitionSymbol));
	if (resultState != m_delta.end())
		return resultState->second;
	else
		return "-";
}

void Transitions::PrintTransitions()
{
	for (const auto& element : m_delta)
	{
		if (element.second.size() == 1)
		{
			std::cout << "delta (" << element.first.first << ", " << element.first.second << ") = ";
			std::cout << element.second << std::endl;
		}
		else
		{
			for (auto c : element.second)
			{
				std::cout << "delta (" << element.first.first << ", " << element.first.second << ") = ";
				std::cout << c << std::endl;
			}
		}
	}
}

Unordered_map Transitions::GetDeltaFunction()
{
	return m_delta;
}

std::string Transitions::GetUsedStates()
{
	return m_states;
}

std::string Transitions::GetUsedSymbols()
{
	return m_symbols;
}

bool Transitions::ExistsState(std::string state)
{
	for (const auto& s : m_states)
		if (s == state[0])
			return true;
	return false;
}

bool Transitions::ExistsSymbol(std::string symbol)
{
	for (const auto& s : m_symbols)
		if (s == symbol[0])
			return true;
	return false;
}

