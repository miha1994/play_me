#include "space.h"
#include "read_write.h"
#include "fadma.h"
#include <map>
#include "glob.h"

Space::Space (string name) {
	string tmp_str = prefix_path + "inf/" + name + "_space.txt";
	freopen (tmp_str.c_str (), "r", stdin);
	read_string (); // "{"
	string chapter;

	for (chapter = read_string (); chapter != "}"; chapter = read_string ()) {
		SWITCH (chapter)
		CASE ("UPDATE_SEQUENCE") {
			read_string (); // "{"
			string str;
			while ((str = read_string ()) != "}") {
				if (m_objects.find (str) == m_objects.end ()) {
					m_objects[str] = new_Object (str);
				}
				m_objects_to_update.push_back (m_objects[str]);
			}
		}
		CASE ("RENDER_SEQUENCE") {
			read_string (); // "{"
			string str;
			while ((str = read_string ()) != "}") {
				if (m_objects.find (str) == m_objects.end ()) {
					m_objects[str] = new_Object (str);
				}
				m_objects_to_render.push_back (m_objects[str]);
			}
		}
		CASE ("DEF") {
			read_string (); // "{"
			string str;
			while ((str = read_string ()) != "}") {
				if (m_objects.find (str) == m_objects.end ()) {
					m_objects[str] = new_Object (str);
				}
				read_string (); // "{"
				m_objects[str]->load ();
			}
		}
		DEFAULT {
			error ("symbol '" + chapter + "' in file   res/inf/" + name + "_space.txt    expected 'UPDATE_SEQUENCE' or 'DEF' or ...");
		}
	}
}