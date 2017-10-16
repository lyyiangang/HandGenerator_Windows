/**
*	Alvise Memo, 20/10/2015
*	Multimedia Technology and Telecommunications Laboratory, University of Padova
*	HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
*	The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.
*
*	\file HandGenerator_C.h Bridge for the library, interface for matlab
**/

#ifndef __HANDGENERATOR_C_H__
#define __HANDGENERATOR_C_H__

typedef struct _C_HandParameters
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
} C_HandParameters;

#ifdef  __cplusplus
extern "C" {
#endif
	
	__declspec(dllexport) void Run(C_HandParameters param, bool is_gui);

#ifdef  __cplusplus
}
#endif


#endif //__HANDGENERATOR_C_H__