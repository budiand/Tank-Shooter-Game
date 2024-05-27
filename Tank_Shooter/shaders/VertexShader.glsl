#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 deform_position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float hp;

void main()
{
   vec3 modifiedPosition = v_position;

        
   float deformation = sin(hp * v_position.x) * sin(hp * v_position.y) * sin(hp * v_position.z);

        
   modifiedPosition += deformation * 0.1 * normalize(v_position);

   gl_Position = Projection * View * Model * vec4(modifiedPosition, 1.0);

}   
