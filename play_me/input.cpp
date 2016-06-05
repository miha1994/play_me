#include "input.h"
#include "glob.h"
#include "Renderer.h"

input in;
v2i g_shift_corrector (-10, -33);

sf::Mouse::Button b_ar[3] = {sf::Mouse::Left, sf::Mouse::Right, sf::Mouse::Middle};
#define kb_	sf::Keyboard

void button_state::_upd (bool new_v) {
	if (new_v != pressed_now) {
		if (new_v) {
			just_pressed = true;
			just_released = false;
		} else {
			just_released = true;
			just_pressed = false;
		}
	} else {
		just_pressed = false;
		just_released = false;
	}
	pressed_now = new_v;
}

void input::upd () {
	auto a = sf::Mouse::getPosition ();
	mouse.pos.x = a.x;
	mouse.pos.y = a.y;
	if(!g_fullscreen) {
		mouse.pos.x -= D_X_INIT;
		mouse.pos.y -= D_Y_INIT;
		mouse.pos += g_shift_corrector;
	}
	mouse.pos /= D_SCALE;
	FOR (i, 3) {
		mouse.mbutton[i]._upd (sf::Mouse::isButtonPressed (b_ar[i]));
	}
    FOR (i, 26) {
		kb._abc[i]._upd (kb_::isKeyPressed (kb_::Key (i)));
	}
	FOR (i, 10) {
		kb._digits[i]._upd (kb_::isKeyPressed (kb_::Key (26 + i)));
	}
	kb.ctrl._upd (kb_::isKeyPressed (kb_::LControl) || kb_::isKeyPressed (kb_::RControl));
	kb.alt._upd (kb_::isKeyPressed (kb_::LAlt) || kb_::isKeyPressed (kb_::RAlt));
	kb.shift._upd (kb_::isKeyPressed (kb_::LShift) || kb_::isKeyPressed (kb_::RShift));
	kb.escape._upd (kb_::isKeyPressed (kb_::Escape));
	kb.space._upd (kb_::isKeyPressed (kb_::Space));
	kb.delete_._upd (kb_::isKeyPressed (kb_::Delete));
	kb.dirs[D_RIGHT]._upd (kb_::isKeyPressed (kb_::Right));
	kb.dirs[D_LEFT]._upd (kb_::isKeyPressed (kb_::Left));
	kb.dirs[D_UP]._upd (kb_::isKeyPressed (kb_::Up));
	kb.dirs[D_DOWN]._upd (kb_::isKeyPressed (kb_::Down));
}