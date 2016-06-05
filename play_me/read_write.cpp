#include "read_write.h"
#include "fadma.h"
#include <stdio.h>
#include <ctype.h>

static void skip_spaces_and_comments () {
	char c;
	while (isspace (c = getchar ()) || c=='#') {
		if(c=='#') {
			while ((c = getchar()) != '\n')
				;
		}
	}
	ungetc (c, stdin);
}

char read_char () {
	skip_spaces_and_comments ();
	return getchar ();
}

int read_int () {
	skip_spaces_and_comments ();
	int i;
	scanf ("%d", &i);
	return i;
}

float read_float () {
	skip_spaces_and_comments ();
	float f;
	scanf ("%f", &f);
	return f;
}

v2f read_vector_2f () {
	skip_spaces_and_comments ();
	v2f v;
	scanf ("%f", &v.x);
	skip_spaces_and_comments ();
	scanf ("%f", &v.y);
	return v;
}

v2i read_vector_2i () {
	skip_spaces_and_comments ();
	v2i v;
	scanf ("%d", &v.x);
	skip_spaces_and_comments ();
	scanf ("%d", &v.y);
	return v;
}

std::string read_string () {
	skip_spaces_and_comments ();
	std::string s;
	char c;
	while (!(isspace (c = getchar ()) || c == EOF))
		s.push_back (c);
	return s;
}

int indent;
#define INDENT	For (indent) printf("\t")
#define COMMENT if(comment != "") printf("\t\t#%s\n", comment.c_str()); else printf ("\n")

void write (char c, std::string comment) {
	if (c == '}') {
		--indent;
	}
	INDENT;
	if (c == '{') {
		indent++;
	}
	printf ("%c", c);
	COMMENT;
}

void write (std::string s, std::string comment) {
	for (auto c : s) {
		if (c == '}') {
			--indent;
		}
	}
	INDENT;
	for (auto c : s) {
		if (c == '{') {
			indent++;
		}
	}
	printf ("%s", s.c_str ());
	COMMENT;
}

void write (int i, std::string comment) {
	INDENT;
	printf ("%d", i);
	COMMENT;
}

void write (float f, std::string comment) {
	INDENT;
	printf ("%f", f);
	COMMENT;
}

void write (v2f v, std::string comment) {
	INDENT;
	printf ("%f %f", v.x, v.y);
	COMMENT;
}

void write (v2i v, std::string comment) {
	INDENT;
	printf ("%d %d", v.x, v.y);
	COMMENT;
}