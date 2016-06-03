#include "direction.h"

/*
#define D_UP	0
#define D_RIGHT	1
#define D_DOWN	2
#define D_LEFT	3
*/

v2i g_n_to_dir[4] = {
	v2i (0, -1),
	v2i (1, 0),
	v2i (0, 1),
	v2i (-1, 0)
};

int g_dir_to_n (v2i dir) {
	if (dir.x) {
		return dir.x > 0 ? D_RIGHT : D_LEFT;
	}
	return dir.y > 0 ? D_DOWN : D_UP;
}

#define ABS(a)	((a) > 0 ? (a) : -(a))

int g_dir_to_n (v2f dir) {
	if (dir.x && dir.y) {
		v2f abs (ABS(dir.x), ABS (dir.y));
		if (abs.x > abs.y) {
			dir.y = 0;
		} else {
			dir.x = 0;
		}
	}
	if (dir.x) {
		return dir.x > 0 ? D_RIGHT : D_LEFT;
	}
	return dir.y > 0 ? D_DOWN : D_UP;
}