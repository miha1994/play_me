#include "frame_buffer.h"
#include "lodepng.h"
#include "core.h"
#include "png.h"

#define D_CURRENT_INSTR			m_instructions[m_current_instruction]

void frames::init (string file_name) {
	m_pos = v2i(0,0);
	m_fps = 10;
	m_time_to_switch_frame = 1.0 / m_fps;
	m_current_instruction = 0;

	m_current_frame = 0;
	rgba_array rgb (true);
	load_png (rgb, Texture_name (file_name));
	unsigned w = rgb.get_W (), h = rgb.get_H ();

	FILE *read = fopen ((prefix_path + "tex/" + file_name + "_info.an").c_str (), "r");
	int rw, rh;
	fscanf (read, "%d%d", &rw, &rh);

	FOR (i, w/rw) {
		m_frames.push_back (rgba_array (true));
		m_frames.back ().init (rw, rh);
		m_frames.back ().clear (CLR (0,0,0,0));
		m_frames.back ().draw (&rgb, v2i (0, 0), 1, v2i(rw,rh), v2i (rw*i,0));
	}
	fclose (read);

	m_flip_frames.assign (m_frames.size (), rgb);
	FOR (i, m_frames.size ()) {
		m_flip_frames[i] = m_frames[i];
	}
	forstl (p, m_flip_frames) {
		rgba_array &a = *p;
		
		FOR_2D (v, a.m_W / 2, a.m_H) {
			swap (a[v], a[v2i (a.m_W - 1 - v.x, v.y)]);
		}
		
	}

	m_instructions.push_back (command (0, m_frames.size (), 0, 0, 10));
}

void frames::update (float dt) {
	m_time_to_switch_frame -= dt;
	if (m_time_to_switch_frame < 0) {
		m_time_to_switch_frame += 1.0 / m_fps;
		++m_current_frame;
		if (m_current_frame == D_CURRENT_INSTR.stop_point) {
			m_current_frame = D_CURRENT_INSTR.next_point_after_stop;
			m_current_instruction = D_CURRENT_INSTR.next_command;
			m_fps = D_CURRENT_INSTR.fps;
		}
	}
}

void frames::render_frame (bool flip) {
	if (flip) {
		D_ADD_SPRITE (m_flip_frames[m_current_frame], m_pos);
	} else {
		D_ADD_SPRITE (m_frames[m_current_frame], m_pos);
	}
}

void frames::go_to_instruction (int num) {
	m_current_instruction = num;
	m_current_frame = D_CURRENT_INSTR.start;
	m_fps = D_CURRENT_INSTR.fps;
	m_time_to_switch_frame = 1.0 / m_fps;
}