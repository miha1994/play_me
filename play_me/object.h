#pragma once
#include "state.h"
#include "type.h"
#include "fadma.h"

// объект. Сферический. в вакууме...
struct Object {
	Type m_type;

	virtual void render (State state) = 0;
	virtual void update (State state, float dt) = 0;
	virtual void clean () = 0;
	virtual void load () = 0;

	Object () {}
	virtual ~Object () {}
};

Object *new_Object (string id);