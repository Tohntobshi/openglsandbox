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
  // suka
  vec3 vasheNorm = normalize(norm);
  vec3 lightDir = normalize(fragPos - lightPos);
  vec3 reflectionDir = reflect(lightDir, vasheNorm);
  vec3 lookDir = normalize(fragPos - camPos);
  float diffused = -0.5 * (dot(vasheNorm, lightDir) - 1.0);
  vec3 color = vec3(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, 1.0);
  gl_FragColor = vec4(color * diffused, 1.0);
}
#blockend