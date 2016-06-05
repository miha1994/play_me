#include <stdio.h>
#include "math_3d.h"
#include "core.h"
#include "pure_opengl.h"
#include "glob.h"

#define CONSOLE_MAIN int main (int argc, char **argv) { glutInit (&argc, argv);
#define NO_CONSOLE_MAIN int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int       cmdShow) {int argc = 0;	glutInit (&argc, 0);

CONSOLE_MAIN
	glut_initialization ();

    glutMainLoop();

    return 0;
}