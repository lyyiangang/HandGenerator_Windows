/**
*	Alvise Memo, 20/10/2015
*	Multimedia Technology and Telecommunications Laboratory, University of Padova
*	HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
*	The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.
*
*	\file HandGenerator.cpp Test class for the HandGenerator library
**/

#include <iostream>

#include "HandGenerator.h"

int main(int argc, char** argv)
{
	if (argc > 1 + 15)
	{
		std::cout << "HANDGENERATOR AUTOMATIC GENERATION" << std::endl;

		HandParameters hg(HandParameters::Default());

		//EXAMPLE OF COMMAND LINE CALL:
		//"HandGenerator_Test.exe" 1 0 200 200 0 "data/shader_depth.vert" "data/shader_depth.frag" "data/hand_model_full.dae" "data/hand_model.dae" "gesti/hand_test.property" 1 "gesti/hand_test.property.rules" "data/hand_texture_image.bmp" "gesti/" 0.001f 40.0f

		if (argv[1][0] == '0')
		{
			hg.render_use_full_model = argv[2][0] == '1';
			hg.render_bone_sight = argv[5][0] == '1';
			hg.setup_vertexShaderPath = argv[6];
			hg.setup_fragmentShaderPath = argv[7];
			hg.setup_model_path_full = argv[8];
			hg.setup_model_path_low = argv[9];
			hg.setup_model_property_path = argv[10];
			hg.general_use_rules = argv[11][0] == '1';
			hg.setup_model_rules_path = argv[12];
			hg.setup_modelTexture_path = argv[13];

			hg.render_near = (float)atof(argv[15]);
			hg.render_far = (float)atof(argv[16]);

			HandGenerator::Run(hg, true);
		}
		else if (argv[1][0] == '1')
		{
			hg.render_use_full_model = argv[2][0] == '1';
			hg.render_fbo_height = atoi(argv[3]);
			hg.render_fbo_width = atoi(argv[4]);
			hg.render_bone_sight = argv[5][0] == '1';
			hg.setup_vertexShaderPath = argv[6];
			hg.setup_fragmentShaderPath = argv[7];
			hg.setup_model_path_full = argv[8];
			hg.setup_model_path_low = argv[9];
			hg.setup_model_property_path = argv[10];
			hg.general_use_rules = argv[11][0] == '1';
			hg.setup_model_rules_path = argv[12];
			hg.setup_modelTexture_path = argv[13];

			hg.setup_output_folder_path = argv[14];

			hg.render_near = (float)atof(argv[15]);
			hg.render_far = (float)atof(argv[16]);

			HandGenerator::Run(hg, false);
		}
	}
	else
	{
		std::cout << "USER PROGRAM:\nPress 0 to start GUI mode, press 1 to start PROCESSING mode:" << std::endl;
		//char c = (char)std::cin.get();
        char c = '0';
		HandParameters hg(HandParameters::Default());

		if (c == '0')
		{
			hg.render_use_full_model = false;
			hg.render_bone_sight = true;
			hg.setup_vertexShaderPath = "data/shader.vert";
			hg.setup_fragmentShaderPath = "data/shader.frag";
			hg.setup_model_path_full = "data/hand_model_full.dae";
            //hg.setup_model_path_low = "data/hand_model_full.dae";
            hg.setup_model_path_low = "data/hand_model.dae";
			//hg.setup_model_property_path = "data/hand.property";
			hg.setup_model_property_path = "data/test/hand_test.property";
			hg.general_use_rules = false;
			hg.setup_model_rules_path = "data/hand.property.rules";
			hg.setup_modelTexture_path = "data/hand_texture_image.bmp";

			hg.render_near = 0.001f;
			hg.render_far = 40.0f;
			//hg.setup_modelTexture_path = "data/hand_texture_image_colored.bmp";
			
			HandGenerator::Run(hg, true);
		}
		else if (c == '1')
		{
			hg.render_use_full_model = false;
			hg.render_fbo_height = 200;
			hg.render_fbo_width = 200;
			hg.render_bone_sight = false;
			hg.setup_vertexShaderPath = "data/shader_depth.vert";
			hg.setup_fragmentShaderPath = "data/shader_depth.frag";
			hg.setup_model_path_full = "data/hand_model_full.dae";
			hg.setup_model_path_low = "data/hand_model.dae";
			hg.setup_model_property_path = "data/gesti/hand_test.property";
			hg.general_use_rules = true;
			hg.setup_model_rules_path = "data/gesti/hand_test.property.rules";
			hg.setup_modelTexture_path = "data/hand_texture_image.bmp";

			hg.setup_output_folder_path = "data/gesti/";

			hg.render_near = 0.001f;
			hg.render_far = 40.0f;

			HandGenerator::Run(hg, false);
		}
	}

	std::cout << "Press enter to exit";
	std::cin.ignore().get();
	return 0;
}