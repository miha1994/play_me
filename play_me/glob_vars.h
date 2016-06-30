#pragma once
#include <map>
#include <string>

using namespace std;

struct glob_vars {
	struct vars {
		map <string, int> I;
		map <string, double> D;
		map <string, string> S;
		void save (FILE *f, char *pref);
	};
	string m_file;
	vars glob;
	map <string, vars> lists;

	glob_vars (string file);
	void save ();
};

extern glob_vars GV;