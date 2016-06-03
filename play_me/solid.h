#pragma once
#include "object.h"
#include "state.h"

struct solid : Object {
	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};