#include "item.h"
#include "png.h"
#include "Renderer.h"
#include "core.h"
#include "input.h"
#include "camera.h"
#include "cat.h"
#include "hot_vars.h"
#include "read_write.h"

static const float lif_tm = 0.3;

void item::reload () {
	forstl (p, m_all_items) {
		p->active = true;
	}
}

void item::render (State state) {
	camera *cam = D_PLATFORM (camera);
	if (state == State_PLATFORM_GAME) {
		forstl (p, m_all_items) {
			if (p->active) {
				D_ADD_SPRITE (m_light[p->type_num], p->pos + v2i (0, 3 * sin (2*m_time) - 0.5) - v2i (21,21) - cam->m_pos);
				D_ADD_SPRITE (m_textures[p->type_num], p->pos + v2i (0, 3 * sin (2*m_time) - 0.5) - v2i (6,6) - cam->m_pos);
			}
		}
		forstl (p, m_sp_effect_square) {
			D_ADD_SP_EFFECT_SQUARE (p->pos - cam->m_pos, p->time * 150 / lif_tm, (lif_tm - p->time)/lif_tm * 10);
		}
	} else if (state == State_PLATFORM_REDACTOR) {
		forstl (p, m_all_items) {
			D_ADD_SPRITE (m_textures[p->type_num], p->pos - v2i (6,6) - cam->m_pos);
		}
		if (core.m_active_type == Type::Item) {
			D_ADD_SPRITE (m_textures[m_redact.tmp.type_num], m_redact.tmp.pos - v2i (6,6));
		}
	}
}

void item::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	cat *ct = D_PLATFORM (cat);
	m_time += dt;
	if (state == State_PLATFORM_GAME) {
		FOR (i, 5) {
			FOR_ARRAY_2D (v, m_light[i]) {
				m_light[i][v].a = m_light_smpl[v].a * abs (0.63 * sin (1.8 * m_time));
			}
		}
		forstl (p, m_all_items) {
			if (p->active) {
				if (length (p->pos - (ct->m_pos + v2f (12.5, 8))) < 20) {
					sp_effect_square sq;
					sq.pos = p->pos;
					sq.time = 0;
					m_sp_effect_square.push_back (sq);
					p->active = false;
				}
			}
		}
		forstl_no_inc (p, m_sp_effect_square) {
			static const float lif_tm = 0.3;
			p->time += dt;
			if (p->time > lif_tm) {
				p = m_sp_effect_square.erase (p);
			} else {
				++p;
			}
		}
	} else if (state == State_PLATFORM_REDACTOR) {
		if (core.m_active_type == Type::Item) {
			m_redact.tmp.pos = in.mouse.pos;
			if (in.mouse.right.just_pressed) {
				m_redact.tmp.type_num = (m_redact.tmp.type_num + 1) % 5;
			}
			if (in.mouse.left.just_pressed) {
				m_redact.tmp.pos += cam->m_pos;
				m_all_items.push_back (m_redact.tmp);
				m_redact.tmp.pos -= cam->m_pos;
			}
		}
		if (in.kb.ctrl.pressed_now && in.kb['S'].just_pressed) {
			FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_items.inf")).c_str (), "w");
			if (f) {
				fprintf (f, "%d\n", m_all_items.size ());
				forstl (p, m_all_items) {
					fprintf (f, "%d %d %d\n", p->pos.x, p->pos.y, p->type_num);
				}
				fclose (f);
			}
		}
	}
}

void item::clean () {
	//PUT YOUR CODE HERE
}

void item::load () {
	m_time = 0;

	m_redact.tmp.type_num = 0;
	m_redact.tmp.active = true;

	rgba_array all_5;
	if (!load_png (all_5, Texture_name ("items"))) {
		exit (0);
	}
	all_5.alpha_matters = true;

	FILE *f = fopen ((string (prefix_path + "lvl/" + S_[0] + "_items.inf")).c_str (), "r");
	if (f) {
		one_item el;
		el.active = true;
		int count;
		fscanf (f, "%d", &count);
		For (count) {
			fscanf (f, "%d%d%d", &el.pos.x, &el.pos.y, &el.type_num);
			m_all_items.push_back (el);
		}
		fclose (f);
	}

	FOR (i, 5) {
		m_textures[i].init (15, 15);
		m_textures[i].alpha_matters = true;
		m_textures[i].clear (CLR(0,0,0,0));
		m_textures[i].draw (&all_5, v2i(0,0), 1, v2i(15,15), v2i (i * 15, 0));

		load_png (m_light[i], Texture_name ("item_light"));
		m_light[i].alpha_matters = true;
		FOR_ARRAY_2D (v, m_light[i]) {
			m_light[i][v] = m_textures[i][v2i (7,7)];
		}
	}
	load_png (m_light_smpl, Texture_name ("item_light"));
	m_light_smpl.alpha_matters = true;

	read_string ();
}