#pragma once
#include "object.h"
#include "state.h"
#include "glob.h"
#include "array_2d.h"

struct static_im : Object {
	rgba_array m_bg;

	void render (State state);
	void update (State state, float dt);
	void clean ();
	void load ();
};