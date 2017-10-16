/**
*	Alvise Memo, 20/10/2015
*	Multimedia Technology and Telecommunications Laboratory, University of Padova
*	HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
*	The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.
*
*	\file HandGenerator_C.c Bridge for the library, interface for matlab
**/

#include "HandGenerator_C.h"
#include "HandGenerator.h"

void Run(C_HandParameters param, bool is_gui)
{
	HandParameters hp;
	hp.general_animation_on = param.general_animation_on;
	hp.general_use_rules = param.general_use_rules;

	hp.render_use_full_model = param.render_use_full_model;
	hp.render_bone_sight = param.render_bone_sight;
	hp.render_FOV = param.render_FOV;
	hp.render_near = param.render_near;
	hp.render_far = param.render_far;
	hp.render_animationSpeed = param.render_animationSpeed;
	hp.render_fbo_width = param.render_fbo_width;
	hp.render_fbo_height = param.render_fbo_height;

	hp.setup_vertexShaderPath = param.setup_vertexShaderPath;
	hp.setup_fragmentShaderPath = param.setup_fragmentShaderPath;
	hp.setup_shader_modelViewProjUniformName = param.setup_shader_modelViewProjUniformName;
	hp.setup_shader_textureUniformName = param.setup_shader_textureUniformName;
	hp.setup_shader_boneUniformName = param.setup_shader_boneUniformName;

	hp.setup_model_path_full = param.setup_model_path_full;
	hp.setup_model_path_low = param.setup_model_path_low;
	hp.setup_model_property_path = param.setup_model_property_path;
	hp.setup_model_rules_path = param.setup_model_rules_path;
	hp.setup_modelTexture_path = param.setup_modelTexture_path;

	HandGenerator::Run(hp, is_gui);
}