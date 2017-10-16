GUIDE FOR HANDGENERATOR

Alvise Memo, 20/10/2015
Multimedia Technology and Telecommunications Laboratory, University of Padova

HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.



0) INTRODUCTION
		This library is intended for providing a fast and easy way of computing synthetic dataset with a high-definition
		fully-customizable environment. Every aspect of the 3D model can be modified, as long as it mantains the same bone-structure
		and bone-name as in the model provided by M. Saric. The renderizaion pipeline is very straight forward, and the user can modify
		the shader output, as the one provided to output depth-data.

1) STRUCTURE
		The HandGenerator rely:
			1.a) POSE DEFINITIONS saved on *.property files
					This defines for each bone, a set of position that it can have in relation to it's bone predecessor.
					Also defines the exact possible values for each rotation axis, needed for processing.
					(AT LEAST one angle for each axis must be given and one position for each finger also)
					Example: (file bin/hand_test.property)
						rotationx -20 0 0 -> defines a possible angle at which the hand must be rendered.
						finger5joint1 -16.9745 4.24361 18.7426	-> defines a possible rotation around the three axis for
																the finger 5, joint 1.
			
			1.b) RULES DEFINITIONS created on *.rules files
					This defines for each finger-joint a condition that MUST (w) or MUST NOT (wo) apply to a possible
					hand-pose (the set of the whole finger-joint positions taken by the hand in a precise pose) to be
					valid. Each position is defined by its ordered numer on the possible finger-joint positions.
					Example: (file bin/hand.property.rules)
						finger1joint1 1 w finger2joint1 0	-> defines that each hand position where the finger1joint1
															assumes its 2nd (counting from 0 to ...) possible position,
															then finger2joint1 MUST assume its 1st possible position.
															Also viceversa. This example implies that finger1joint1 has
															at least 2 defined positions on the loaded *.property file.
					This files is useful for generating multiple pose from a single set of finger positions, but is a bit
					tricky to understand, for more informations contact the Author.

2) MODUS OPERANDI
		The HandGenerator has two possible utilizations. A GUI for pose-setup, and a PROCESSING mode for fast off-screen generation
		of the required hand-pose.
			2.a) GUI MODE
					The LEFT sliders are in order for
						- finger/joint selection
						- X/Y/Z rotation modification
					The RIGHT slider is for the position selection (if more than one is present).
					Keys:
						S -> saves the current fingerS/jointS position in a *.property file
						R -> reset current join, zeroing it's positions and its values
						A/P -> switch between pose setup and loaded animation view
						E -> exit

			2.b) PROCESSING MODE
					After being called with the required parameters, it produces as output a file *.data. Example of the file name is
						d_3200_200_200_4_6400.0000.data  -> 3200 are the number of positions required
															200 is the width of the frame
															200 is the height of the frame
															4 is the number of channel in the frame
															6400 is the double of the total frames possible in the maximum file dimension specified in the library (1024 MB)
															0000 is the index of the file (if positions exceed the number of max possible frames per file, then the next one
																							will have index 0001, 0002, ...)
					This file contains raw image data, easily readable as:
					
							int frame_number = 3200;
							int frame_size = 200 * 200 * 4;
							int data_size = frame_size * frame_number;
							unsigned char * buf = new unsigned char[data_size];

							FILE* pFile;
							fopen_s(&pFile, file_path.c_str(), "rb");
							fread(buf, sizeof(unsigned char), sizeof(unsigned char) * data_size, pFile);
							fclose(pFile);

							for(int i = 0; i < frame_number; i++)
							{
								////// OPENCV VISUALIZATION FOR MANIPULATION ///////
								cv::Mat res = cv::Mat::zeros(height, width, CV_8UC4);
								memcpy(res.data, &buf[frame_index * frame_size], sizeof(unsigned char) * frame_size);
								cv::flip(res, res, 0);
								cv::imshow("frame", res);
								cv::waitKey(0);
							}

					This kind of file-buffer is needed for high-capacity dataset creation.

3) COMMAND LINE PARAMETERS
		To launch the HandGesture library, parameters must be set before the call at the library interface is made. The parameters available are
		all referred to the structure 

			HandParameters

		Explanation:

			general_animation_on = true;		-> if the gui start with the animation on
			general_use_rules = false;			-> if rules are used in this rendering

			render_use_full_model = false;		-> are we using the full model? (high quality polygon)
			render_bone_sight = true;			-> do we render a sphere around the selected bone? (Suggestion: ON for GUI, OFF for PROCESSING)
			render_FOV = 60.0f;					-> camera FOV
			render_near = 0.01f;				-> camera near
			render_far = 20.0f;					-> camera far
			render_animationSpeed = 0.01f;		-> animation speed
			render_fbo_width = 200;				-> rendering output width, if processing mode
			render_fbo_height = 200;			-> rendering output height, if processing mode

			setup_vertexShaderPath = "data/shader.vert";			-> path to vertex shader
			setup_fragmentShaderPath = "data/shader.frag";			-> path to fragment shader
			setup_shader_modelViewProjUniformName = "model_view_proj_mat";		-> modelViewProjection uniform MUST BE PRESENT in the referred shader
			setup_shader_textureUniformName = "textureObject";		-> texture uniform MUST BE PRESENT in the referred shader
			setup_shader_boneUniformName = "bone_matrix";			-> bone uniform MUST BE PRESENT in the referred shader

			setup_model_path_full = "data/hand_model_full.dae";			-> path to the full model, needed if full_model is true
			setup_model_path_low = "data/hand_model.dae";				-> path to low qualiy model, needed if full_model is false
			setup_model_property_path = "data/hand.property";			-> path to the property file 
			setup_model_rules_path = "data/hand.property.rules";		-> path to the rules file, needed if use_rules is true
			setup_modelTexture_path = "data/hand_texture_image.bmp";	-> path to the model texture, 2048x2048 RGB BMP ONLY

			setup_output_folder_path = "data/";				-> path to output for the PROCESSING mode

