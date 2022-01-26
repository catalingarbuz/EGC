#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;
in vec3 frag_color;

// Output
layout(location = 0) out vec4 out_color;

uniform vec3 object_color;
uniform float Time;
uniform float Rand;

void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(abs(frag_normal), 1);
}

