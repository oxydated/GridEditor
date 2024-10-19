#version 130

in  vec3 in_Position;
in  vec3 in_Color;
out vec3 ex_Color;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 World;

//    vs << "layout(location = 4) uniform vec2 screenOffset;

void main(void)
{
    ex_Color = in_Color;
   // gl_Position = View * Proj * World * vec4(in_Position, 1.0);
    gl_Position = View * Proj * World * vec4(in_Position, 1.0);
}
