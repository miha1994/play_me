#include "solid.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"
#include "input.h"

void solid::render (State state) {
	if (state == State_PLATFORM_REDACTOR) {
		if (m_tmp.m_initiated) {
			D_ADD_SPRITE (m_tmp, m_tmp_pos);
		}
	}
	D_ADD_SPRITE (m_map_visualisation, v2i (0,0));
}

void solid::update (State state, float dt) {
	if (state == State_PLATFORM_GAME) {
		
	} else if (state == State_PLATFORM_REDACTOR && core.m_active_type == Solid) {
		if (in.mouse.left.just_pressed || in.mouse.right.just_pressed) {
			m_start_mouse_pos = in.mouse.pos;
		}
		if (in.mouse.left.pressed_now || in.mouse.right.pressed_now) {
			v2i dif = in.mouse.pos - m_start_mouse_pos;
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
			m_tmp.clear (CLR::White);
			m_tmp_pos = v1;
		}
		if (in.mouse.left.just_released || in.mouse.right.just_released) {
			FOR_ARRAY_2D (v, m_tmp) {
				if (m_map << m_tmp_pos + v) {
					m_map[m_tmp_pos + v] = in.mouse.left.just_released;
				}
			}
			FOR_ARRAY_2D (v, m_map) {
				m_map_visualisation [v] = m_map[v] ? CLR (255,255,255,120) : CLR(0,0,0,0);
			}
			m_tmp.destroy ();
		}
	}
}

void solid::clean () {
	//PUT YOUR CODE HERE
}

void solid::load () {
	m_map.init (D_W, D_H);
	m_map.clear (0);

	m_map_visualisation.init (D_W, D_H);
	m_map_visualisation.alpha_matters = true;
	m_map_visualisation.clear (CLR(0,0,0,0));

	read_string (); // "}"
}