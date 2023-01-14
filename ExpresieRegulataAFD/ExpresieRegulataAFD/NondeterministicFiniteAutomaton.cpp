#include "NondeterministicFiniteAutomaton.h"

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates)
    :m_states(states),
    m_alphabet(alphabet),
    m_transitions(transitions),
    m_initialState(initialState),
    m_finalStates(finalStates)
{
}

void NondeterministicFiniteAutomaton::PrintAutomaton()
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

Transitions NondeterministicFiniteAutomaton::GetTransitions()
{
    return m_transitions;
}

std::vector<std::string> NondeterministicFiniteAutomaton::GetStates()
{
    return m_states;
}

std::string NondeterministicFiniteAutomaton::GetInitialState()
{
    return m_initialState;
}

std::vector<std::string> NondeterministicFiniteAutomaton::GetFinalStates()
{
    return m_finalStates;
}

std::string NondeterministicFiniteAutomaton::GetAlphabet()
{
    return m_alphabet;
}
