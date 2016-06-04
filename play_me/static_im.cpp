#include "static_im.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"
#include "camera.h"
#include "input.h"
#include "solid.h"

void static_im::render (State state) {
	camera *cam = D_PLATFORM (camera);
	D_ADD_SPRITE (m_bg, -cam->m_pos);
}

void static_im::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	solid *sol = D_PLATFORM (solid);
	if (state == State_PLATFORM_GAME) {
		
	} else if (state == State_PLATFORM_REDACTOR && core.m_active_type == Static_im) {
		if (in.kb.ctrl.pressed_now) {
			if (in.kb['M'].just_pressed) {
				v2i nsize (in.mouse.pos + cam->m_pos);
				int w = nsize.x, h = nsize.y;
				auto cpy = m_bg;
				m_bg.destroy ();
				m_bg.init (w, h);
				m_bg.clear (CLR (140,140,140,255));
				m_bg.draw (&cpy, v2i(0,0));
				auto cpy4 = cam->m_map_visualisation;
				cam->m_map_visualisation.destroy ();
				cam->m_map_visualisation.init (w, h);
				cam->m_map_visualisation.clear (CLR (0,0,0,0));
				cam->m_map_visualisation.draw (&cpy4, v2i(0,0));
				auto cpy1 = cam->m_map;
				cam->m_map.destroy ();
				cam->m_map.init (w, h);
				cam->m_map.clear (0);
				FOR_ARRAY_2D (v, cpy1) {
					if (cam->m_map << v) {
						cam->m_map[v] = cpy1[v];
					}
				}
				auto cpy2 = sol->m_map_visualisation;
				sol->m_map_visualisation.destroy ();
				sol->m_map_visualisation.init (w, h);
				sol->m_map_visualisation.clear (CLR (0,0,0,0));
				sol->m_map_visualisation.draw (&cpy2, v2i(0,0));
				auto cpy3 = sol->m_map;
				sol->m_map.destroy ();
				sol->m_map.init (w, h);
				sol->m_map.clear (0);
				FOR_ARRAY_2D (v, cpy3) {
					if (sol->m_map << v) {
						sol->m_map[v] = cpy3[v];
					}
				}
			}
		}
	}
}

void static_im::clean () {
	//PUT YOUR CODE HERE
}

void static_im::load () {
	m_bg.init (D_W, D_H);
	m_bg.clear (CLR (140,140,140,255));

	read_string (); // "}"
}