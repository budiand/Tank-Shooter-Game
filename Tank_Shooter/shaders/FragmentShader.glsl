#version 330

uniform vec3 uniform_color; // Uniform color value
uniform float hp;

layout(location = 0) out vec4 out_color;

void main()
{
    float redComponent = uniform_color.r;
    float greenComponent = uniform_color.g; 
    float blueComponent = uniform_color.b; 

    if(hp == 2) {
        redComponent = uniform_color.r;
        greenComponent = uniform_color.g* (128.0 - hp) /255.0f; 
        blueComponent = uniform_color.b; 
    }
    if(hp == 3) {
        redComponent = uniform_color.r * (128.0 - hp) /255.0f;
        greenComponent = uniform_color.g * (128.0 - hp) /255.0f; 
        blueComponent = uniform_color.b; 
    }
    if(hp == 4) {
        redComponent = uniform_color.r * (90.0 - hp) /255.0f;
        greenComponent = uniform_color.g *(90.0 - hp) /255.0f; 
        blueComponent = uniform_color.b * (90.0 - hp) /255.0f; 
    }

    
    vec3 newColor = vec3(redComponent, greenComponent, blueComponent); 
    
    out_color = vec4(newColor, 1.0); 
}
