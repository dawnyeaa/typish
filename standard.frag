#version 460

in vec3 position;
in vec4 color;

out vec4 fragColor;

uniform float time;

void main() {
//  float h = fract(time*0.125-position.y)*360;
//  float s = 0.8;
//  float l = 0.6;
//  float c = (1 - abs(2*l - 1)) * s;
//  float hy = h/60;
//  float x = c * (1.0 - abs(mod(hy, 2.0) - 1.0));
//  vec3 col = vec3(c, x, 0) * (step(0, hy) * step(hy, 1)) +
//             vec3(x, c, 0) * (step(1, hy) * step(hy, 2)) +
//             vec3(0, c, x) * (step(2, hy) * step(hy, 3)) +
//             vec3(0, x, c) * (step(3, hy) * step(hy, 4)) +
//             vec3(x, 0, c) * (step(4, hy) * step(hy, 5)) +
//             vec3(c, 0, x) * (step(5, hy) * step(hy, 6));
//  float m = l - (c * 0.5);
//  col += m;
  fragColor = color + time * 0.0000001;
}