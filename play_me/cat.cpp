#include "cat.h"
#include "read_write.h"
#include "input.h"

#include "hot_vars.h"

void cat::render (State state) {
	m_frames.render_frame (!m_to_right);
}

void cat::update (State state, float dt) {
	m_frames.update (dt);
	if (m_frames.m_current_instruction == 1) {
		m_time_to_next_movement -= dt;
		if (m_time_to_next_movement < 0) {
			m_frames.go_to_instruction (2 + rand () %2);
			m_time_to_next_movement = 0.2 + rand1;
		}
	}
	if (in.kb.dirs[D_RIGHT].just_pressed || in.kb.dirs[D_LEFT].just_pressed) {
		m_to_right = in.kb.dirs[D_RIGHT].pressed_now;
		m_frames.go_to_instruction (4);
	}
	if (in.kb.dirs[D_RIGHT].pressed_now || in.kb.dirs[D_LEFT].pressed_now) {
		m_pos.x += (in.kb.dirs[D_RIGHT].pressed_now ? 1 : -1) * dt * 150;
	}
	if (in.kb.dirs[D_RIGHT].just_released || in.kb.dirs[D_LEFT].just_released) {
		if (!in.kb.dirs[D_RIGHT].pressed_now && !in.kb.dirs[D_LEFT].pressed_now) {
			m_frames.go_to_instruction (1);
		} else {
			m_to_right = in.kb.dirs[D_RIGHT].pressed_now;
		}
	}

	m_frames.m_pos = m_pos;
}

void cat::clean () {
	//PUT YOUR CODE HERE
}

void cat::load () {
	m_time_to_next_movement = 2;
	m_frames.init ("cat");
	m_frames.m_fps = 10;
	m_pos = m_frames.m_pos = v2i (150, 100);
	m_frames.m_instructions.push_back (frames::command (0, 1, 0, 1, 30));
	m_frames.m_instructions.push_back (frames::command (1, 2, 0, 1, 8));
	m_frames.m_instructions.push_back (frames::command (2, 8, 0, 1, 8));
	m_frames.m_instructions.push_back (frames::command (8, 10, 10, 5, I_[1]));
	m_frames.m_instructions.push_back (frames::command (10, 16, 10, 5, I_[1]));
	m_frames.m_current_instruction = 1;
	read_string ();
}