#include "solid.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"
#include "input.h"
#include "camera.h"
#include "hot_vars.h"
#include "glob_vars.h"
#include "png.h"

CLR col_table [] = {
	CLR(0,0,0,0),
	CLR (255,255,255,170),
	CLR (255,0,0,170)
};

void solid::render (State state) {
	camera *cam = D_PLATFORM (camera);
	if (state == State_PLATFORM_REDACTOR) {
		if (m_tmp.m_initiated) {
			D_ADD_SPRITE (m_tmp, m_tmp_pos - cam->m_pos);
		}
		D_ADD_SPRITE (m_map_visualisation, -cam->m_pos);
	} else {
		if (m_draw_in_game_mode) {
			D_ADD_SPRITE (m_map_visualisation, -cam->m_pos);
		}
	}
}

void solid::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	if (state == State_PLATFORM_GAME) {
		if (in.kb['H'].just_pressed) {
			m_draw_in_game_mode = !m_draw_in_game_mode;
		}
	} else if (state == State_PLATFORM_REDACTOR) {
		if (core.m_active_type == Solid) {
			
			if (in.mouse.left.just_pressed || in.mouse.right.just_pressed) {
				m_start_mouse_pos = in.mouse.pos + cam->m_pos;
			}
			if (in.mouse.left.pressed_now || in.mouse.right.pressed_now) {
				v2i dif = in.mouse.pos + cam->m_pos - m_start_mouse_pos;
				v2i abs (Abs (dif.x), Abs (dif.y));
				if (abs.x > abs.y) {
					dif.y = 1;
					abs.y = 1;
				} else {
					dif.x = 1;
					abs.x = 1;
				}
				if (!abs.x) {
					abs.x = 1;
					dif.x = 1;
				}
				if (!abs.y) {
					abs.y = 1;
					dif.y = 1;
				}
				v2i v1 (m_start_mouse_pos), v2 (m_start_mouse_pos + dif);
				if (v1.x > v2.x) {
					swap (v1.x, v2.x);
				}
				if (v1.y > v2.y) {
					swap (v1.y, v2.y);
				}
				m_tmp.init (abs.x + (in.mouse.right.pressed_now ? 2 * m_thick : 0), abs.y + (in.mouse.right.pressed_now ? 2 * m_thick : 0));
				CLR tmp_col = col_table[m_color];
				tmp_col.a = 255;
				m_tmp.clear (tmp_col);
				m_tmp_pos = v1 - (in.mouse.right.pressed_now ? v2i (m_thick,m_thick) : v2i (0,0));
			}
			if (in.mouse.left.just_released || in.mouse.right.just_released) {
				FOR_ARRAY_2D (v, m_tmp) {
					if (m_map << m_tmp_pos + v) {
						m_map[m_tmp_pos + v] = in.mouse.left.just_released ? m_color : 0;
					}
				}
				FOR_ARRAY_2D (v, m_map) {
					m_map_visualisation [v] = col_table[m_map[v]];
				}
				m_tmp.destroy ();
			}
			if (in.kb.up.just_pressed || in.kb.down.just_pressed) {
				m_thick += in.kb.up.just_pressed ? 1 : -1;
				m_thick = Max (m_thick, 0);
			}
			if (in.kb['1'].just_pressed) {
				m_color = 1;
			}
			if (in.kb['2'].just_pressed) {
				m_color = 2;
			}
		}
		if (in.kb.ctrl.pressed_now && in.kb['S'].just_pressed) {
			FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_solid.inf")).c_str (), "wb");
			//hot_vars.string_
			int count = 0;
			int val = m_map[v2i(0,0)];
			FOR_ARRAY_2D (v, m_map) {
				if (m_map[v] != val) {
					Write (f, val);
					Write (f, count);
					val = m_map[v];
					count = 0;
				}
				++count;
			}
			Write (f, val);
			Write (f, count);
			fclose (f);
			save_png (m_map_visualisation, prefix_path + "tex/" + S_[0] + "/solid_visualisation.png");
		}
	}
}

void solid::clean () {
	//PUT YOUR CODE HERE
}

void solid::load () {
	m_color = 1;
	m_draw_in_game_mode = false;
	m_thick = 2;

	int w,h;
	if (D_CONTAINES ( GV.lists, S_[0])) {
		auto v = GV.lists[S_[0]];

		m_map.init (w = v.I["w"], h = v.I["h"]);
		
		FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_solid.inf")).c_str (), "rb");
		if (f) {
			int count = 0;
			int val = 0;
			FOR_ARRAY_2D (v, m_map) {
				if (!count) {
					Read (f, val);
					Read (f, count);
				}
				m_map[v] = val;
				--count;
			}
			fclose (f);
		} else {
			m_map.init (w, h);
			m_map.clear (0);
		}
	} else {
		m_map.init (w = D_W, h = D_H);
		m_map.clear (0);
		m_map[v2i (D_W, D_H)/2] = 1;
	}

	m_map_visualisation.init (w, h);
	m_map_visualisation.alpha_matters = true;
	FOR_ARRAY_2D (v, m_map) {
		m_map_visualisation [v] = col_table[m_map[v]];
	}

	read_string (); // "}"
}