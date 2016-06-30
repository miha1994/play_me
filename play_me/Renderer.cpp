#include "Renderer.h"
#include "pure_opengl.h"
#include "glob.h"
#include "input.h"

bool g_fullscreen = 0;
GLfloat colors[D_W*D_H];
GLuint colors_location;

void Renderer::draw_everything () {
	static CLR black_ar[D_W * D_H];
	for each (auto var in black_ar) {
		var = CLR::Black;
	}
	memcpy (m_tex_array, black_ar, sizeof (black_ar));
	forstl (p, m_render_list) {
		m_tex_array.draw (p->rgba_ar, p->pos, p->scale);
	}
	m_render_list.clear ();

	forstl (p, m_sp_effect_square) {
		p->apply (m_tex_array);
	}
	m_sp_effect_square.clear ();

	//  old school screen effect
	if (in.kb['A'].pressed_now) {
		rgba_array na (m_tex_array);
		memset (na.m_a, 0, D_W * D_H * sizeof (CLR));
#define ma	m_tex_array
#define def(arg,ar2)	if (na << (v + v2i (arg,ar2))) {			na[v + v2i (arg,ar2)] = na[v + v2i (arg,ar2)] ^ ma[v];		}	
		FOR_2D (v, D_W, D_H) {
			def (1,0)
			def (-1,0)
		}
		FOR_2D (v, D_W, D_H) {
			ma[v] = (ma[v] * 0.7) ^ (na[v] * 0.3);
		}
#undef def
#undef ma
	}
	// end of code of old school screen effect

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_tex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_tex), (const GLvoid*)12);
	glActiveTexture(GL_TEXTURE0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_tex_array.get_W (), m_tex_array.get_H (), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_tex_array);

    glBindTexture(GL_TEXTURE_2D, m_textureObj);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::init () {
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	m_screen[0].pos = v3f (-1,-1,0);
	m_screen[0].tex_coord = v2f (0,1);
	m_screen[1].pos = v3f (-1,1,0);
	m_screen[1].tex_coord = v2f (0,0);
	m_screen[2].pos = v3f (1,1,0);
	m_screen[2].tex_coord = v2f (1,0);
	m_screen[3].pos = v3f (1,-1,0);
	m_screen[3].tex_coord = v2f (1,1);

	vector <int> m_indices;
	FOR (i, 4) {
		m_indices.push_back (i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_tex) * 4, m_screen, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indices.size (), &m_indices[0], GL_STATIC_DRAW);

	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_2D, m_textureObj);

	m_tex_array.init (D_W, D_H);

	sprite.scale = 1;
}

void Renderer::add_sprite_to_render_list () {
	m_render_list.push_back (sprite);
}



#define def_loop		For (radius + i) { \
			src = dst + offset; \
			if (ar << dst && ar << src) { \
				ar[dst] = ar[src]; \
			} \
			dst = dst + stp; \
		}

void Renderer::sp_effect_square_info::apply (rgba_array &ar) {
	v2i strt;
	v2i stp, offset;
	v2i src, dst;
	FOR (i, thickness) {
		strt = v2i (0, -(radius+i));
		stp = v2i(1,1), offset = v2i(-thickness, thickness);
		dst = pos + strt;
		def_loop;
		strt = v2i (0, (radius+i));
		stp = v2i(1,-1), offset = v2i(-thickness, -thickness);
		dst = pos + strt;
		def_loop;
		strt = v2i (0, -(radius+i));
		stp = v2i(-1,1), offset = v2i(thickness, thickness);
		dst = pos + strt;
		def_loop;
		strt = v2i (0, (radius+i));
		stp = v2i(-1,-1), offset = v2i(thickness, -thickness);
		dst = pos + strt;
		def_loop;
	}
}

#undef def_loop