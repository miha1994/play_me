#include "png.h"
#include "lodepng.h"

bool load_png (rgba_array &texture, string file_name) {
	unsigned int w,h;
	vector <unsigned char> cpy;
	auto err = lodepng::decode (cpy, w, h, file_name);
	if (err) {
		std::cout << "decoder error " << err << ": " << lodepng_error_text(err) << std::endl;
		return false;
	}
	texture.init (w,h);
	memcpy (texture, &cpy[0], w*h*4);
	return true;
}

bool load_png (rgba_array &texture, string file_name, IntRect rect) {
	unsigned int w,h;
	vector <unsigned char> cpy;
	auto err = lodepng::decode (cpy, w, h, file_name);
	if (err) {
		std::cout << "decoder error " << err << ": " << lodepng_error_text(err) << std::endl;
		return false;
	}
	rgba_array full;
	full.init (w,h);
	memcpy (full, &cpy[0], w*h*4);

	texture.init (rect.size.x, rect.size.y);
	texture.clear (CLR(0,0,0,0));
	texture.draw (&full, -rect.pos);
	
	return true;
}

bool save_png (rgba_array &texture, string file_name) {
	vector <unsigned char> image;
	unsigned int w = texture.m_W, h = texture.m_H;
	image.assign (w * h * 4, 0);
	memcpy (&image[0], texture, w * h * 4);
	auto err = lodepng::encode (file_name, image, w, h);
	return !err;
}