#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "fadma.h"

extern string prefix_folder;	// корнева€ папка, содержаща€ папку с ресурсами
extern string prefix_path;		// путь к папке с ресурсами (текстурами, шрифтами, звуками и информацией в текстовом и другом виде)
extern GLuint gSampler;	// deep and dark opengl

#define D_(v)	(v).x][(v).y		// используетс€ дл€ замены вот такого кода:  v2i v; int a[10][10]; a[v.x][v.y] = 0;   на такой: a[D_(v)]
#define Texture_name(name)	(prefix_folder + "res/tex/" + string (name) + ".png")	// преобразователь имени текстуры в полное название файла текстуры

#define CLR		sf::Color

#define rand1	(rand () / float (RAND_MAX))

CLR operator * (CLR c, float f);		// умножение цвета на скал€р
CLR operator ^ (CLR c, CLR s);			// сложение цветов. нельз€ использовать +, так как он уже перегружен в sfml, получитс€ неоднозначность