#include <iostream>
#include <fstream>
#include "nfa_parser.h"

using namespace std;

void create_dfa(const string& input,const string& output)
{
	string nfa_filename = input;
    string dfa_filename = output;


        cout << "Loading NFA File : " << nfa_filename << endl;
        nfa_parser parser(nfa_filename);
        nfa automata = parser.create_nfa();

        cout << "NFA:\n\n" << automata.tostring() << endl << endl;

        string dfa = automata.to_dfa();
        ofstream dfa_file(dfa_filename.c_str());
        dfa_file << dfa;
        dfa_file.close();

        cout << "File written to : " << dfa_filename << endl;
        cout << "---------------" << endl;

}

int main()
{
	string inputfile, outputfile;
	cout << "input file(folder+filename.txt):";
	cin >> inputfile;
	cout << "output file(folder+filename.txt):";
	cin >> outputfile;
    cout << "---------------" << endl;
    create_dfa(inputfile,outputfile);

  
    return 0;
}