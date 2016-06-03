#include "static_im.h"
#include "read_write.h"
#include "Renderer.h"
#include "core.h"

void static_im::render (State state) {
	D_ADD_SPRITE (m_bg, v2i (0,0));
}

void static_im::update (State state, float dt) {
	//PUT YOUR CODE HERE
}

void static_im::clean () {
	//PUT YOUR CODE HERE
}

void static_im::load () {
	m_bg.init (D_W, D_H);
	m_bg.clear (CLR (140,140,140,255));

	read_string (); // "}"
}