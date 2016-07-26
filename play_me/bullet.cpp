#include "bullet.h"

#include "bullet.h"
#include "png.h"
#include "Renderer.h"
#include "core.h"
#include "input.h"
#include "camera.h"
#include "cat.h"
#include "hot_vars.h"
#include "read_write.h"
#include "solid.h"

void bullet::reload () {
	forstl_no_inc (p, m_all_bullets) {
		if (m_type_is_restorable [p->type_num]) {
			p->active = true;
		} else {
			p = m_all_bullets.erase (p);
			continue;
		}
		++p;
	}
}

void bullet::render (State state) {
	camera *cam = D_PLATFORM (camera);
	if (state == State_PLATFORM_GAME) {
		forstl (p, m_all_bullets) {
			switch (p->type_num) {
			case 0:
				D_ADD_SPRITE (m_textures[p->type_num], p->pos - v2i (2,2) - cam->m_pos);
				break;
			}
		}
		m_blow_anim.render (cam->m_pos);
	} else if (state == State_PLATFORM_REDACTOR) {
	
	}
}

void bullet::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	cat *ct = D_PLATFORM (cat);
	solid *sld = (solid *) D_OBJ ("platform")["solid"];

	m_blow_anim.update (dt);
	if (state == State_PLATFORM_GAME) {
		bool contin;
		forstl_no_inc (p, m_all_bullets) {
			contin = false;

			v2f strt = p->pos;
			switch (p->type_num) {
			case 0:
				p->tm_inc += dt;
				if (p->tm_inc > 10) {
					p = m_all_bullets.erase (p);
					contin = true;
					break;
				}
				p->pos.y += dt * 200.0;
				break;
			}
			if (contin) {
				continue;
			}
			v2f fnsh = p->pos;
			v2f dif = fnsh - strt;
			v2f _abs = dif.abs ();
			int N = 2 + Max (_abs.x, _abs.y);
			v2i v;
			contin = false;
			FOR (z, N) {
				v = strt + dif * float (z) / (N-1);
				if (sld->m_map << v && sld->m_map[v] == 1) {
					m_blow_anim.add (&m_blows[p->type_num], p->pos - v2i (8,8), 20);
					p = m_all_bullets.erase (p);
					contin = true;
					break;
				}
			}
			if (contin) {
				continue;
			}
			++p;
		}
	} else if (state == State_PLATFORM_REDACTOR) {
	}
}

void bullet::clean () {
	//PUT YOUR CODE HERE
}

void bullet::load () {
	FOR (i, g_num_of_bullet_types) {
		string file_name = read_string ();
		if (!load_png (m_textures[i], Texture_name (file_name))) {
			exit (0);
		}
		m_textures[i].alpha_matters = true;

		m_blows[i].init (read_string ());
	}
	m_type_is_restorable[0] = false;

	read_string ();
}