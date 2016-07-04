#include "static_im.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"
#include "camera.h"
#include "input.h"
#include "solid.h"
#include "glob_vars.h"
#include "hot_vars.h"
#include "png.h"

void static_im::render (State state) {
	camera *cam = D_PLATFORM (camera);
	if (state == State_PLATFORM_GAME) {
		v2i pos (0,0);
		int &x = pos.x;
		int cx = -cam->m_pos.x / 2;
		cx %= D_W;
		while (cx < 0) {
			cx += D_W;
		}
		x = cx;

		ren.sprite.rgba_ar = &m_real_bg;
		ren.sprite.pos = pos;
		ren.m_render_list.insert (ren.m_render_list.begin(), ren.sprite);

		x = cx - D_W;
		ren.sprite.pos = pos;
		ren.m_render_list.insert (ren.m_render_list.begin(), ren.sprite);
		D_ADD_SPRITE (m_fg, -cam->m_pos);
	} else {
		D_ADD_SPRITE (m_fg, -cam->m_pos);
		ren.sprite.rgba_ar = &m_bg;
		ren.sprite.pos = -cam->m_pos;
		ren.m_render_list.insert (ren.m_render_list.begin(), ren.sprite);
	}
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
				GV.lists[S_[0]].I["w"] = w;
				GV.lists[S_[0]].I["h"] = h;
				auto cpy = m_bg;
				m_bg.destroy ();
				m_bg.init (w, h);
				m_bg.clear (CLR (140,140,140,255));
				m_bg.draw (&cpy, v2i(0,0));

				cpy = m_fg;
				m_fg.destroy ();
				m_fg.init (w, h);
				m_fg.clear (CLR (0,0,0,0));
				m_fg.draw (&cpy, v2i(0,0));

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
			if (in.kb['S'].just_pressed) {
				if (!save_png (m_fg, prefix_path + "tex/" + S_[0] + "/fg.png")) {
					exit (0);
				}
			}
		}
	}
}

void static_im::clean () {
	//PUT YOUR CODE HERE
}

void static_im::load () {
	m_bg.init (GV.lists[S_[0]].I["w"], GV.lists[S_[0]].I["h"]);
	m_bg.clear (CLR (140,140,140,255));

	if (!load_png (m_real_bg, prefix_path + "tex/" + S_[0] + "/bg.png")) {
		exit (0);
	}
	if (!load_png (m_fg, prefix_path + "tex/" + S_[0] + "/fg.png")) {
		exit (0);
	} else {
		m_fg.alpha_matters = true;
	}

	read_string (); // "}"
}