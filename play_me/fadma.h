#pragma once
#define VERSION		1.051
#include <stdio.h>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;

#define Pi						3.1415926535									// константы
#define e_						2.7182818284
#define log_(Base, LogValue)	(log (double (LogValue)) / log (double (Base))) // логарифмы
#define lb_(LogValue)			log_ (2, LogValue)						// двоичный
#define lg_(LogValue)			log_ (10, LogValue)						// десятичный
#define ln_(LogValue)			log_ (e_, LogValue)						// натуральный
inline double rad_ (double degree);			// перевод угла из градусов в радианы
inline double deg_ (double radian);		// перевод угла из радиан в градусы

#define _FOR_(I,INITVALUE,N_)	for (I = (INITVALUE); I < (N_); ++I)		// часто используемые конструкции
#define _FOR(I,N_)				_FOR_ (I, 0, N_)							// с использованием цикла for
#define forstl(p,stl)			for (auto p = (stl).begin (), end = (stl).end (); p != end; ++p)
#define forstl_p(p,stl)			for (auto p : (stl))

#define FOR_(I,INITVALUE,N_)	for (int I = (INITVALUE), variable_N = (N_); I < variable_N; ++I)
#define FOR(I,N_)				FOR_ (I, 0, N_)
#define For(N_)					FOR (count_variable_for_loop_, N_)
#define _n_						<< ' ' <<
#define SWITCH(Var)				auto cpy_for_switch = Var; if (false) {}
#define CASE(Value)				else if (cpy_for_switch == Value)
#define CASE2(Value, Value2)	else if (cpy_for_switch == Value || cpy_for_switch == Value2)
#define DEFAULT					else if (1)

// минимум и максимум
#define Max(a,b)				(((a)<(b))?(b):(a))
#define Min(a,b)				(((a)>(b))?(b):(a))

#define error(text)	do {cout << "\n\nERROR : " + string (text) + "\n"; } while (0)

// вызывает ошибку деления на ноль
#define ERR_BREAK	fadma_zero_for_err_break = (1 / fadma_zero_for_err_break)

extern int fadma_zero_for_err_break;