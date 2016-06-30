#include "camera.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"
#include "cat.h"
#include "input.h"
#include "glob_vars.h"
#include "hot_vars.h"

void camera::render (State state) {
	if (state == State_PLATFORM_REDACTOR) {
		if (m_tmp.m_initiated) {
			D_ADD_SPRITE (m_tmp, m_tmp_pos - m_pos);
		}
		D_ADD_SPRITE (m_map_visualisation, -m_pos);
	} else {
		if (m_draw_in_game_mode) {
			D_ADD_SPRITE (m_map_visualisation, -m_pos);
		}
	}
}

void camera::update (State state, float dt) {
	cat *ct = D_PLATFORM (cat);
	if (state == State_PLATFORM_GAME) {
		if (in.kb['H'].just_pressed) {
			m_draw_in_game_mode = !m_draw_in_game_mode;
		}

		m_pos += v2i (D_W/2, D_H/2);
		v2i catp = ct->m_pos + v2i (13, 0) + v2i ((ct->m_to_right ? 1 : -1) * 70, -25);
		
		int w = 600, h = 40;
		v2i st (-w/2,-h/2);
		v2i cr;
		v2i best = m_pos;
		int best_pr = 0;
		float best_len = length (m_pos - catp);
		FOR_2D (v, w, h) {
			cr = m_pos + st + v;
			if (m_map << cr && (m_map[cr] > best_pr || (m_map[cr] == best_pr && length (cr - catp) < best_len))) {
				best = cr;
				best_pr = m_map[cr];
				best_len = length (cr - catp);
			}
		}
		w = 40, h = 500;
		st = -v2i (w/2,h/2);
		FOR_2D (v, w, h) {
			cr = m_pos + st + v;
			if (m_map << cr && (m_map[cr] > best_pr || (m_map[cr] == best_pr && length (cr - catp) < best_len))) {
				best = cr;
				best_pr = m_map[cr];
				best_len = length (cr - catp);
			}
		}


		catp = ct->m_pos + v2i (13, 0) + v2i ((ct->m_to_right ? 1 : -1) * 25, -25);
		v2i mpos = best;
		w = 120, h = 40;
		st = -v2i (w/2,h/2);
		best_len = 1000;
		FOR_2D (v, w, h) {
			cr = mpos + st + v;
			if (m_map << cr && (m_map[cr] > best_pr || (m_map[cr] == best_pr && length (cr - catp) < best_len))) {
				best = cr;
				best_pr = m_map[cr];
				best_len = length (cr - catp);
			}
		}

		v2f dir = v2f(best) - m_pos;
		if (length (dir) < 1) {
			m_pos += dir * 20 * dt;
		} else if (length (dir) < 10) {
			m_pos += dir / sqrt(length(dir)) * 13 * dt;
		} else {
			m_pos += dir * 6 * dt;
		}

		m_pos -= v2i (D_W/2, D_H/2);
	} else if (state == State_PLATFORM_REDACTOR) {
		if (in.kb.space.just_pressed) {
			m_start_mouse_pos = in.mouse.pos;
			m_tmp_pos = m_pos;
		}
		if (in.kb.space.pressed_now) {
			m_pos = m_tmp_pos - in.mouse.pos + m_start_mouse_pos;
			GV.lists[S_[0]].I["pos_x"] = m_pos.x;
			GV.lists[S_[0]].I["pos_y"] = m_pos.y;
		}
		if (core.m_active_type == Camera) {
			if (in.mouse.left.just_pressed || in.mouse.right.just_pressed) {
				m_start_mouse_pos = in.mouse.pos + m_pos;
			}
			if (in.mouse.left.pressed_now || in.mouse.right.pressed_now) {
				v2i dif = in.mouse.pos + m_pos - m_start_mouse_pos;
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
				m_tmp.clear (CLR::Cyan);
				m_tmp_pos = v1 - (in.mouse.right.pressed_now ? v2i (m_thick,m_thick) : v2i (0,0));
			}
			if (in.mouse.left.just_released || in.mouse.right.just_released) {
				FOR_ARRAY_2D (v, m_tmp) {
					if (m_map << m_tmp_pos + v) {
						m_map[m_tmp_pos + v] = in.mouse.left.just_released ? 1 : 0;
					}
				}
				FOR_ARRAY_2D (v, m_map) {
					m_map_visualisation [v] = m_map[v] ? CLR (0,255,0,120) : CLR(0,0,0,0);
				}
				m_tmp.destroy ();
			}
			if (in.kb.up.just_pressed || in.kb.down.just_pressed) {
				m_thick += in.kb.up.just_pressed ? 1 : -1;
				m_thick = Max (m_thick, 0);
			}
		}

		if (in.kb.ctrl.pressed_now && in.kb['S'].just_pressed) {
			FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_camera.inf")).c_str (), "wb");
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
		}
	}
}

void camera::clean () {
	//PUT YOUR CODE HERE
}

void camera::load () {
	m_draw_in_game_mode = false;

	int w, h;
	if (D_CONTAINES ( GV.lists, S_[0])) {
		auto v = GV.lists[S_[0]];

		m_pos = v2i (v.I["pos_x"], v.I["pos_y"]);
		m_map.init (w = v.I["w"], h = v.I["h"]);
		
		FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_camera.inf")).c_str (), "rb");
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
			m_map[v2i (D_W, D_H)/2] = 1;
		}
	} else {
		GV.lists[S_[0]].I["posx"] = GV.lists[S_[0]].I["posy"] = 0;
		GV.lists[S_[0]].I["w"] = D_W;
		GV.lists[S_[0]].I["h"] = D_H;
		m_map.init (w = D_W, h = D_H);
		m_map.clear (0);
		m_map[v2i (D_W, D_H)/2] = 1;

		m_pos = v2i (0, 0);
	}
	m_thick = 2;

	m_map_visualisation.init (w, h);
	m_map_visualisation.alpha_matters = true;
	FOR_ARRAY_2D (v, m_map) {
		m_map_visualisation [v] = m_map[v] ? CLR (0,255,0,120) : CLR(0,0,0,0);
	}

	read_string ();	// "}"
}