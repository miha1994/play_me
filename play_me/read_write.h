#pragma once

#include "math_3d.h"
#include <string>

// весь этот модуль используется для парсинга текстовых файлов и вывода в определенном формате в файлы

char read_char ();
std::string read_string ();
int read_int ();
float read_float ();
v2f read_vector_2f ();
v2i read_vector_2i ();

void write (char c, std::string comment = "");
void write (std::string s, std::string comment = "");
void write (int i, std::string comment = "");
void write (float f, std::string comment = "");
void write (v2f v, std::string comment = "");
void write (v2i v, std::string comment = "");