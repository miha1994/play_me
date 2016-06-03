#pragma once
#include "object.h"
#include "Renderer.h"
#include "space.h"
#include <vector>
#include <map>

using namespace std;

// Ядро. Главный класс. Объект этого класса существует в единственном экземпляре. Он - сердце программы.
class Core {
public:
	void init ();	// вызывается один раз при инициализации
	void update (float dt);		// обновление всего и вся. dt - прошедшее время с прошлого кадра
	void render ();				// отрисовать кадр

	Renderer m_renderer;		// поле класса, занимающееся рисованием
	State m_current_state;		// текущее состояние процесса
	map <string, Space *> m_spaces;	// все спэйсы
	vector <Space *> m_current_spaces;	// активные сейчас спэйсы. Только для них вызываются функции обновления и отрисовки

	Core () {}	// конструктор
	~Core () {	// деструктор
		forstl (p, m_spaces) {
			delete p->second;
		}
	}
};

extern Core core;