#include "core.h"
#include "fadma.h"
#include "input.h"
#include "glob_vars.h"

Core core;

void Core::init () {
	m_current_state = State_PLATFORM_GAME;
	m_spaces["platform"] = new Space ("platform");	// здесь парсится файл platform_space.txt и создается основной спэйс
	m_current_spaces.push_back (m_spaces["platform"]);	// он назначается активным

	m_renderer.init ();
}

void Core::update (float dt) {
	static input inp;
	inp.upd ();
	rand ();
	bool goto_ = false;
	if (m_current_state == State_PLATFORM_REDACTOR) {
		if (!inp.kb.ctrl.pressed_now) {
			if (inp.kb ['B'].just_pressed) {
				m_active_type = Solid;
			} else if (inp.kb ['H'].just_pressed) {
				m_active_type = Cat;
			} else if (inp.kb ['C'].just_pressed) {
				m_active_type = Camera;
			} else if (inp.kb ['M'].just_pressed) {
				m_active_type = Static_im;
			} else if (inp.kb ['I'].just_pressed) {
				m_active_type = Item;
			} else if (inp.kb ['D'].just_pressed) {
				m_active_type = Dark_matter;
			} else if (inp.kb ['S'].just_pressed) {
				m_active_type = Ship;
			} else if (inp.kb ['Q'].just_pressed) {
				m_current_state = State_PLATFORM_GAME;
				goto_ = true;
			}
		} else {
			if (inp.kb ['S'].just_pressed) {
				GV.save ();
			}
		}
	}
	if (!goto_ && m_current_state == State_PLATFORM_GAME) {
		if (!inp.kb.ctrl.pressed_now) {
			if (inp.kb ['Q'].just_pressed) {
				m_current_state = State_PLATFORM_REDACTOR;
			}
		}
	}
	dt = Min (dt, 0.03);
	static float TIME = 0;
	TIME += dt;

	in.upd ();
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_update) {
			p->update (m_current_state, 0.0f);
		}
	}
	while (TIME > 0) {
		in.upd ();
		forstl_p (q, m_current_spaces) {
			forstl_p (p, q->m_objects_to_update) {
				p->update (m_current_state, 0.01);
			}
		}
		TIME -= 0.01;
	}
}

void Core::render () {
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_render) {
			p->render (m_current_state);
		}
	}
}