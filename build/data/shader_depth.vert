#version 330

layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in ivec4 BoneIDs_1;
layout (location = 3) in vec4 Weights_1;
layout (location = 4) in ivec4 BoneIDs_2;
layout (location = 5) in vec4 Weights_2;
layout (location = 6) in ivec4 BoneIDs_3;
layout (location = 7) in vec4 Weights_3;

out vec2 tex_coord;
out highp float depth_val;

uniform mat4 bone_matrix[30];
uniform mat4 model_view_proj_mat;

void main(void)
{   
	mat4 boneTransform;
	
	boneTransform = bone_matrix[BoneIDs_1[0]] * Weights_1[0];
	for (int i = 1; i < 4; i++)
		boneTransform += bone_matrix[BoneIDs_1[i]] * Weights_1[i];
	for (int i = 0; i < 4; i++)
		boneTransform += bone_matrix[BoneIDs_2[i]] * Weights_2[i];
	for (int i = 0; i < 4; i++)
		boneTransform += bone_matrix[BoneIDs_3[i]] * Weights_3[i];

	gl_Position = transpose(model_view_proj_mat) * boneTransform * vec4(Position, 1.0);
	tex_coord = TexCoord;
	
	depth_val = distance(gl_Position, vec4(0.0, 0.0, 0.0, 1.0));
}