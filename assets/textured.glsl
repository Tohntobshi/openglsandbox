#vertex
attribute vec3 positionAttr;
attribute vec3 normalAttr;
attribute vec2 texcoordAttr;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
varying vec2 texPos;
varying vec3 fragPos;
varying vec3 norm;
void main()
{
  norm = vec3(model * vec4(normalAttr, 0.0));
  fragPos = vec3(model * vec4(positionAttr, 1.0));
  texPos = texcoordAttr;
  gl_Position = projection * view * model * vec4(positionAttr, 1.0);
}
#blockend



#fragment
precision mediump float;
uniform vec3 lightPos;
uniform vec3 camPos;
varying vec3 fragPos;
varying vec3 norm;
uniform sampler2D tex;
varying vec2 texPos;
void main()
{
  vec3 vasheNorm = normalize(norm);
  vec3 lightDir = normalize(fragPos - lightPos);
  vec3 reflectionDir = reflect(lightDir, vasheNorm);
  vec3 lookDir = normalize(fragPos - camPos);
  float diffused = -0.5 * (dot(vasheNorm, lightDir) - 1.0);
  float specular = -0.5 * (dot(reflectionDir, lookDir) - 1.0);
  vec4 color = texture2D(tex, texPos) * 0.5;
  gl_FragColor = color * (diffused + pow(specular, 32.0f));
}
#blockend