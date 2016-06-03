#include "glob.h"

string prefix_folder = "D:/";
string prefix_path = "D:/res/";
GLuint gSampler;

CLR operator * (CLR c, float f) {
	c.r = Min (255, c.r * f);
	c.g = Min (255, c.g * f);
	c.b = Min (255, c.b * f);
	return c;
}

CLR operator ^ (CLR c, CLR s) {
	c.r = Min (255, int(c.r) + s.r);
	c.g = Min (255, c.g + int(s.g));
	c.b = Min (255, c.b + int (s.b));
	return c;
}