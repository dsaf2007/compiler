#ifndef NFA_NFA_PARSER_H
#define NFA_NFA_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "nfa2dfa.h"

using namespace std;

class parser 
{
	private:
		map<string, string> symbolmap;
		set<string> get_set_items(const string& key);

	public:
		explicit parser(const string& filename);
		nfa create_nfa();

};


#endif //NFA_NFA_PARSER_H
