#version 420

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 MVP;

out vec2 varTexCoord;

void main(void) {
  vec4 pos = inPosition;

  varTexCoord = inTexCoord;
  //pos.x -= 2 * gl_InstanceID;

  //int e = gl_InstanceID / 256;
  //int c = gl_InstanceID % 16;
  //pos.x -= 3 * c;
  //int d = gl_InstanceID / 16;
  //if (d > 0) {
  //  pos.z -= 3 * d - e * 48;
  //}
  //if (e > 0) {
  //  pos.y += 3 * e;
  //}
  gl_Position = MVP * pos;
}