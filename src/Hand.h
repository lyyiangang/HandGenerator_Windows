/**
*	Alvise Memo, 20/10/2015
*	Multimedia Technology and Telecommunications Laboratory, University of Padova
*	HandGenerator by Alvise Memo is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
*	Based on a work at http://lttm.dei.unipd.it/downloads/handposegenerator/index.html.
*	The 3D Hand Model derives from The LibHand 3D Hand Model by Marin Saric and is licensed under a Creative Commons Attribution 3.0.
*
*	\file Hand.h Header of the Hand object. This file requires the libraries: ASSIMP, FREEGLUT, GLEW, and the header corresponding to the HandGenerator library.
**/

#ifndef __HAND_H__
#define __HAND_H__

#include "HandStructs.h"
#include "HandGenerator.h"

#include "GL/glew.h"
#include <GL/freeglut.h>

/**
*	\brief Class for the hand object, to be used with the help of the HandGenerator class, holds most graphics requires methods
**/
class Hand
{
private:
	// Parameters for setting up the hand
	HandParameters param;

	// GL indexes for shading objects
	GLuint shader_modelViewProj_loc; // location of the modelViewProj matrix uniform
	GLuint shader_texture_loc; // location of the texture uniform
	GLuint shader_bones_loc[30]; // location of the bones uniform
	GLuint shader_id; // id of the compiled shader
	GLuint stf_fbo; // location of the FrameBufferObject containing all setup data
	GLuint stf_pboIds[2]; // location of the two PixelBufferObjects

	// Some matrix variables to store spatial and geometric transformations
	Matrix4f matrix_modelViewProj;
	Matrix4f matrix_model;
	Matrix4f matrix_projection;
	Matrix4f matrix_translation;

	float runningTime; // Time elapsed from last render call, used for animations timing

	std::string joints[19]; // Vector with the names of the 19 bones present in the hand model

public:


	/**
	*	\brief Creates the hand object by initializing the joint vector rapresenting names-jointID
	**/
	Hand(void);

	/**
	*	\brief Does nothing, TODO: erase all OpenGL created objects
	**/
	~Hand(void);

	/**
	*	\brief Initialization function, MUST be called in order to setup the hand object
	*	\param parameters Preferences specifing how the user will utilize the hand
	*	\return Bool value telling wheter or not the initialization had success
	**/
	bool Init(HandParameters parameters);

	/**
	*	\brief Rendering of the hand ONCE with the current setup
	*	\param w Integer specifing the width of the screen to render into
	*	\param h Integer specifing the height of the screen to render into
	*	\param r_x Float specifing the rotation on the x-axis
	*	\param r_y Float specifing the rotation on the y-axis
	*	\param r_z Float specifing the rotation on the z-axis
	*	\param cont_rot Boolean indicating if the rotation is addictive (to previus rotation) or not
	*	\param d Float specifing the distance of the center of the hand from the camera
	**/
	void Render(int w, int h, float r_x, float r_y, float r_z, bool cont_rot, float d);

    void DrawDepthData(int w, int h);

	/**
	*	\brief Sets the joint to the position
	*	\param jn Integer specifing wich joint to be set
	*	\param pc Integer specifing the index of the position to be set
	**/
	void SetJoint(int jn, int pc);

	/**
	*	\brief Sets the position of the joint to the rotations given
	*	\param jn Integer specifing wich joint to be set
	*	\param pc Integer specifing the index of the position to be set
	*	\param x Float the rotation around the x-axis
	*	\param y Float the rotation around the y-axis
	*	\param z Float the rotation around the z-axis
	**/
	void SetJoint(int jn, int pc, float x, float y, float z);

    void InitAllJoints();

	/**
	*	\brief Gets the rotations of the position of the joint
	*	\param jn Integer specifing wich joint to be set
	*	\param pc Integer specifing the index of the position to be set
	*	\param x Float reference of the rotation around the x-axis
	*	\param y Float reference of the rotation around the y-axis
	*	\param z Float reference of the rotation around the z-axis
	**/
	void GetJointRotation(int jn, int pc, float &x, float &y, float &z);

	/**
	*	\brief Gets the positions of the joint
	*	\param jn Integer specifing wich joint to be set
	*	\param pc Integer reference of the positions of the joint
	**/
	void GetJointPos(int jn, int &pc);

	/**
	*	\brief Gets the name of the joint
	*	\param jn Integer specifing wich joint to retrieve the name of
	*	\return String the name of the joint
	**/
	std::string GetJointName(int jn);

	/**
	*	\brief Gets the rotations of the hand
	*	\param out_rx Vector of Float reference of the rotations around the x-axis
	*	\param out_ry Vector of Float reference of the rotations around the y-axis
	*	\param out_rz Vector of Float reference of the rotations around the z-axis
	**/
	void GetHandRotations(std::vector<float> &out_rx, std::vector<float> &out_ry, std::vector<float> &out_rz);

	/**
	*	\brief Set the hand behavior from manual-transformation to animation
	*	\param isAnimation Boolean specifing wheter or not has to be set animation behavior
	**/
	void SwitchAnimationStatic(bool isAnimation);

	/**
	*	\brief Checks if the given joint-position combination is alloweb by the position rules
	*	\param combination Vector of Integer For each joint of index from 0 to combination.Size(), the values of the position to be taken
	*	\return Boolean rapresenting if the given combination is possible or not
	**/
	bool CheckForRules(std::vector<int> combination);

	/**
	*	\brief Saves the current hand properties (joint-positions)
	**/
	void SaveProperties();
};

#endif
