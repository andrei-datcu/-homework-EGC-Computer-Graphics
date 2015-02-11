#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec3 velocity;                                             
layout (location = 2) in vec3 color;    
                                                                                    
out vec3 position0;                                                                 
out vec3 velocity0;    
out vec3 color0;                                                             
                                                                                    
void main()                                                                         
{                                                                                   
    position0 = position;                                                           
    velocity0 = velocity;          
	color0 = color;                                                 
}