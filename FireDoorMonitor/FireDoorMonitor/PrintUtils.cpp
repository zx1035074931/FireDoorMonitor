#include "PrintUtils.h"

string const cutoff_rule(70, '-');

void logging(const string &logging, bool end){
	cout << logging << endl;
	if(end)
		cout << cutoff_rule << endl;
}
void warning(const string &warning, bool end){
	cout << "���棺" << warning << endl;
	if(end)
		cout << cutoff_rule << endl;
}