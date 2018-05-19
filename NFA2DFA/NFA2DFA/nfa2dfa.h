#ifndef NFA_NFA_H
#define NFA_NFA_H

#include <string>
#include <set>
#include <map>

using namespace std;

class nfa
{
	private:
		set<string> alphabet, states, final;
		string initial;

		map< pair<string, string>, set<string> > transitions;
		map< set<string>, set<string> > epsilon_closures;

		string format_output(ostringstream& oss, vector< set<string> >& dfa_states);

	public:
		void set_states(const set<string>& states, const set<string>& initial, const set<string>& final);
		void set_alphabet(const set<string>& alphabet);

		void add_transition(const string& current_state, const string& alphabet, const set<string>& result);

		const set<string> get_states(const string& state, const string& alphabet);//state의 원소 하나씩을 처리하기 위한 함수
		const set<string> get_states(const set<string>& states, const string& alphabet);//set으로 이루어진 set을 반복문으로 위의 함수를 통해 각각 처리

		const set<string> get_epsilon_closure(const string& state);//get_states와 마찬가지로 하나씩 처리하기 위한 함수
		const set<string> get_epsilon_closure(const set<string>& states);

		bool is_final(const set<string>& states);

		string to_dfa();

		string tostring();
};


#endif //NFA_NFA_H
