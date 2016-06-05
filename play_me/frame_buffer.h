#pragma once
#include "array_2d.h"
#include "glob.h"
#include <vector>
#include "Renderer.h"

// анимация: кадры, 
struct frames {
	v2i m_pos; // позиция кота в кадре
	float m_time_to_switch_frame;  //время, количество секунд кот осталось до смены следующего кадра
	int m_fps;  // фпс
	int m_current_frame; // текущий кадр
	vector <rgba_array> m_frames;  //все кадры (право)
	vector <rgba_array> m_flip_frames;  //инвертированные кадры (лево)
	struct command {  // команда(инструкция) - шаблон анимации(есть команды: моргунуть, вильнуть хвостом)
		int start;
		int stop_point;
		int next_point_after_stop;
		int next_command;
		int fps;
		command (int _start, int _stop_point, int _next_point_after_stop, int _next_command, int _fps) {
			start = _start;
			stop_point = _stop_point;
			next_point_after_stop = _next_point_after_stop;
			next_command = _next_command;
			fps = _fps;
		}
	};
	vector <command> m_instructions;
	int m_current_instruction;

	void go_to_instruction (int num_of_command);  // переход по инструкции/по команде
	void init (string file);
	void update (float dt);
	void render_frame (bool flip);
};