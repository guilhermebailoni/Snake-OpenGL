#version 410

in vec4 fragColor;

out vec4 outColor;

/*void main() {
  float intensity = 0;
  if(length(gl_PointCoord - vec2(0.5)) * 2.0 > 1){
      intensity = 0;
  } else {
      intensity = 1;
  }
  outColor = fragColor;
}*/

void main(){outColor = fragColor;}