#vertex
attribute vec3 positionAttr;
attribute vec3 normalAttr;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
  gl_Position = projection * view * model * vec4(positionAttr, 1.0);
}
#blockend



#fragment
precision mediump float;
void main()
{
  gl_FragColor = vec4(1.0, 1.0, 0.0, 0.5);
}
#blockend