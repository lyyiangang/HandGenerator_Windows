/**
*	Alvise Memo, 20/10/2015
*	Multimedia Technology and Telecommunications Laboratory, University of Padova
*	HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
*	The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.
*
*	\file HandGenerator.h
**/

#ifndef __HANDGENERATOR_H__
#define __HANDGENERATOR_H__

#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(_GCC)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

struct HandParameters
{
	bool general_animation_on;
	bool general_use_rules;

	bool render_use_full_model;
	bool render_bone_sight;
	float render_FOV;
	float render_near;
	float render_far;
	float render_animationSpeed;
	int render_fbo_width;
	int render_fbo_height;

	char* setup_vertexShaderPath;
	char* setup_fragmentShaderPath;
	char* setup_shader_modelViewProjUniformName;
	char* setup_shader_textureUniformName;
	char* setup_shader_boneUniformName;
	char* setup_model_path_full;
	char* setup_model_path_low;
	char* setup_modelTexture_path;
	char* setup_model_property_path;
	char* setup_model_rules_path;
	char* setup_output_folder_path;

	static HandParameters Default()
	{
		HandParameters hp;

		hp.general_animation_on = true;
		hp.general_use_rules = false;

		hp.render_use_full_model = false;
		hp.render_bone_sight = true;
		hp.render_FOV = 60.0f;
		hp.render_near = 0.01f;
		hp.render_far = 20.0f;
		hp.render_animationSpeed = 0.01f;
		hp.render_fbo_width = 200;
		hp.render_fbo_height = 200;

		hp.setup_vertexShaderPath = "data/shader.vert";
		hp.setup_fragmentShaderPath = "data/shader.frag";
		hp.setup_shader_modelViewProjUniformName = "model_view_proj_mat";
		hp.setup_shader_textureUniformName = "textureObject";
		hp.setup_shader_boneUniformName = "bone_matrix";

		hp.setup_model_path_full = "data/hand_model_full.dae";
		hp.setup_model_path_low = "data/hand_model.dae";
		hp.setup_model_property_path = "data/hand.property";
		hp.setup_model_rules_path = "data/hand.property.rules";
		hp.setup_modelTexture_path = "data/hand_texture_image.bmp";

		hp.setup_output_folder_path = "data/";

		return hp;
	}
	void CopyTo(HandParameters * dst)
	{
		dst->general_animation_on = general_animation_on;
		dst->general_use_rules = general_use_rules;

		dst->render_use_full_model = render_use_full_model;
		dst->render_bone_sight = render_bone_sight;
		dst->render_FOV = render_FOV;
		dst->render_near = render_near;
		dst->render_far = render_far;
		dst->render_animationSpeed = render_animationSpeed;
		dst->render_fbo_width = render_fbo_width;
		dst->render_fbo_height = render_fbo_height;

		dst->setup_vertexShaderPath = setup_vertexShaderPath;
		dst->setup_fragmentShaderPath = setup_fragmentShaderPath;
		dst->setup_shader_modelViewProjUniformName = setup_shader_modelViewProjUniformName;
		dst->setup_shader_textureUniformName = setup_shader_textureUniformName;
		dst->setup_shader_boneUniformName = setup_shader_boneUniformName;

		dst->setup_model_path_full = setup_model_path_full;
		dst->setup_model_path_low = setup_model_path_low;
		dst->setup_model_property_path = setup_model_property_path;
		dst->setup_model_rules_path = setup_model_rules_path;
		dst->setup_modelTexture_path = setup_modelTexture_path;

		dst->setup_output_folder_path = setup_output_folder_path;
	}
};


class HandGenerator
{
public:
	EXPORT static void Run(HandParameters parameters, bool is_gui);
};

#endif
