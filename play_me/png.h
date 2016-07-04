#pragma once
#include <string>
#include "array_2d.h"

using namespace std;

bool load_png (rgba_array &texture, string file_name);
bool load_png (rgba_array &texture, string file_name, IntRect rect);
bool save_png (rgba_array &texture, string file_name);