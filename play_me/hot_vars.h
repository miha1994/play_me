#pragma once
#include <vector>
#include <string>

/*
_______________________
***********************
 EXAMPLE OF USING:
_______________________
***********************

#include "hot_vars.h"

double f () {
    double d =  I_[0]  /  D_[2];
}



__________________________
**************************
 EXAMPLE OF HOT_VARS.TXT:
__________________________
**************************
%double
0.3 #radius_regulator
0.2  #explosion_time_scaling
0.2
1.5
75

%int

0 0 255
255 0 0

%string
"consolas.ttf"
"pos"
"time"
"hints"
*/

struct ___hot_vars {
    std::vector <double> double_;
    std::vector <int> int_;
    std::vector <std::string> string_;
    ___hot_vars ();
};

extern ___hot_vars hot_vars;

#define I_		hot_vars.int_
#define D_		hot_vars.double_
#define S_		hot_vars.string_