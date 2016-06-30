#include "core.h"
#include "fadma.h"
#include "input.h"
#include "glob_vars.h"

Core core;

void Core::init () {
	m_current_state = State_PLATFORM_REDACTOR;
	m_spaces["platform"] = new Space ("platform");	// здесь парсится файл platform_space.txt и создается основной спэйс
	m_current_spaces.push_back (m_spaces["platform"]);	// он назначается активным

	m_renderer.init ();
}

void Core::update (float dt) {
	in.upd ();
	rand ();
	bool goto_ = false;
	if (m_current_state == State_PLATFORM_REDACTOR) {
		if (!in.kb.ctrl.pressed_now) {
			if (in.kb ['B'].just_pressed) {
				m_active_type = Solid;
			} else if (in.kb ['H'].just_pressed) {
				m_active_type = Cat;
			} else if (in.kb ['C'].just_pressed) {
				m_active_type = Camera;
			} else if (in.kb ['M'].just_pressed) {
				m_active_type = Static_im;
			} else if (in.kb ['I'].just_pressed) {
				m_active_type = Item;
			} else if (in.kb ['Q'].just_pressed) {
				m_current_state = State_PLATFORM_GAME;
				goto_ = true;
			}
		} else {
			if (in.kb ['S'].just_pressed) {
				GV.save ();
			}
		}
	}
	if (!goto_ && m_current_state == State_PLATFORM_GAME) {
		if (!in.kb.ctrl.pressed_now) {
			if (in.kb ['Q'].just_pressed) {
				m_current_state = State_PLATFORM_REDACTOR;
			}
		}
	}
	dt = Min (dt, 0.03);

	while (dt > 0) {
		forstl_p (q, m_current_spaces) {
			forstl_p (p, q->m_objects_to_update) {
				p->update (m_current_state, Min (dt, 0.01));
			}
		}
		dt -= 0.01;
	}
}

void Core::render () {
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_render) {
			p->render (m_current_state);
		}
	}
}