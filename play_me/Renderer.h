#pragma once
#include "math_3d.h"
#include "fadma.h"
#include <vector>
#include <list>
#include <map>
#include "array_2d.h"

// ������ � ������ ������ ���������
#define D_W		320
#define D_H		240

// ��� ��� ���������
#define D_W_norm	(2.0/D_W)
#define D_H_norm	(2.0/D_H)

// ���������� ������������ ������
#define D_X_INIT	100
#define D_Y_INIT	30
// ������� ��������
#define D_SCALE		4
// ������������� ��������� ������� �������� core.m_renderer (���������� � ������ ������������ �������� (� �����))
#define D_ADD_SPRITE(_array, _pos)	do {ren.sprite.rgba_ar = &_array; \
									ren.sprite.pos = _pos; \
									ren.add_sprite_to_render_list (); } while (0)

// ������������� ��������� ������� � ��������� �������� (����������)
#define D_ADD_SPRITE_SCALE(_array, _pos, _scale)	\
									do {ren.sprite.rgba_ar = &_array; \
									ren.sprite.pos = _pos; \
									ren.sprite.scale = _scale; \
									ren.add_sprite_to_render_list (); \
									ren.sprite.scale = 1; } while (0)

#define D_ADD_SPRITE_ALPHA(_array, _pos, _alpha)	\
									do {ren.sprite_alpha.rgba_ar = &_array; \
									ren.sprite_alpha.pos = _pos; \
									ren.sprite_alpha.alpha1 = _alpha; \
									ren.m_render_alpha_list.push_back (ren.sprite_alpha); \
									} while (0)

#define D_ADD_SP_EFFECT_SQUARE(_pos, _radius, _thickness) \
									do {					\
									ren.sp_effect_square.pos = _pos;	\
									ren.sp_effect_square.radius = _radius;	\
									ren.sp_effect_square.thickness = _thickness;	\
									ren.m_sp_effect_square.push_back (ren.sp_effect_square);	\
									} while (0)

// ���������� ��������� � ������ ����������
#define ren	core.m_renderer

extern bool g_fullscreen;	// ��������?

// ��������� ����� �������� ���� ������� ������ � �������� ��������. � ��� ����� ���������
struct Renderer {
	GLuint m_VBO;	// ����� ������
	GLuint m_IBO;	// ����� ��������
	GLuint m_textureObj;	// ���������
	rgba_array m_tex_array;	// ������ � ������� �� ��� ��������, ������� � ����� � ������������ � �������� ��������

	// ���������� � ��������������� �� ��������� �������
	struct sprite_info {
		rgba_array *rgba_ar;
		v2i pos;
		int scale;
	};
	struct sprite_alpha_info {
		rgba_array *rgba_ar;
		v2i pos;
		float alpha1;
	};
	struct sp_effect_square_info {
		v2i pos;
		int radius;
		int thickness;

		void apply (rgba_array &ar);
	};
	list <sprite_info> m_render_list;	// ������ �������� (������� �� ��������)
	sprite_info sprite;			// ������ ����� �������������� ��� ���������� ��� � �������. ������ ���������� ������� � ������� ����������� � ��������� ����� ����� � ������ add_sprite_to_render_list ()

	list <sprite_alpha_info> m_render_alpha_list;
	sprite_alpha_info sprite_alpha;

	Vertex_tex m_screen[4];			// ��� ����� � ���� ������� ������. ����� ������ ��� ��������� ��������
	list <sp_effect_square_info> m_sp_effect_square;
	sp_effect_square_info sp_effect_square;
	void draw_everything ();		// ������ ��� ������� m_render_list, ������� ��������, ������ �� �� �����
	void init ();		//  �������������
	void add_sprite_to_render_list ();	// ���������� ������� � �������
};