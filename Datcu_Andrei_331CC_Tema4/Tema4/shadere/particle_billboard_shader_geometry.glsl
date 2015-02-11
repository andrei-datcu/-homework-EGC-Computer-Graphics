#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;                                                       
                             
							 
in vec3 vColor[];
							                                                        
uniform mat4 projection_matrix;                                                                   
uniform float gBillboardSize;                                                       
                                                                                    
out vec2 texCoord;
out vec3 fColor;                                                                
                                                                                    
void main()                                                                         
{                                                                                   
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(-pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * gBillboardSize;                              
    //right = vec3(-gBillboardSize, 0, 0);                              
    pos -= right;                                                                   
    gl_Position = projection_matrix * vec4(pos, 1.0);                                             
    texCoord = vec2(0.0, 0.0);   
	fColor = vColor[0];                                              
    EmitVertex();                                                                   
                                                                                    
    pos.y += gBillboardSize;                                                        
    gl_Position = projection_matrix * vec4(pos, 1.0);                                             
    texCoord = vec2(0.0, 1.0);                                                      
	fColor = vColor[0];
    EmitVertex();                                                                   
                                                                                    
    pos.y -= gBillboardSize;                                                        
    pos += right;                                                                   
    gl_Position = projection_matrix * vec4(pos, 1.0);                                             
    texCoord = vec2(1.0, 0.0);                                                      
	fColor = vColor[0];
    EmitVertex();                                                                   
                                                                                    
    pos.y += gBillboardSize;                                                        
    gl_Position = projection_matrix * vec4(pos, 1.0);                                             
    texCoord = vec2(1.0, 1.0);                                                      
	fColor = vColor[0];
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();
}