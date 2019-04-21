#vertex
attribute vec3 positionAttr;
attribute vec3 normalAttr;
attribute vec2 texcoordAttr;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
varying vec2 texPos;
void main()
{
  texPos = texcoordAttr;
  gl_Position = projection * view * model * vec4(positionAttr, 1.0);
}
#blockend



#fragment
precision mediump float;
uniform sampler2D tex;
varying vec2 texPos;
void main()
{
  vec4 color = texture2D(tex, texPos);
  gl_FragColor = color;
}
#blockend