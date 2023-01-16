#include "Transitions.h"

Transitions::Transitions()
{
}

Transitions::Transitions(Unordered_map delta, std::vector<std::string> states, std::string symbols)
	:m_delta(delta), m_states(states), m_symbols(symbols)
{
}

void Transitions::InsertTransition(std::string transitionState, std::string transitionSymbol, std::vector<std::string> transitionResultStates)
{
	if (!ExistsState(transitionState))
		m_states.push_back(transitionState);
	
	for(auto& s: transitionResultStates)
		if (!ExistsState(s))
			m_states.push_back(s);
	
	if (!ExistsSymbol(transitionSymbol))
		m_symbols += transitionSymbol;

	if (!ExistsTransition(transitionState, transitionSymbol))
	{
		std::pair<std::string, std::string> p = std::make_pair(transitionState, transitionSymbol);
		m_delta.insert({ p, {transitionResultStates} });
	}
	else
	{
		std::pair<std::string, std::string> p = std::make_pair(transitionState, transitionSymbol);
		auto resultState = m_delta.find(p);
		
		for(auto& s: transitionResultStates)
			if (std::find(resultState->second.begin(), resultState->second.end(), s) == resultState->second.end())
				resultState->second.push_back(s);
	}
}

bool Transitions::ExistsTransition(std::string transitionState, std::string transitionSymbol)
{
	if (m_delta.find(std::make_pair(transitionState, transitionSymbol)) != m_delta.end())
		return true;
	return false;
}

std::vector<std::string> Transitions::GetTransitionResultStates(std::string transitionState, std::string transitionSymbol)
{
	auto resultState = m_delta.find(std::make_pair(transitionState, transitionSymbol));
	if (resultState != m_delta.end())
		return resultState->second;
	else
		return {};
}

std::vector<std::string> Transitions::GetTransitionResultStates(LambdaClosure lambdaClosure, std::string transitionSymbol)
{
	std::vector<std::string> resultStates;
	for (auto& state : lambdaClosure)
	{
		auto transitionResultStates = GetTransitionResultStates(state, transitionSymbol);
		resultStates.insert(resultStates.end(), transitionResultStates.begin(), transitionResultStates.end());
	}
	return resultStates;
}

LambdaClosure Transitions::GetLambdaClosure(std::string state)
{
	std::queue<std::string> statesToVisit;
	LambdaClosure lambdaClosure;
	statesToVisit.push(state);
	lambdaClosure.insert(state);
	
	while (!statesToVisit.empty()) {
		std::string currentState = statesToVisit.front();
		statesToVisit.pop();
		if (ExistsTransition(currentState, "-"))
		{
			std::vector<std::string> resultStates = GetTransitionResultStates(currentState, "-");
			for (auto& s : resultStates)
			{
				lambdaClosure.insert(s);
				statesToVisit.push(s);
			}
		}
	}
	
	return lambdaClosure;
}

void Transitions::PrintTransitions()
{
	for (const auto& element : m_delta)
	{
		if (element.second.size() == 1)
		{
			std::cout << "delta (" << element.first.first << ", " << element.first.second << ") = ";
			std::cout << element.second[0] << std::endl;
		}
		else
		{
			std::cout << "delta (" << element.first.first << ", " << element.first.second << ") = {";
			for (int i = 0; i < element.second.size(); i ++)
			{
				if (i != (element.second.size() - 1)) 
					std::cout << element.second[i] << ", ";
				else
					std::cout << element.second[i] << "}" << std::endl;
			}
		}
	}
}

Unordered_map Transitions::GetDeltaFunction()
{
	return m_delta;
}

std::vector<std::string> Transitions::GetUsedStates()
{
	return m_states;
}

std::string Transitions::GetUsedSymbols()
{
	return m_symbols;
}

bool Transitions::ExistsState(std::string state)
{
	for(auto& currentState: m_states)
		if (state == currentState)
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

