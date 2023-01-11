#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <functional>

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);
		return h1 ^ h2;
	}
};

using TransitionFunctionInputs = std::pair<std::string, std::string>;
using Unordered_map = std::unordered_map<TransitionFunctionInputs, std::string, pair_hash>;

class Transitions
{
public:
	Transitions();
	Transitions(Unordered_map delta, std::string states, std::string symbols);

	void InsertTransition(std::string transitionState, std::string transitionSymbol, std::string transitionResultStates);
	bool ExistsTransition(std::string transitionState, std::string transitionSymbol);
	std::string GetTransitionResultStates(std::string transitionState, std::string transitionSymbol);
	void PrintTransitions();

public:
	Unordered_map GetDeltaFunction();
	std::string GetUsedStates();
	std::string GetUsedSymbols();

private:
	bool ExistsState(std::string state);
	bool ExistsSymbol(std::string symbol);

private:
	Unordered_map m_delta;
	std::string m_states;
	std::string m_symbols;
};

