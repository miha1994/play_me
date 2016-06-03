#include "core.h"
#include "fadma.h"
#include "input.h"

Core core;

void Core::init () {
	m_current_state = State_0;
	m_spaces["platform"] = new Space ("platform");	// здесь парсится файл platform_space.txt и создается основной спэйс
	m_current_spaces.push_back (m_spaces["platform"]);	// он назначается активным

	m_renderer.init ();
}

void Core::update (float dt) {
	in.upd ();
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_update) {
			p->update (m_current_state, dt);
		}
	}
}

void Core::render () {
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_render) {
			p->render (m_current_state);
		}
	}
}