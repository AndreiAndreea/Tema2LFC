#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::string states, std::string alphabet, Transitions transitions, std::string initialState, std::string finalStates)
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
    if (m_initialState.size() == 0 || m_initialState.size() > 1)
        return false;
    if (m_states.find(m_initialState[0]) == std::string::npos)
        return false;
    //4. multimea starilor finale F este finita si nevida + F este inclusa in Q
    if (m_finalStates.size() == 0)
        return false;
    for (auto& F : m_finalStates)
        if (m_states.find(F) == std::string::npos)
            return false;
    //5. tranzitii definite corect
    //stari si simboluri din starile si simbolurile mele
    for (const auto& state : m_transitions.GetUsedStates())
        if (m_states.find(state) == std::string::npos)
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
    std::string currentStates;
    std::string symbol, input, result;
    bool isAccepted = false;
    for (int i = 0; i < word.size(); i++)
    {
        currentStates = "";
        symbol = "";
        symbol += word[i];
        for (const auto& inputState : transitionInputState)
        {
            input = "";
            input += inputState;
            transitionResultState = m_transitions.GetTransitionResultStates(input, symbol);
            for (const auto& resultState : transitionResultState)
            {
                result = "";
                result += resultState;
                if (result != "-")
                    currentStates += result;
                std::cout << "delta(" << input << ", " << symbol << ") = " << result << std::endl;
            }
        }
        if (currentStates.size() == 0)
        {
            std::cout << "Nu avem stari rezultate! BLOCAJ!" << std::endl;
            return false;
        }
        std::cout << "Multimea de stari rezultate este: {";
        for (int i = 0; i < currentStates.size(); i++)
        {
            if (i != (currentStates.size() - 1))
                std::cout << currentStates[i] << ", ";
            else
                std::cout << currentStates[i] << "}" << std::endl;
        }
        transitionInputState = currentStates;
    }
    std::cout << "Multimea starilor finale este: {";
    for (int i = 0; i < m_finalStates.size(); i++)
    {
        if (i != (m_finalStates.size() - 1))
            std::cout << m_finalStates[i] << ", ";
        else
            std::cout << m_finalStates[i] << "}" << std::endl;
    }
    for (const auto& state : currentStates)
        if (m_finalStates.find(state) != std::string::npos)
            isAccepted = true;
    if (isAccepted)
    {
        std::cout << "Multimea de stari rezultate contine cel putin o stare finala!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Multimea de stari rezultate NU contine nicio stare finala!" << std::endl;
        return false;
    }
}

bool DeterministicFiniteAutomaton::IsDeterministic()
{
    Unordered_map transitions = m_transitions.GetDeltaFunction();
    for (const auto& element : transitions)
        if (element.second.size() > 1)
            return false;
    return true;
}
