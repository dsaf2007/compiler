#include "parser.h"

#include <fstream>
#include <iostream>
#include "utility.cpp"

parser::parser(const string& filename) //파일을 읽어들여 줄단위로 받아서 
{
    ifstream file(filename.c_str());

    if (!file.is_open())
        throw "File I/O error : Error opening file!";

    string line;
    while ( getline(file,line) ) 
	{
        if(strip(line).length() <= 0)
            continue;

        vector<string> splitted = split(line, ':');
        if(splitted.size() != 2)
            throw "Parse Error : A single line must consist one and only one ':' symbol!";


        string label = strip(splitted[0]);
        string contained = splitted[1];

        if(label != "Q0") // state가 set이 아닐경우
            contained = get_contained(contained, '{', '}');

        symbolmap[label] = strip(contained);
    }

    file.close();
}

string get_item(const map<string, string>& m, const string& key)//원소 return
{
    string error_message = "NFA Parse Error : '" + key + "' label not found!";

    if(m.count(key))
        return m.at(key);
    else
        throw error_message;
}

set<string> parser::get_set_items(const string& key) //집합의 원소들을 return
{
    vector<string> items = split(get_item(symbolmap, key), ',');

    set<string> item_set(items.begin(), items.end());
    symbolmap.erase(key);

    return item_set;
}

nfa parser::create_nfa()//불러온 파일로 부터 nfa생성
{
    string error_message = "NFA Parse Error : ";

    set<string> states = get_set_items("Q");
    set<string> alphabet = get_set_items("E");
    set<string> initial = get_set_items("Q0");
    set<string> final = get_set_items("F");

    nfa automata;
    automata.set_states(states, initial, final);
    automata.set_alphabet(alphabet);

    alphabet.insert("e"); // alphabet에 epsilon move추가
    set<string>::const_iterator i, j;
    for (i = states.begin(); i != states.end(); ++i) 
	{

        for (j = alphabet.begin(); j != alphabet.end(); ++j) 
		{
            string key = "T(" + *i + "," + *j + ")";

            try {
                automata.add_transition(*i, *j, get_set_items(key));
            } catch (const string& message)
			{
                cout << "Transition Error : Required Transition for state '"
                << *i << "' on alphabet '" << *j << "' not defined!" << endl;
                throw;
            }
        }

    }
    alphabet.erase("e"); // epsilon move 를 alphabet에서 삭제

    if(symbolmap.size()>0) 
	{
        cout << "Input File contains the  following undefined transitions :" << endl;
        map<string, string>::const_iterator iter;
        for (iter = symbolmap.begin(); iter!=symbolmap.end(); ++iter) 
		{
            cout << iter->first << " -> " << iter->second << endl;
        }

        throw error_message + "Undefined transitions!";
    }


    return automata;
}