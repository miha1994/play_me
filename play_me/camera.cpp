#include "camera.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"
#include "cat.h"
#include "input.h"

void camera::render (State state) {
	if (state == State_PLATFORM_REDACTOR) {
		if (m_tmp.m_initiated) {
			D_ADD_SPRITE (m_tmp, m_tmp_pos - m_pos);
		}
	}
	D_ADD_SPRITE (m_map_visualisation, -m_pos);
}

void camera::update (State state, float dt) {
	cat *ct = D_PLATFORM (cat);
	if (state == State_PLATFORM_GAME) {
		m_pos += v2i (D_W/2, D_H/2);
		v2i catp = ct->m_pos + v2i ((ct->m_to_right ? 1 : -1) * 9, -9);
		
		int w = 300, h = 40;
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
		v2f dir = v2f(best) - m_pos;
		if (length (dir) < 1) {
			m_pos += dir * 20 * dt;
		} else if (length (dir) < 10) {
			m_pos += dir / sqrt(length(dir)) * 13 * dt;
		} else {
			m_pos += dir * 4 * dt;
		}

		m_pos -= v2i (D_W/2, D_H/2);
	} else if (state == State_PLATFORM_REDACTOR && core.m_active_type == Camera) {
		if (in.kb.space.just_pressed) {
			m_start_mouse_pos = in.mouse.pos;
			m_tmp_pos = m_pos;
		}
		if (in.kb.space.pressed_now) {
			m_pos = m_tmp_pos - in.mouse.pos + m_start_mouse_pos;
		}
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
			m_tmp.init (abs.x, abs.y);
			m_tmp.clear (CLR::Cyan);
			m_tmp_pos = v1;
		}
		if (in.mouse.left.just_released || in.mouse.right.just_released) {
			FOR_ARRAY_2D (v, m_tmp) {
				if (m_map << m_tmp_pos + v) {
					m_map[m_tmp_pos + v] = in.mouse.left.just_released ? 1 : 0;
				}
			}
			FOR_ARRAY_2D (v, m_map) {
				m_map_visualisation [v] = m_map[v] ? CLR (0,255,255,120) : CLR(0,0,0,0);
			}
			m_tmp.destroy ();
		}
	}
}

void camera::clean () {
	//PUT YOUR CODE HERE
}

void camera::load () {
	m_pos = v2i (0, 0);

	m_map.init (D_W, D_H);
	m_map.clear (0);
	m_map[v2i (D_W, D_H)/2] = 1;

	m_map_visualisation.init (D_W, D_H);
	m_map_visualisation.alpha_matters = true;
	m_map_visualisation.clear (CLR (0,0,0,0));
	m_map_visualisation[v2i (D_W, D_H)/2] = CLR (0,255,255,120);

	read_string ();	// "}"
}