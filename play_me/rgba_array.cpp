#include "array_2d.h"

#define SIZE	

void rgba_array::draw (rgba_array *src, v2i pos) {
	int r_w = Min (m_W, pos.x + src->m_W) - Max (0, pos.x);
	int r_h = Min (m_H, pos.y + src->m_H) - Max (0, pos.y);
	v2i start (0,0);
	if (pos.x < 0) {
		start.x = -pos.x;
		pos.x = 0;
	}
	if (pos.y < 0) {
		start.y = -pos.y;
		pos.y = 0;
	}
	if (r_w <= 0 || r_h <= 0) {
		return;
	}
	if (src->alpha_matters) {
		CLR *s, *d;
		float d_p, s_p;
		FOR_2D (v, r_w, r_h) {
			s = &src->at (start + v);
			if (!s->a) {
				continue;
			}
			d = &this->at (pos + v);
			d_p = (1 - (s->a / 255.0f)) * (d->a / 255.0f);
			s_p = 1 - d_p;
			d->r = d_p * d->r + s_p * s->r;
			d->g = d_p * d->g + s_p * s->g;
			d->b = d_p * d->b + s_p * s->b;
			d->a = d->a + ((255-d->a)/255.0) * s->a;
		}
	} else {
		if (m_W == src->m_W && r_w == m_W) {
			memcpy (m_a + m_W * pos.y, src->m_a, r_h * r_w * sizeof (CLR));
		} else {
			FOR (i, r_h) {
				memcpy (m_a + pos.x + m_W * (pos.y + i), src->m_a + src->m_W * (i + start.y) + start.x, r_w * sizeof (CLR));
			}
		}
	}
}

void rgba_array::draw (rgba_array *src, v2i pos, int scale, v2i max_size) {
	if (scale == 1) {
		draw (src, pos);
		return ;
	}
	int Ws = src->m_W * scale, Hs = src->m_H * scale;
	int r_w = Min (m_W, pos.x + Ws) - Max (0, pos.x);
	int r_h = Min (m_H, pos.y + Hs) - Max (0, pos.y);
	if (max_size != v2i (0,0)) {
		r_w = Min (r_w, max_size.x);
		r_h = Min (r_h, max_size.y);
	}
	v2i start (0,0);
	if (pos.x < 0) {
		start.x = -pos.x;
		pos.x = 0;
	}
	if (pos.y < 0) {
		start.y = -pos.y;
		pos.y = 0;
	}
	if (r_w <= 0 || r_h <= 0) {
		return;
	}
	if (src->alpha_matters) {
		CLR *s, *d;
		float d_p, s_p;
		FOR_2D (v, r_w, r_h) {
			s = &src->at ((start + v) / scale);
			if (!s->a) {
				continue;
			}
			d = &this->at (pos + v);
			d_p = (1 - (s->a / 255.0f)) * (d->a / 255.0f);
			s_p = 1 - d_p;
			d->r = d_p * d->r + s_p * s->r;
			d->g = d_p * d->g + s_p * s->g;
			d->b = d_p * d->b + s_p * s->b;
			d->a = d->a + ((255-d->a)/255.0) * s->a;
		}
	} else {
		FOR_2D (v, r_w, r_h) {
			this->at (pos + v) = src->at ((start + v) / scale);
		}
	}
}

void rgba_array::draw (rgba_array *src, v2i pos, int scale, v2i max_size, v2i src_pos) {
	if (max_size == v2i (0,0)) {
		max_size = src->get_WH ();
	}
	int Ws = Min (src->m_W, max_size.x) * scale, Hs = Min (src->m_H, max_size.y) * scale;
	int r_w = Min (m_W, pos.x + Ws) - Max (0, pos.x);
	int r_h = Min (m_H, pos.y + Hs) - Max (0, pos.y);

	v2i start (src_pos * scale);
	if (pos.x < 0) {
		start.x -= pos.x;
		pos.x = 0;
	}
	if (pos.y < 0) {
		start.y -= pos.y;
		pos.y = 0;
	}
	if (r_w <= 0 || r_h <= 0) {
		return;
	}
	if (src->alpha_matters) {
		CLR *s, *d;
		float d_p, s_p;
		FOR_2D (v, r_w, r_h) {
			s = &src->at ((start + v) / scale);
			if (!s->a) {
				continue;
			}
			d = &this->at (pos + v);
			d_p = (1 - (s->a / 255.0f)) * (d->a / 255.0f);
			s_p = 1 - d_p;
			d->r = d_p * d->r + s_p * s->r;
			d->g = d_p * d->g + s_p * s->g;
			d->b = d_p * d->b + s_p * s->b;
			d->a = d->a + ((255-d->a)/255.0) * s->a;
		}
	} else {
		FOR_2D (v, r_w, r_h) {
			this->at (pos + v) = src->at ((start + v) / scale);
		}
	}
}