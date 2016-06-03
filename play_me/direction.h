#pragma once
#include "math_3d.h"

// определение четырех направлений
#define D_UP	0
#define D_RIGHT	1
#define D_DOWN	2
#define D_LEFT	3

// массив векторов, соответствующих четырем направлениям
extern v2i g_n_to_dir[4];

// вектор => номер направления
int g_dir_to_n (v2i dir);
int g_dir_to_n (v2f dir);