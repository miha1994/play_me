#include "dark_matter.h"
#include "Renderer.h"
#include "core.h"
#include "camera.h"
#include "input.h"
#include "read_write.h"
#include "png.h"
#include "hot_vars.h"
#include "cat.h"

void dark_matter::render (State state) {
	camera *cam = D_PLATFORM (camera);
	m_ps1.render (cam->m_pos);
}

void dark_matter::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	if (state == State_PLATFORM_GAME) {
		cat *ct = D_PLATFORM (cat);
		if (ct->m_state != Cat_dead) {
			IntRect rect;
			v2i sz (12, 8);
			forstl (p, m_elements) {
				rect.pos = p->pos - sz;
				rect.size = sz * 2 + v2i (0, p->len);
				if (rect << (ct->m_pos + v2i (12, 8))) {
					ct->death ();
					break;
				}
			}
		}
	} else if (state == State_PLATFORM_REDACTOR) {
		if (core.m_active_type == Dark_matter) {
			if (in.mouse.left.just_pressed) {
				m_redactor.strt_pos = in.mouse.pos + cam->m_pos;
				elem nw;
				nw.pos = m_redactor.strt_pos;
				nw.time_to_next_particle = 0;
				m_elements.push_back (nw);
			}
			if (in.mouse.left.pressed_now) {
				m_elements.back ().len = D_ABS (m_redactor.strt_pos.y - (in.mouse.pos + cam->m_pos).y);
			}
			if (in.mouse.right.pressed_now) {
				forstl_no_inc (p, m_elements) {
					if (length (p->pos - (in.mouse.pos + cam->m_pos)) < 10) {
						p = m_elements.erase (p);
						continue;
					}
					++p;
				}
			}
		}
		
		bool b = in.kb.ctrl.pressed_now && in.kb['S'].just_pressed;
		if (b) {
			FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_dark_matter.inf")).c_str (), "w");
			if (f) {
				fprintf (f, "%d\n", m_elements.size ());
				forstl (p, m_elements) {
					fprintf (f, "%d %d %d\n", p->pos.x, p->pos.y, p->len);
				}
				fclose (f);
			}
		}
		
	}
	forstl (p, m_elements) {
		if ((p->time_to_next_particle -= dt) < 0) {
			p->time_to_next_particle += rand1 * hvD_[0];
			int n = rand () % 2;
			m_ps1.add_elem (&m_textures[n], p->pos + v2i ((n ? -4 : -3) - 3 + rand () % 6, (n ? -4 : -3) - 4), v2f (0, 1), 4, p->len, 4, (p->len+8) / hvD_[1]);
		}
	}
	m_ps1.upd (dt);
}

void dark_matter::clean () {
	//PUT YOUR CODE HERE
}

void dark_matter::load () {
	load_png (m_textures[0], prefix_path + "tex/dark_matter.png", IntRect (v2i (0,0), v2i (6,6)));
	load_png (m_textures[1], prefix_path + "tex/dark_matter.png", IntRect (v2i (6,0), v2i (8,8)));

	FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_dark_matter.inf")).c_str (), "r");
	if (f) {
		elem el;
		el.time_to_next_particle = 0;
		int count;
		fscanf (f, "%d", &count);
		For (count) {
			fscanf (f, "%d%d%d", &el.pos.x, &el.pos.y, &el.len);
			m_elements.push_back (el);
		}
		fclose (f);
	}

	float dt = 0.01;
	For (2000) {
		forstl (p, m_elements) {
			if ((p->time_to_next_particle -= dt) < 0) {
				p->time_to_next_particle += rand1 * hvD_[0];
				int n = rand () % 2;
				m_ps1.add_elem (&m_textures[n], p->pos + v2i ((n ? -4 : -3) - 3 + rand () % 6, (n ? -4 : -3) - 4), v2f (0, 1), 4, p->len, 4, (p->len+8) / hvD_[1]);
			}
		}
		m_ps1.upd (dt);
	}

	read_string (); // "}"
}