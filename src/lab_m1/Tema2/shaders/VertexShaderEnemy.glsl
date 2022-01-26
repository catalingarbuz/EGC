#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform float Rand;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

float randGen(vec3 aux)
{
    float x = 1.91139;
    float y = 3.21233;
    float z = 2.52533;
    float prod = dot(aux, vec3(x, y, z));
    float val = mod(dot(aux, vec3(x, y, z)), 3.14);
    return fract(sin(val) * x * y * z);
}
void main()
{
    frag_position = v_position;
	frag_normal = v_normal; 
	frag_texture = v_texture;
	frag_color = v_color;

    // TODO(student): Compute gl_Position
    float randVal = randGen(v_position);
    vec3 Pos = v_position + v_normal  * sin(Time * 10 + v_position.z * 2) * 1 * randVal;
	gl_Position = Projection * View * Model * vec4(Pos, 1.0);
}