#include <queue>
#include <iostream>
#include <sstream>
#include "nfa2dfa.h"
#include "utility.cpp"

string error_message = "NFA Creation Error : ";

void verify_state_set(const set<string>& states, const set<string>& state_set) 
{

    set<string>::const_iterator it;
    for (it = state_set.begin(); it != state_set.end(); ++it)
	{
        if(!states.count(*it))
            throw error_message + "State '" + *it + "' does not belong to Q!";
    }
}

//initial과 final의 모든 state가 state vector에 속하는지 확인.
void verify_states(set<string> states, set<string> initial, set<string> final) 
{
    if(initial.size() != 1)
        throw error_message + "There should be one and only one initial state!";

    if(final.size() < 1 || final.size() > states.size() )
        throw error_message + "Final states should be between 1 and total number of states!";

   

    if (!states.count(*initial.begin()))
        throw error_message + "Initial state '" + *initial.begin() + "' does not belong to Q!";

    try 
	{
        verify_state_set(states, final);
    } catch (const string& message) 
	{
        cout << "Invalid Final State!" << endl;
        throw;
    }
}

void nfa::set_states(const set<string> &states, const set<string> &initial, const set<string> &final) //state 설정
{
    verify_states(states, initial, final);

    this->states = states;
    this->final = final;
    this->initial = *initial.begin();
}

void nfa::set_alphabet(const set<string> &alphabet) //alphabet 설정
{
    if(alphabet.count("e"))
        throw "NFA Parse Error : Language alphabet 'e' clashes with keyword for epsilon!";

    this->alphabet = alphabet;
}

void nfa::add_transition(const string& current_state, const string& alphabet, const set<string>& result) //transition을 추가
{
    verify_state_set(states, result);
    transitions[make_pair(current_state, alphabet)] = result;
}

const set<string> nfa::get_states(const string& state, const string& alphabet)//state를 받는다. set안의 원소를 처리하기 위한 함수
{
    pair<string, string> key = make_pair(state, alphabet);
    if(!transitions.count(key))
        throw "NFA Error : Invalid Transition!";

    return transitions[key];
}

const set<string> nfa::get_states(const set<string> &states, const string &alphabet)//state를 받는다
{
    set<string> compound_states;

    set<string>::const_iterator it;
    for(it = states.begin(); it != states.end(); ++it) 
	{
        set<string> out_state = get_states(*it, alphabet);
        compound_states.insert(out_state.begin(), out_state.end());
    }

    return compound_states;
}

const set<string> nfa::get_epsilon_closure(const string &state) //set의 원소를 처리하기 위한 함수
{
    set<string> key;
    key.insert(state);

    if(epsilon_closures.count(key))
        return epsilon_closures[key];

    set<string> epsilon_states;

    queue<string> remaining;
    remaining.push(state);

    while (!remaining.empty()) {
        string current = remaining.front();

        epsilon_states.insert(current);

        set<string> epsilon_moves = get_states(current, "e");
        set<string>::const_iterator it;
        for(it = epsilon_moves.begin(); it != epsilon_moves.end(); ++it)
		{
            if(!epsilon_states.count(*it))
                remaining.push(*it);
        }

        remaining.pop();
    }

    epsilon_closures[key] = epsilon_states;

    return epsilon_states;
}

const set<string> nfa::get_epsilon_closure(const set<string> &states)//set으로 된 e-closure을 받아 처리 get_epsilon_closure(const string &state)를 이용한다.
{
    if (epsilon_closures.count(states))
        return epsilon_closures[states];

    set<string> epsilon_states;

    set<string>::const_iterator it;
    for(it = states.begin(); it != states.end(); ++it) 
	{
        set<string> current_states = get_epsilon_closure(*it);
        epsilon_states.insert(current_states.begin(), current_states.end());//epsilon_states에 입력.
    }

    epsilon_closures[states] = epsilon_states;

    return epsilon_states;
}

bool nfa::is_final(const set<string> &states) //final state를 확인하여 true false반환
{
    set<string>::const_iterator it;

    for(it = states.begin(); it != states.end(); ++it) 
	{
        if(contains(final, *it))
            return true;
    }

    return false;
}

string nfa::format_output(ostringstream& oss, vector< set<string> >& dfa_states)//output format
{
    cout << "\nInitial State : " << endl;
    string initial_state = to_string(dfa_states[0], ',');
    cout << "{ " << initial_state << " }" << endl;

    oss << (string) "Q0 : { " << initial_state << " }" << endl;
    oss << "F : {";

    cout << "Final States : " << endl;
    string prefix = "Q : {";
    const char* delim = " ";
    for (int i = 0; i < dfa_states.size(); ++i) 
	{
        set<string> current = dfa_states[i];

        prefix+= " { " + to_string(current, ',') + " },";

        if(is_final(current))
		{
            string final_state = to_string(current, ',');
            cout << "{ " << final_state << " }" << endl;

            oss << delim << "{ " << final_state << " }";
            delim = ", ";
        }
    }
    oss << " }" << endl;

    // state와 language를 output에 추가
    unsigned long index = prefix.length()-1;
    if(prefix[index] == ',')
        prefix.erase(prefix.begin() + index);
    prefix += " }\n";
    prefix += "E : { " + to_string(alphabet, ',') + " }\n";

    return prefix + "\n" + oss.str();
}

string nfa::to_dfa()//nfa to dfa함수
{
    ostringstream oss;
    cout << "DFA Conversion : \n" << endl;
    set<string> initial = get_epsilon_closure(this->initial);

    queue< set<string> > remaining;
    remaining.push(initial);

    vector< set<string> > dfa_states;

    set<string>::const_iterator it;

    while(!remaining.empty())//remaining이 빌 때까지
	{
        set<string> current = remaining.front();
        remaining.pop();

        if(contains(dfa_states, current))
            continue;//dfa state를 포함하는 경우 다음 index로

        dfa_states.push_back(current);

        string current_set = to_string(current, ',');//current set에 current 입력
        cout << "For state : { " << current_set << " }" << endl;
        for (it = alphabet.begin(); it != alphabet.end(); ++it)
		{
            set<string> epsilon_state = get_epsilon_closure(get_states(current, *it));//current의 state에서 e-closure을 받는다

            string epsilon_set = to_string(epsilon_state, ',');
            cout << "    " << *it << " -> { " << epsilon_set << " }" << endl;

            oss << (string) "T({ " << current_set << " }, " << *it << " ) : { " << epsilon_set << " }" << endl;

            if (!contains(dfa_states, epsilon_state))
                remaining.push(epsilon_state);
        }

        cout << endl;
        oss << endl;
    }

    return format_output(oss, dfa_states);
}

const string get_transition_string(const map< pair<string, string>, set<string> >& m) //transition string을 받아 하나의 string으로 만들어 return
{
    string output;
    map< pair<string, string>, set<string> >::const_iterator it;

    for(it = m.begin(); it != m.end(); ++it) 
	{
        output += "(" + it->first.first + ", " + it->first.second + ") -> { " + to_string(it->second, ',') + " }\n";
    }

    return output;
}

string nfa::tostring() //utils.cpp 의 to_string을 이용하여 각각 string으로 변환
{
    return "Alphabet:\n"
            "{ " + to_string(alphabet, ',') + " }"
            "\n\nStates:\n"
            "{ " + to_string(states, ',') + " }"
            "\n\nInitial State : \n" +
            initial +
            "\n\nFinal States:\n"
            "{ " + to_string(final, ',') + " }"
            "\n\nTransitions:\n" +
            get_transition_string(transitions);
}