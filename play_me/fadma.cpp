#include "fadma.h"

inline double rad_ (double degree) {return degree * Pi / 180;}			// перевод угла из градусов в радианы
inline double deg_ (double radian) {return radian * 180 / Pi;}			// перевод угла из радиан в градусы

int fadma_zero_for_err_break = 0;