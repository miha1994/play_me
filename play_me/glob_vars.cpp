#include "glob_vars.h"
#include "glob.h"
#include <stdio.h>

glob_vars GV ("gv");

static string file_name;

char get_c (FILE *f) {
	if (feof (f)) {
		return 0;
	}
	return getc(f);
}

glob_vars::glob_vars (string file) {
	m_file = file;
	file_name = prefix_path + string("inf/" + file + ".txt");
	FILE *f = fopen (file_name.c_str (), "r");
	if (!f) {
		return;
	}
	char c;
	string cur;
	string typ;
	char s[100];
	while (c = get_c (f)) {
		if (isspace (c)) {
			while ((c = get_c (f)) && isspace (c)) {
			}
		}
		cur = c;
		while ((c = get_c (f)) && !isspace (c)) {
			cur += c;
		}
		while ((c = get_c (f)) && isspace (c)) {
		}
		if (c == '{') {
			string gl = cur;
			while (c = get_c (f)) {
				if (isspace (c)) {
					while ((c = get_c (f)) && isspace (c)) {
					}
				}
				if (c == '}') {
					break;
				}
				cur = c;
				while ((c = get_c (f)) && !isspace (c)) {
					cur += c;
				}
				while ((c = get_c (f)) && isspace (c)) {
				}
				
				fscanf (f, "%s", s+1);
				s[0] = c;
				typ = s;
				SWITCH (typ) 
				CASE ("int") {
					int i;
					fscanf (f, "%d", &i);
					lists[gl].I[cur] = i;
				}
				CASE ("double") {
					double d;
					fscanf (f, "%lf", &d);
					lists[gl].D[cur] = d;
				}
				CASE ("string") {
					fscanf (f, "%s", s);
					lists[gl].S[cur] = s;
				}
				
			}
		} else {
			fscanf (f, "%s", s+1);
			s[0] = c;
			typ = s;
			SWITCH (typ) 
			CASE ("int") {
				int i;
				fscanf (f, "%d", &i);
				glob.I[cur] = i;
			}
			CASE ("double") {
				double d;
				fscanf (f, "%lf", &d);
				glob.D[cur] = d;
			}
			CASE ("string") {
				fscanf (f, "%s", s);
				glob.S[cur] = s;
			}
		}
	}
	fclose (f);
}

void glob_vars::vars::save (FILE *f, char *pref) {
	forstl (p, I) {
		fprintf (f, "%s%s int %d\n", pref, p->first.c_str (), p->second);
	}
	forstl (p, D) {
		fprintf (f, "%s%s double %g\n", pref, p->first.c_str (), p->second);
	}
	forstl (p, S) {
		fprintf (f, "%s%s string %s\n", pref, p->first.c_str (), p->second.c_str ());
	}
}

void glob_vars::save () {
	file_name = prefix_path + string("inf/" + m_file + ".txt");
	FILE *f = fopen (file_name.c_str (), "w");
	glob.save (f, "");
	forstl (p, lists) {
		fprintf (f, "%s {\n", p->first.c_str ());
		p->second.save (f, "\t");
		fprintf (f, "}\n");
	}
	fclose (f);
}