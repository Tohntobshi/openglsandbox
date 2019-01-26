#vertex
attribute vec3 positionAttr;
attribute vec3 normalAttr;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
varying vec3 fragPos;
varying vec3 norm;
void main()
{
  norm = vec3(model * vec4(normalAttr, 0.0));
  fragPos = vec3(model * vec4(positionAttr, 1.0));
  gl_Position = projection * view * model * vec4(positionAttr, 1.0);
}
#blockend



#fragment
precision mediump float;
uniform vec3 lightPos;
uniform vec3 camPos;
varying vec3 fragPos;
varying vec3 norm;
void main()
{
  vec3 vasheNorm = normalize(norm);
  vec3 lightDir = normalize(fragPos - lightPos);
  vec3 reflectionDir = reflect(lightDir, vasheNorm);
  vec3 lookDir = normalize(fragPos - camPos);
  float diffused = -0.5 * (dot(vasheNorm, lightDir) - 1.0);
  float specular = -0.5 * (dot(reflectionDir, lookDir) - 1.0);
  vec3 color = vec3(0.5, 0.5, 1.0) * 0.2 + vec3(1.0) * diffused * 0.4 + vec3(1.0) * pow(specular, 32.0) * 0.4;
  gl_FragColor = vec4(color, 1.0);
}
#blockend