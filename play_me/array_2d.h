#pragma once
#include "math_3d.h"
#include "glob.h"

// дефайны форов, которые часто используются при обходе двухмерных массивов
#define FOR_2D(v, W, H)		for (v2i v(0,0); v.y < (H); ++v.y) for (v.x = 0; v.x < (W); ++v.x)
#define FOR_ARRAY_2D(v, array_2d_obj)	for (v2i s_init((array_2d_obj).get_Shift ()), s_WH(s_init + (array_2d_obj).get_WH()), v(s_init); v.y < s_WH.y; ++v.y) \
	for (v.x = s_init.x; v.x < s_WH.x; ++v.x)

// двухмерный массив, элементами которого могут быть объекты любого типа
template <class T>
class array_2d {
public:
	T *m_a;		// указатель на сам массив
	int m_W, m_H; // ширина и высота
	bool m_shift; // есть ли смщение
	bool m_initiated; // инициирован ли массив
	int m_x_shift, m_y_shift; // смещение
public:
	// задать смещение
	void set_shift (int x_shift, int y_shift) {
		if (x_shift || y_shift) {
			m_shift = true;
		}
		m_x_shift = x_shift;
		m_y_shift = y_shift;
	}
	// обращение к элементу
	T& operator [] (v2i index) {
		if (m_shift) {
			index.x -= m_x_shift;
			index.y -= m_y_shift;
		}
		if (! (index.x >= 0 && index.y >= 0 && index.x < m_W && index.y < m_H)) {
			ERR_BREAK;
		}
		return m_a [index.x + m_W * index.y];
	}
	// синоним обращения к элементу
	T& at (v2i index) {
		if (m_shift) {
			index.x -= m_x_shift;
			index.y -= m_y_shift;
		}
		if (! (index.x >= 0 && index.y >= 0 && index.x < m_W && index.y < m_H)) {
			ERR_BREAK;
		}
		return m_a [index.x + m_W * index.y];
	}
	// принадлежит ли точка массиву (можно ли обратиться к элементу массива с таким индексом?)
	bool operator << (v2i index) { // returns true if index is valid
		if (m_shift) {
			index.x -= m_x_shift;
			index.y -= m_y_shift;
		}
		return index.x >= 0 && index.y >= 0 && index.x < m_W && index.y < m_H;
	}
	// узнать ширину
	int get_W () {
		return m_W;
	}
	// узнать высоту
	int get_H () {
		return m_H;
	}
	// узнать размер в виде v2i
	v2i get_WH () {
		return v2i (m_W, m_H);
	}
	// узнать смещение v2i
	v2i get_Shift () {
		return v2i (m_x_shift, m_y_shift);
	}
	// выделение памяти для массива. начало работы, инициализация
	void init (int W, int H) {
		if (m_initiated) {
			delete []m_a;
		}
		if (W <= 0 || H <= 0) {
			ERR_BREAK;
		}
		m_a = new T [W*H];
		m_W = W;
		m_H = H;
		m_shift = false;
		m_x_shift = 0;
		m_y_shift = 0;
		m_initiated = true;
	}
	// конструктор копирования
	array_2d (const array_2d& r) {
		if (!r.m_initiated) {
			m_initiated = false;
			return;
		}
		m_initiated = true;
		m_a = new T [r.m_H * r.m_W];
		m_W = r.m_W;
		m_H = r.m_H;
		m_shift = r.m_shift;
		m_x_shift = r.m_x_shift;
		m_y_shift = r.m_y_shift;
		memcpy (m_a, r.m_a, m_W * m_H * sizeof (T));
	}
	// оператор присваивания
	array_2d& operator = (const array_2d& r) {
		if (!r.m_initiated) {
			m_initiated = false;
			return (*this);
		}
		if (m_initiated) {
			if (m_H == r.m_H && m_W == r.m_W) {
				memcpy (m_a, r.m_a, m_W * m_H * sizeof (T));
				return (*this);
			}
			delete []m_a;
		}
		m_initiated = true;
		m_a = new T [r.m_H * r.m_W];
		m_W = r.m_W;
		m_H = r.m_H;
		m_shift = r.m_shift;
		m_x_shift = r.m_x_shift;
		m_y_shift = r.m_y_shift;
		memcpy (m_a, r.m_a, m_W * m_H * sizeof (T));
		return (*this);
	}
	// пустой конструктор
	array_2d () {
		m_initiated = false;
	}
	void destroy () {
		if (m_initiated) {
			delete []m_a;
			m_initiated = false;
		}
	}
	~array_2d () {
		destroy ();
	}
	// преобразование к указателю
	operator T* () {
		return m_a;
	}
	// заполнить все ячейки массива одним и тем же значением
	void clear (T t) {
		FOR (i, m_W * m_H) {
			m_a[i] = t;
		}
	}
};

// rgba текстура (массив)
struct rgba_array : public array_2d <CLR> {
	bool alpha_matters;  // обращать ли внимание на прозрачность при его рисовании?
	void draw (rgba_array *src, v2i pos);	// нарисовать на this массив src в точке приложения pos
	void draw (rgba_array *src, v2i pos, int scale, v2i max_size = v2i (0,0)); // отрисовка с масштабом и максимальным размером рендеринга
	void draw_alpha (rgba_array *src, v2i pos, float alpha1);
	void draw (rgba_array *src, v2i pos, int scale, v2i max_size, v2i src_start);
	rgba_array (bool _alpha_matters = 0) { // конструктор
		alpha_matters = _alpha_matters;
	}
};