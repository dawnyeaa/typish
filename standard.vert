#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 position;
out vec4 color;

uniform mat4x4 transform;
uniform float time;

const vec4 colors[3] = vec4[3](vec4(1, 0, 0, 1),
                               vec4(0, 1, 0, 1),
                               vec4(0, 0, 1, 1));

void main() {
//  float s = sin(time);
//  float c = cos(time);
//
//  vec2 pos2 = pos.xy;
//  pos2 = mat2x2(c, -s, s, c) * pos2;

  gl_Position = transform * vec4(pos, 1);
  position = pos.xyz;
  color = vec4(col, 1);
}