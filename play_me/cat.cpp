#include "cat.h"
#include "read_write.h"
#include "input.h"
#include "core.h"
#include "solid.h"
#include "camera.h"

#define D_VELOCITY_X_MAX	230
#define D_VELOCITY_Y_MAX	400
// проверка на граничение по скорости, нормировка по Х и по Y
#define D_VEL_X		if (m_velocity.x < -D_VELOCITY_X_MAX) { \
					m_velocity.x = -D_VELOCITY_X_MAX; \
				} else if (m_velocity.x > D_VELOCITY_X_MAX) { \
					m_velocity.x = D_VELOCITY_X_MAX; \
				}
#define D_VEL_Y		if (m_velocity.y < -D_VELOCITY_Y_MAX) { \
					m_velocity.y = -D_VELOCITY_Y_MAX; \
				} else if (m_velocity.y > D_VELOCITY_Y_MAX) { \
					m_velocity.y = D_VELOCITY_Y_MAX; \
				}

void cat::render (State state) {
	camera *cam = D_PLATFORM (camera);
	m_frames.m_pos -= cam->m_pos;
	m_frames.render_frame (!m_to_right);
}

void cat::update (State state, float dt) {
	camera *cam = D_PLATFORM (camera);
	int y_offset = 0;
	int x_offset = 0;
	if (state == State_PLATFORM_GAME) {
		switch (m_state) {
		case Cat_on_floor:  // когда мы на полу
			y_offset = 1;
			m_frames.update (dt);  //обновляем фрейм
			if (m_frames.m_current_instruction == 1) { // цикл стояние на месте: если сейчас стоим, то 
				m_time_to_next_movement -= dt;  // уменьшаем время
				if (m_time_to_next_movement < 0) {  // смена "стоячей" команды на движение
					m_frames.go_to_instruction (2 + rand () %2);
					m_time_to_next_movement = 0.2 + rand1;  
				}
			}
			if (in.kb.dirs[D_RIGHT].just_pressed || in.kb.dirs[D_LEFT].just_pressed) {
				m_frames.go_to_instruction (4);
			}
			if (in.kb.dirs[D_RIGHT].pressed_now || in.kb.dirs[D_LEFT].pressed_now) {
				m_velocity.y = 0;
				m_velocity.x += (in.kb.dirs[D_RIGHT].pressed_now ? 1 : -1) * 1200 * dt;
				D_VEL_X;
				x_offset = m_velocity.x * dt + (m_velocity.x > 0 ? 1 : -1);
				m_pos += m_velocity * dt;
			} else {
				m_velocity = v2i (0,0);
			}
			if (in.kb.dirs[D_RIGHT].just_released || in.kb.dirs[D_LEFT].just_released) {
				if (!in.kb.dirs[D_RIGHT].pressed_now && !in.kb.dirs[D_LEFT].pressed_now) {
					m_frames.go_to_instruction (1);
				}
			}
			if (in.kb.space.just_pressed) { 
				m_velocity.y = -180;
				m_time_velocity = 0.2;
				m_state = Cat_in_jump;
			}
			break;
		case Cat_in_jump:
			if (in.kb.left.pressed_now || in.kb.right.pressed_now) {
				m_velocity += v2f ((in.kb.left.pressed_now ? -1 : 1) * 900 * dt, 0);
			}
			m_velocity += v2f (0, 800 * dt);
			if ((m_time_velocity -= dt) > 0) {
				if (in.kb.space.pressed_now) {
					m_velocity.y = -180;
				} else {
					m_time_velocity = -1;
				}
			}
			D_VEL_X;
			D_VEL_Y;
			y_offset = m_velocity.y * dt + 1;
			x_offset = m_velocity.x * dt + (m_velocity.x > 0 ? 1 : -1);
			m_pos += m_velocity * dt;
			if (m_velocity.y < -40) {  //отрисовка в неподвижном фрейме кота в полете, состояния отражают команды:
				m_frames.m_current_frame = 8;  // кот летит вверх
			} else if (m_velocity.y < 40) {
				m_frames.m_current_frame = 10;  // кот - овощ
			} else {
				m_frames.m_current_frame = 16;  // кот падает
			}
			break;
		}
		if (m_velocity.x) {  // если кот движется по горизонтали, то апдейт кота
			m_to_right = m_velocity.x > 0;
		}

		v2i old = m_frames.m_pos;  // запоминаем старую позицию
		m_frames.m_pos = m_pos;
		if (D_CONTAINES (D_OBJ ("platform"), "solid")) {
			solid *sld = (solid *) D_OBJ ("platform")["solid"];
			bool contin = false;
			bool jump = true;
			if (m_velocity.y >= 0) {
				v2i strt (5, 15 - y_offset);
				
				FOR_2D (vi, 15, y_offset+1) {
					v2i v (m_frames.m_pos + strt + vi);
					if (sld->m_map << v && sld->m_map[v] == 1) {
						m_frames.m_pos.y--;
						m_pos.y -= 1;
						
						if (m_state != Cat_on_floor) {
							m_state = Cat_on_floor;
							if (in.kb.left.pressed_now || in.kb.right.pressed_now) {
								m_frames.go_to_instruction (5);
								m_frames.m_current_frame++;
							} else {
								m_frames.go_to_instruction (1);
							}
						}
						contin = true;
						
						jump = false;
						break;
					}
				}
			} 
			//m_frames.m_pos = m_pos;
			if (m_velocity.x) {
				bool cont = true;
				bool vel_to_z = false;
				while (cont && x_offset) {
					cont = false;
					v2i strt (m_velocity.x > 0 ? 20 - x_offset : 2, 3);
					FOR_2D (vi, Abs(x_offset) + 2, 11) {
						v2i v (m_frames.m_pos + strt + vi);
						if (sld->m_map << v && sld->m_map[v] == 1) {
							cont = true;
						}
					}
					if (cont) {
						m_pos.x = (m_frames.m_pos.x -= x_offset / Abs(x_offset));
						vel_to_z = true;
					}
					if (x_offset) {
						x_offset -= x_offset / Abs(x_offset);
					}
				}
				if (vel_to_z) {
					m_velocity.x = 0;
				}
			}
			v2i strt (5, 15 - y_offset);
			while (contin) {
				contin = false;
				FOR_2D (vi, 15, y_offset+1) {
					v2i v (m_frames.m_pos + strt + vi);
					if (sld->m_map << v && sld->m_map[v] == 1) {
						contin = true;
						m_frames.m_pos.y--;
						m_pos.y -= 1;
						
						break;
					}
				}
			}
			if (jump) {
				m_state = Cat_in_jump;
			} else {
				m_pos.y += 1;
				m_frames.m_pos.y ++;
			}
			if (m_velocity.y < 0) {
				v2i strt (5, y_offset+2);
				FOR_2D (vi, 15, -y_offset+2) {
					v2i v (m_frames.m_pos + strt + vi);
					if (sld->m_map << v && sld->m_map[v] == 1) {
						m_velocity.y = 0;
						m_time_velocity = -1;
						break;
					}
				}
			}
		}
	} else if (state == State_PLATFORM_REDACTOR) {  // если мы не в игре, в режиме редактирования
		if (core.m_active_type == Cat) {			// если юзер работает с котом, то кот бегает за его мышкой
			m_pos = in.mouse.pos + cam->m_pos;
			m_frames.m_pos = m_pos;
		} else {									// ... это работает!
			m_frames.m_pos = m_pos;
		}
	}
}

void cat::clean () {
	//PUT YOUR CODE HERE
}

void cat::load () { // загрузка/создание кота
	m_state = Cat_in_jump;
	m_velocity = v2i (0,0);

	m_time_to_next_movement = 1;  // 1 секунда до след кадра
	m_frames.init ("cat");
	m_frames.m_fps = 10;
	m_pos = m_frames.m_pos = v2i (150, 100);
	m_frames.m_instructions.push_back (frames::command (0, 1, 0, 1, 30));  //цикличное стояние на месте
	m_frames.m_instructions.push_back (frames::command (1, 2, 0, 1, 8));
	m_frames.m_instructions.push_back (frames::command (2, 8, 0, 1, 8));
	m_frames.m_instructions.push_back (frames::command (8, 10, 10, 5, 15));
	m_frames.m_instructions.push_back (frames::command (10, 16, 10, 5, 15)); // цикличный бег
	m_frames.m_current_instruction = 1;
	read_string ();
}