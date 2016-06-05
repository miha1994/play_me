#pragma once
#include "math_3d.h"
#include "fadma.h"
#include <vector>
#include <list>
#include <map>
#include "array_2d.h"

// ширина и высота нашего экранчика
#define D_W		320
#define D_H		240

// это для опенжиэля
#define D_W_norm	(2.0/D_W)
#define D_H_norm	(2.0/D_H)

// координаты расположения окошка
#define D_X_INIT	100
#define D_Y_INIT	30
// масштаб пикселей
#define D_SCALE		4
// запланировать рендеринг спрайта объектом core.m_renderer (добавление в список рендерящихся объектов (в конец))
#define D_ADD_SPRITE(_array, _pos)	do {ren.sprite.rgba_ar = &_array; \
									ren.sprite.pos = _pos; \
									ren.add_sprite_to_render_list (); } while (0)
// запланировать рендеринг спрайта с указанием масштаба (увеличения)
#define D_ADD_SPRITE_SCALE(_array, _pos, _scale)	\
									do {ren.sprite.rgba_ar = &_array; \
									ren.sprite.pos = _pos; \
									ren.sprite.scale = _scale; \
									ren.add_sprite_to_render_list (); \
									ren.sprite.scale = 1; } while (0)

// сокращение обращения к нашему рендерщику
#define ren	core.m_renderer

extern bool g_fullscreen;	// фулскрин?

// рендерщик умеет рисовать наши массивы цветов в реальную текстуру. В нем много опенжиэля
struct Renderer {
	GLuint m_VBO;	// буфер вершин
	GLuint m_IBO;	// буфер индексов
	GLuint m_textureObj;	// текстурка
	rgba_array m_tex_array;	// массив в который мы все рендерим, который в итоге и превращается в реальную текстуру

	// информация о запланированном на рендеринг спрайте
	struct sprite_info {
		rgba_array *rgba_ar;
		v2i pos;
		int scale;
	};
	list <sprite_info> m_render_list;	// список спрайтов (очередь на редеринг)
	sprite_info sprite;			// спрайт будет использоваться для добавления его в очередь. Каждое добавление спрайта в очередь заключается в изменении этого члена и вызова add_sprite_to_render_list ()
	Vertex_tex m_screen[4];			// наш экран в виде четырех вершин. Нужен опенгл при рисовании текстуры
	void draw_everything ();		// рисуем всю очередь m_render_list, создаем текстуру, рисуем ее на экран
	void init ();		//  инициализация
	void add_sprite_to_render_list ();	// добавление спрайта в очередь
};