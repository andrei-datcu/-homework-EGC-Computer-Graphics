#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 position;  
layout (location = 1) in vec3 color;             
uniform mat4 model_matrix, view_matrix;

out vec3 vColor;                           
                                                                                    
void main(){       
	vColor = color;                                                                            
    gl_Position = view_matrix * model_matrix * vec4(position, 1.0);                                              
}