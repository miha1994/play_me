#include "object.h"
#include "fadma.h"
#include "cat.h"
#include "solid.h"
#include "static_im.h"
#include "camera.h"
#include "item.h"
#include "bullet.h"
#include "dark_matter.h"
#include "ship.h"

// ������� ��������
Object *new_Object (string id) {
	SWITCH (id)
	CASE ("cat") {
		cat *o = new cat;
		o->m_type = Type::Cat;
		return o;
	}
	CASE ("solid") {
		solid *o = new solid;
		o->m_type = Type::Solid;
		return o;
	}
	CASE ("static_im") {
		static_im *o = new static_im;
		o->m_type = Type::Static_im;
		return o;
	}
	CASE ("camera") {
		camera *o = new camera;
		o->m_type = Type::Camera;
		return o;
	}
	CASE ("item") {
		item *o = new item;
		o->m_type = Type::Item;
		return o;
	}
	CASE ("bullet") {
		bullet *o = new bullet;
		o->m_type = Type::Bullet;
		return o;
	}
	CASE ("dark_matter") {
		dark_matter *o = new dark_matter;
		o->m_type = Type::Dark_matter;
		return o;
	}
	CASE ("ship") {
		ship *o = new ship;
		o->m_type = Type::Ship;
		return o;
	}
	DEFAULT {
		error ("wrong type name '" + id + "'");
	}
}