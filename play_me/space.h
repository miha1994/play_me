#pragma once
#include "object.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

// спэйс так и будем называть спэйсом
struct Space {
	vector <Object *> m_objects_to_update;	// последовательность объектов, которые надо обновить
	vector <Object *> m_objects_to_render;	// последовательность объектов, которые надо отрисовать
	map <string, Object *> m_objects;	// собственно объекты
	Space (string name);	// конструктор
	~Space () {				// деструктор
		forstl (p, m_objects) {
			delete p->second;
		}
	}
};