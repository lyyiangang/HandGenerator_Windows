/**
*	Alvise Memo, 20/10/2015
*	Multimedia Technology and Telecommunications Laboratory, University of Padova
*	HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
*	The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.
*
*	\file HandGenerator.cpp
**/

#include "HandGenerator.h"
#include "Hand.h"

#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>

#define HANDGENERATOR_TIME_LOG 0

#if HANDGENERATOR_TIME_LOG == 1
#include <fstream>
//std::string log_global_path = std::string(std::getenv("AM_SOFTWAREDATA_DIR")) + "\\data_for_log\\";
std::string log_global_path = "C:\\am_softwaredata\\data_for_log\\";
#endif

#define WINDOW_WIDTH_START 800
#define WINDOW_HEIGHT_START 800
#define FPS_LIMIT 60
#define JOINT_COUNT 19
#define MAX_FILE_DIMENSION_MB 1024
#define WR(x) std::cout << x << std::flush
#define WRL(x) std::cout << x << std::endl

class HandGuiHelp
{
	class TrackBar
	{
	private:
		int wx;
		int wy;
		float px;
		float py_start;
		float py_end;
		float py;
		int mx_p;
		int my_p;
		float v_min, v_max;

		int m_delta;

		bool is_clicking_before;
		bool is_over;
		bool is_integer;

	public:
		float val;
		float c_r;
		float c_g;
		float c_b;

		static const int radius = 10;
		TrackBar() {};
		TrackBar(float pos_x, float up, float bottom, float min, float max, float curr, bool integer)
		{
			is_integer = integer;

			px = pos_x;
			py_start = up;
			py_end = bottom;
			val = curr;
			v_min = min;
			v_max = max;

			wx = 1;
			wy = 1;

			m_delta = -10000;
			is_clicking_before = false;
			is_over = false;

			UpdatePos();

			c_r = 1.0f;
			c_g = 0.0f;
			c_b = 1.0f;
		}
		void UpdateDimension(int w, int h)
		{
			wx = w;
			wy = h;

			UpdatePos();
		}
		void UpdatePositionX(int n_w)
		{
			px = (float)n_w;
		}
		void UpdateValue()
		{
			float step = (wy - (py_start + py_end)) / (v_max - v_min);
			val = (py + v_min * step) / step;
			if (is_integer)
				val = (val < 0.0f) ? ceilf(val - 0.5f) : floorf(val + 0.5f);

			if (val < v_min)
				val = v_min;
			if (val > v_max)
				val = v_max;

			UpdatePos();
		}
		void UpdatePos()
		{
			float step = (wy - (py_start + py_end)) / (v_max - v_min);
			py = val * step - v_min * step;
		}
		bool UpdateMouse(int mx, int my, bool is_down)
		{
			is_over = isClicking(mx, my) || is_clicking_before;

			if (is_down)
			{
				if (m_delta == -10000)
				{
					is_clicking_before = isClicking(mx, my);
					m_delta = (int)py - my;
				}

				if (is_clicking_before)
				{
					py = (float)(my + m_delta);
					UpdateValue();
				}
			}
			else
			{
				is_clicking_before = false;
				m_delta = -10000;
			}

			return is_clicking_before;
		}

		bool isClicking(int x, int y)
		{
			float dist_c = sqrtf(pow(x - px, 2) + pow(y - py - py_start, 2));
			return dist_c < radius;
		}
		void Render()
		{
			//assert(wy - (py_start + py_end) > 0);

			glColor3f(c_r * 0.9f, c_g * 0.9f, c_b * 0.9f);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, wx, wy, 0.0, 0.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glBegin(GL_LINES);
			glColor3f(c_r * 0.9f, c_g * 0.9f, c_b * 0.9f);
			glVertex3f(px , py_start, 0);
			glVertex3f(px , wy - py_end, 0.0f);
			if (is_integer)
			{
				float step = (wy - (py_start + py_end)) / (v_max - v_min);
				for (int i = (int)v_min; i <= (int)v_max; i++)
				{
					glVertex3f(px - 5, py_start + i * step, 0);
					glVertex3f(px + 5, py_start + i * step, 0.0f);
				}
			}
			glEnd();

			glPointSize((GLfloat)radius);
			//if (is_over)
			//	glColor3f(c_r * 1.0f, c_g * 1.0f, c_b * 1.0f);


			glBegin(GL_POINTS);
			glVertex3f(px, py + py_start, 0.0f);
			if (is_over)
			{
				glVertex3f(px - 2.0f, py + py_start, 0.0f);
				glVertex3f(px + 2.0f, py + py_start, 0.0f);
			}
			glEnd();
			glPointSize(1.0f);
			glRasterPos2f(px + radius, py + py_start);
			std::string sss = convert(val);
			glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)convert(val).c_str());
		}
		std::string convert(float value)
		{
			std::stringstream ss;
			ss.precision(4);
			ss << value;
			return ss.str();
		}
	};

private:

	std::clock_t fps_limiter;

	TrackBar tb_x;
	TrackBar tb_y;
	TrackBar tb_z;
	TrackBar tb_index;
	TrackBar tb_pos;

	Hand hand;
	int w, h;
	int m_x_s, m_y_s;
	float m_x_d, m_y_d;
	float m_m_w;
	int joint_index;
	int positions_index;
	int current_pos[JOINT_COUNT];
	bool is_down;

	void display_idle()
	{
		glutPostRedisplay();
	}
	void display(void)
	{
		fps_limiter = std::clock();
        glColor3b(22, 33, 44);
		// If the finger-joint index changes
		if ((int)tb_index.val != joint_index)
		{
			// Save current finger-joint position selection
			current_pos[joint_index] = positions_index;

			//Updating global finger-joint reference to the new finger
			joint_index = (int)tb_index.val;

			// Updating the position index with the saved finger-joint position
			positions_index = current_pos[joint_index];

			// Get values for the position of the new finger-joint
			hand.GetJointRotation(joint_index, positions_index, tb_x.val, tb_y.val, tb_z.val);

			// Update the position trackbar
			int number_of_positions;
			hand.GetJointPos(joint_index, number_of_positions);
			tb_pos = TrackBar((float)w - 20.0f, 300.0f, 10.0f, 0.0f, (float)number_of_positions - 1.0f, (float)positions_index, true);
			tb_pos.UpdateDimension(w, h);
			tb_x.UpdatePos();
			tb_y.UpdatePos();
			tb_z.UpdatePos();
		}
		if ((int)tb_pos.val != positions_index)
		{
			positions_index = (int)tb_pos.val;
			hand.GetJointRotation(joint_index, positions_index, tb_x.val, tb_y.val, tb_z.val);
			tb_x.UpdatePos();
			tb_y.UpdatePos();
			tb_z.UpdatePos();
		}

		//if ((int)tb_index.val != joint_index ||
		//	(int)tb_pos.val != positions_index ||
		//	old_x != tb_x.val ||
		//	old_y != tb_y.val ||
		//	old_z != tb_z.val)
		//{
		//	old_x = tb_x.val;
		//	old_y = tb_y.val;
		//	old_z = tb_z.val;

		hand.SetJoint(joint_index, positions_index, tb_x.val, tb_y.val, tb_z.val);
		//}
		hand.Render(w, h, m_x_d, m_y_d, 0.0f, true, m_m_w);

		m_x_d = 0.0f;
		m_y_d = 0.0f;

		tb_x.Render();
		tb_y.Render();
		tb_z.Render();
		tb_index.Render();
		tb_pos.Render();

		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(50.0f, 10.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)hand.GetJointName(joint_index).c_str());
		glRasterPos2f((float)w - 140.0f, 10.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"Positions control:\n'S' to save positions\n'R' to reset position\n'A' to animation mode\n'P' to position mode\n'E' to leave context");
        {//lyy
          //  hand.DrawDepthData(w, h);
        }
		glutSwapBuffers();

		double elapsed_time = (std::clock() - fps_limiter) / (double)CLOCKS_PER_SEC;
		double fps_diff = (1.0 / (double)FPS_LIMIT) - elapsed_time;
		if (fps_diff > 0.0)
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(fps_diff * 1000.0)));
	}

	void reshape(int wp, int hp)
	{
		w = wp;
		h = hp;
		tb_x.UpdateDimension(w, h);
		tb_y.UpdateDimension(w, h);
		tb_z.UpdateDimension(w, h);
		tb_index.UpdateDimension(w, h);
		tb_pos.UpdateDimension(w, h);
		tb_pos.UpdatePositionX(w - 20);
	}

	void keyboard_key(unsigned char key, int xx, int yy)
	{
		switch (key)
		{
		case 'r':
			// Setting all finger-joint current position angles to zero
			hand.SetJoint(joint_index, positions_index, 0.0f, 0.0f, 0.0f);
			hand.GetJointRotation(joint_index, positions_index, tb_x.val, tb_y.val, tb_z.val);
			tb_x.UpdatePos();
			tb_y.UpdatePos();
			tb_z.UpdatePos();
			WRL("Resetted joint: " << joint_index << " pos: " << positions_index);
			break;
		case 'a':
			hand.SwitchAnimationStatic(true);
			WRL("Activated STATIC mode");
			break;
		case 'p':
			hand.SwitchAnimationStatic(false);
			WRL("Activated ANIMATION mode");
			break;
		case 's':
			hand.SaveProperties();
			WRL("Positions saved");
			break;
		case 'e':
			WRL("Exit");
			glutLeaveMainLoop();
			break;
        case '1':
            WRL("reset all joints");
            hand.InitAllJoints();
            for(int ii=0;ii<JOINT_COUNT;++ii)
                hand.GetJointRotation(ii, 0, tb_x.val, tb_y.val, tb_z.val);
            tb_x.UpdatePos();
            tb_y.UpdatePos();
            tb_z.UpdatePos();
            break;
            
		}
	}
	void mouse_move(int x, int y)
	{
		bool is_pressed = false;
		is_pressed = is_pressed || tb_x.UpdateMouse(x, y, is_down);
		is_pressed = is_pressed || tb_y.UpdateMouse(x, y, is_down);
		is_pressed = is_pressed || tb_z.UpdateMouse(x, y, is_down);
		is_pressed = is_pressed || tb_index.UpdateMouse(x, y, is_down);
		is_pressed = is_pressed || tb_pos.UpdateMouse(x, y, is_down);

		if (is_down && !is_pressed)
		{
			m_x_d = (float)(x - m_x_s);
			m_x_s = x;
			m_y_d = (float)(y - m_y_s);
			m_y_s = y;
		}
	}
	void mouse_button(int button, int state, int x, int y)
	{
		// only start motion if the left button is pressed
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
		{
			is_down = true;
			m_x_s = x;
			m_y_s = y;
		}
		else
		{
			is_down = false;

		}

		mouse_move(x, y);
	}
	void mouse_wheel(int wheel, int direction, int x, int y)
	{
		m_m_w += direction * 0.5f;
	}

	HandGuiHelp()
	{
		//std::cout << "HandGuiHelp1" << std::endl;

		if (!hand.Init(*GetParameters()))
		{
			initiation_ok = false;			
			return;
		}

		//std::cout << "HandGuiHelp2" << std::endl;

		w = 1, h = 1;
		m_x_s = -1, m_y_s = -1;
		m_x_d = 0.0f, m_y_d = 0.0f;
		m_m_w = 0.0f;
		joint_index = 0;
		positions_index = -1;
		for (int i = 0; i < JOINT_COUNT; i++)
			current_pos[i] = 0;
		is_down = false;

		tb_x = TrackBar(50.0f, 20.0f, 20.0f, -180.0f, 180.0f, 0.0f, false);
		tb_x.c_r = 1.0f; tb_x.c_g = 0.0f; tb_x.c_b = 0.0f;
		tb_y = TrackBar(70.0f, 20.0f, 20.0f, -180.0f, 180.0f, 0.0f, false);
		tb_y.c_r = 0.0f; tb_y.c_g = 1.0f; tb_y.c_b = 0.0f;
		tb_z = TrackBar(90.0f, 20.0f, 20.0f, -180.0f, 180.0f, 0.0f, false);
		tb_z.c_r = 0.0f; tb_z.c_g = 0.0f; tb_z.c_b = 1.0f;
		tb_index = TrackBar(20.0f, 20.0f, 20.0f, 0.0f, 18.0f, 0.0f, true);
		tb_pos = TrackBar(110.0f, 300, 10.0f, 0.0f, 1.0f, 0.0f, true);
		reshape(WINDOW_WIDTH_START, WINDOW_HEIGHT_START);
		for (int i = 0; i < JOINT_COUNT; i++)
			current_pos[i] = 0;

		initiation_ok = true;
	}
	HandGuiHelp(HandGuiHelp const&);// = delete;
	void operator=(HandGuiHelp const&);// = delete;
public:
	bool initiation_ok;
	static HandParameters * GetParameters()
	{
		static HandParameters static_instance;
		return &static_instance; 
	}
	static HandGuiHelp * GetInstance()
	{
		static HandGuiHelp static_instance;
		return &static_instance; 
	}
	static void s_idle_display(void) { GetInstance()->display_idle(); };
	static void s_display(void) { GetInstance()->display(); };
	static void s_reshape(int wp, int hp) { GetInstance()->reshape(wp, hp); };
	static void s_keyboard_key(unsigned char key, int xx, int yy) { GetInstance()->keyboard_key(key, xx, yy); };
	static void s_mouse_move(int x, int y) { GetInstance()->mouse_move(x, y); };
	static void s_mouse_button(int button, int state, int x, int y) { GetInstance()->mouse_button(button, state, x, y); };
	static void s_mouse_wheel(int wheel, int direction, int x, int y) { GetInstance()->mouse_wheel(wheel, direction, x, y); };
};
class HandProHelp
{
private:
	GLuint stf_fbo;
	GLuint stf_pboIds[2];

	Hand hand;
	int count;
	int current_setup;
	int current_rotation;
	std::vector<std::vector<int>> positions;
	struct rotation_values
	{
		float r_x;
		float r_y;
		float r_z;

		rotation_values(float x, float y, float z)
		{
			r_x = x;
			r_y = y;
			r_z = z;
		}
	};
	std::vector<rotation_values> rotations;

	std::string file_name;

	bool job_finished;

	unsigned char * mem_data;
	int data_size;
	int max_frame;

	void ComputePosition(std::vector<std::vector<int>>* cont, std::vector<int> to_add, int curr_joint)
	{
		if (curr_joint < JOINT_COUNT)
		{
			int n_of_pos;
			hand.GetJointPos(curr_joint, n_of_pos);

			to_add.push_back(0);

			for(int i = 0; i < n_of_pos; i++)
			{
				to_add[to_add.size() - 1] = i;
				ComputePosition(cont, to_add, curr_joint + 1);
			}
		}
		else
		{
			if (hand.CheckForRules(to_add))
				cont->push_back(to_add);
			else
			{
				//WRL(to_add.size());
			}

			return;
		}
	}
	void ComputeRotation(std::vector<rotation_values>* out)
	{
		std::vector<float> rot_x;
		std::vector<float> rot_y;
		std::vector<float> rot_z;
		hand.GetHandRotations(rot_x, rot_y, rot_z);

		if (rot_x.size() < 1) rot_x.push_back(0.0f);
		if (rot_y.size() < 1) rot_y.push_back(0.0f);
		if (rot_z.size() < 1) rot_z.push_back(0.0f);
		for (int x = 0; x < rot_x.size(); x++)
			for (int y = 0; y < rot_y.size(); y++)
				for (int z = 0; z < rot_z.size(); z++)
					out->push_back(rotation_values(rot_x[x], rot_y[y], rot_z[z]));
		if (out->size() < 1)
			out->push_back(rotation_values(0.0f, 0.0f, -4.0f));
	}
	void copyToFile(GLubyte* data)
	{
		static int mem_count = 0;
		static int file_count = 0;

#if HANDGENERATOR_TIME_LOG == 1
		static std::vector<double> timing;
		static clock_t start;
		static clock_t end;
		if (mem_count == 0)
		{
			timing.clear();
			timing.reserve(max_frame);
			start = clock();
		}
		else
		{
			end = clock();
			timing.push_back(double(end - start) / CLOCKS_PER_SEC);

			std::ofstream outfile;
			outfile.open(log_global_path + "log_HandGenerator_interleaved.txt", std::ios::app);
			outfile << "HandGenerator - [" << clock() << "] - Number of computation: " << mem_count << " - Time: " << timing[mem_count - 1] << "\n";
			outfile.close();
			if ((mem_count >= max_frame) || (job_finished))
			{
				double sum = 0.0;
				for (int i = 0; i < mem_count; i++) sum += timing[i];
				sum /= mem_count;

				std::ofstream outfile;
				outfile.open(log_global_path + "log_HandGenerator_average.txt", std::ios::app);
				outfile << "HandGenerator - [" << clock() << "] - Number of computations: " << mem_count << " - Medium time: " << sum << "\n";
				outfile.close();
			}

			start = clock();
		}
#endif

		memcpy(&mem_data[mem_count * data_size], data, sizeof(unsigned char) * data_size);
		mem_count++;

		if ((mem_count >= max_frame) || (job_finished))
		{
			if (job_finished)
			{
				std::string f_n = std::to_string(file_count);
				while (f_n.size() < 4)
					f_n = '0' + f_n;
				f_n = file_name + f_n + ".data";
				//FILE* pFile;
				//fopen_s(&pFile, f_n.c_str(), "wb");
				FILE* pFile = fopen(f_n.c_str(), "wb");				
				fwrite(mem_data, sizeof(unsigned char), sizeof(unsigned char) * data_size * mem_count, pFile);
				fclose(pFile);
				mem_count = 0;
				file_count ++;
				std::cout << "saved file: " << f_n << std::endl;
				std::cout << "Job finished" << f_n << std::endl;
				glutLeaveMainLoop();
			}
			else
			{
				std::string f_n = std::to_string(file_count);
				while (f_n.size() < 4)
					f_n = '0' + f_n;
				f_n = file_name + f_n + ".data";
				//FILE* pFile;
				//fopen_s(&pFile, f_n.c_str(), "wb");
				FILE* pFile = fopen(f_n.c_str(), "wb");
				fwrite(mem_data, sizeof(unsigned char), sizeof(unsigned char) * data_size * max_frame, pFile);
				fclose(pFile);
				mem_count = 0;
				file_count ++;
				std::cout << "saved file: " << f_n << std::endl;
			}
		}
	}
	void display()
	{
		static bool is_first_run = true;

		count++;

		HandParameters * hp_ptr = GetParameters();

		static int frame_number = 0;
		static int index = 0;
		int nextIndex = 0; // pbo index used for next frame
		index = (index + 1) % 2; // "index" is used to read pixels from a framebuffer to a PBO
		nextIndex = (index + 1) % 2; // "nextIndex" is used to process pixels in the other PBO

		if (count % (positions.size() * rotations.size() / 10) == 0)
			std::cout << count << "/" << positions.size() * rotations.size() << std::endl;

		if (current_setup >= positions.size())
		{
			current_setup = 0;
			current_rotation++;
			if (current_rotation >= rotations.size())
			{
				current_rotation = 0;
				job_finished = true;
			}
		}
		for(int i = 0; i < JOINT_COUNT; i++) hand.SetJoint(i, positions[current_setup][i]);
		current_setup++;


		// 1: Bind FBO
		glBindFramebuffer(GL_FRAMEBUFFER, stf_fbo); // Bind our frame buffer for rendering

		hand.Render(hp_ptr->render_fbo_width, hp_ptr->render_fbo_height, rotations[current_rotation].r_x, rotations[current_rotation].r_y, rotations[current_rotation].r_z, false, -4.0f);

		// 3: Copy FBO to PBO
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, stf_pboIds[index]);
		glReadPixels(0, 0, hp_ptr->render_fbo_width, hp_ptr->render_fbo_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		//// 4: Process PBO
		glBindBuffer(GL_PIXEL_PACK_BUFFER, stf_pboIds[nextIndex]);
		GLubyte* src = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
		//assert(src);
		if (is_first_run)
			is_first_run = false;
		else
			copyToFile(src);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);     // release pointer to the mapped buffer

		//// 5: Unbind FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind our texture
       
		glutSwapBuffers();
	}

	HandProHelp()
	{
		WRL("Init start");
		HandParameters * hp_ptr = GetParameters();

		bool hand_success = hand.Init(*hp_ptr);
		if (!hand_success)
		{
			WRL("ERROR: Hand loading was not successfull, please check the parameters and/or your system");
			return;
		}
		WRL("hand OK");

		std::vector<int> temp;
		ComputePosition(&positions, temp, 0);
		WRL("positions OK");

		ComputeRotation(&rotations);
		WRL("rotations OK");		

		job_finished = false;
		current_rotation = 0;
		current_setup = 0;
		count = 0;

		// Number of byte per frame
		data_size = hp_ptr->render_fbo_width * hp_ptr->render_fbo_height * 4;
		// Number of frames per packet
		max_frame = (int)(MAX_FILE_DIMENSION_MB / ((double)data_size * 8.0 * 0.000000125));
		WRL("Session info: frame size-> " << data_size << " frames per packet-> " << max_frame);

		WR("Allocating memory..");
		mem_data = new unsigned char[max_frame * data_size];
		WRL("OK");

		std::stringstream ss;
		ss << hp_ptr->setup_output_folder_path << "d_" << positions.size() * rotations.size() << "_" << hp_ptr->render_fbo_width << "_" << hp_ptr->render_fbo_height << "_" << 4 << "_" << max_frame << ".";
		file_name = ss.str();
		WRL("File name template: " << file_name);

		// Setting up FBO and PBO for rendering offscreen and retrieving pixel informations from the GPU
		///////////////////////////////////////////////// FBO CREATION /////////////////////////////////////////////////////////////
		GLuint depthBuf;
		glGenRenderbuffers(1, &depthBuf);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, hp_ptr->render_fbo_width, hp_ptr->render_fbo_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		GLuint colorBuf;
		glGenRenderbuffers(1, &colorBuf);
		glBindRenderbuffer(GL_RENDERBUFFER, colorBuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, hp_ptr->render_fbo_width, hp_ptr->render_fbo_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuf);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glGenFramebuffers(1, &stf_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, stf_fbo);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuf);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			WRL("ERROR: FBO creation was not successfull, please check your system");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////// PBO CREATION /////////////////////////////////////////////////////////////
		glGenBuffers(2, stf_pboIds);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, stf_pboIds[0]);
		glBufferData(GL_PIXEL_PACK_BUFFER, data_size, 0, GL_STREAM_READ);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, stf_pboIds[1]);
		glBufferData(GL_PIXEL_PACK_BUFFER, data_size, 0, GL_STREAM_READ);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		WRL("Everything OK");
	}
	HandProHelp(HandProHelp const&);// = delete;
	void operator=(HandProHelp const&);// = delete;
public:
	static HandParameters * GetParameters()
	{
		static HandParameters static_instance;
		return &static_instance; 
	}
	static HandProHelp * GetInstance()
	{
		static HandProHelp static_instance;
		return &static_instance; 
	}
	static void s_display(void) { GetInstance()->display(); };
};

void HandGenerator::Run(HandParameters parameters, bool is_gui)
{
	if (is_gui)
	{
		std::cout << "HANDGENERATOR v1.0, Memo Alvise, 05/2015\nGRAPHIC USER INTERFACE MODULE\n\n" << std::endl;

		int argc = 1;
		char * argv[] = {"HandGenerator 1.0, Alvise Memo"};
		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
		glutInitWindowSize(WINDOW_WIDTH_START,WINDOW_HEIGHT_START);
		glutInitWindowPosition(100,100);

		glutCreateWindow(argv[0]);

		glewInit();

		// Creation of singleton instance for static usage
		HandParameters * hp = HandGuiHelp::GetParameters();
		parameters.CopyTo(hp);
		if (!HandGuiHelp::GetInstance()->initiation_ok) return;

		glutIgnoreKeyRepeat(true);
		glutKeyboardFunc(HandGuiHelp::s_keyboard_key);
		glutMouseFunc(HandGuiHelp::s_mouse_button);
		glutMotionFunc(HandGuiHelp::s_mouse_move);
		glutPassiveMotionFunc(HandGuiHelp::s_mouse_move);
		glutMouseWheelFunc(HandGuiHelp::s_mouse_wheel);

		glutDisplayFunc(HandGuiHelp::s_display);
		glutIdleFunc(HandGuiHelp::s_idle_display);
		glutReshapeFunc(HandGuiHelp::s_reshape);

		glutMainLoop();
	}
	else
	{
		std::cout << "HANDGENERATOR v1.0, Memo Alvise, 05/2015\nPROCESSING MODE\n\n" << std::endl;

		int argc = 1;
		char * argv[] = {"HandGenerator 1.0, Alvise Memo"};
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
		glutInitWindowPosition(0, 10000);
		glutInitWindowSize(1, 1);
		glutCreateWindow(argv[0]);
		glutHideWindow();

		glewInit();

		// Creation of singleton instance for static usage
		HandParameters * hp = HandProHelp::GetParameters();
		parameters.CopyTo(hp);
		HandProHelp::GetInstance();

		glutDisplayFunc(HandProHelp::s_display);
		glutIdleFunc(HandProHelp::s_display);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

		glutMainLoop();
	}
};
