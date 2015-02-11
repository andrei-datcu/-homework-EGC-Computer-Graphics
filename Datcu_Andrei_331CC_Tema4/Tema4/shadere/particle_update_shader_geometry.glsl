#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(points) out;                                                                 
layout(max_vertices = 11) out;                                                      
                                                                                    
in vec3 position0[];                                                                
in vec3 velocity0[]; 
in vec3 color0[];                                                               
                                                                                    
out vec3 position1;                                                                 
out vec3 velocity1;     
out vec3 color1;                                                            
                                                                                    
uniform float time; 
uniform vec3 initialpos;
uniform vec3 extremepos;
uniform float delta_time; //cate secunde au trecut de la frameul anterior                                                         
        
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}           

// Pseudo-random value in half-open range [0:1] - NU-MI APARTINE.
float random( inout float x ) {
	x = floatConstruct(hash(floatBitsToUint(x)));
	return min(1, x);
}                                                                 
                                                                                    
void main(){                                                                                   
    float randseed = time;
	vec3 dpos = vec3(0.1, 0, 0);                                  
                                                                                   
    if (position0[0] == initialpos) {   //daca e pozitia emitatorului
		//il regenerez mai intai pe el
		position1 = position0[0];
		velocity1 = velocity0[0];
		color1 = color0[0];
		EmitVertex();
		EndPrimitive();
		
		int particlesToGenerate = int(random(randseed) * 8);
		for (int i = 0; i < particlesToGenerate; ++i){                              
			position1 = position0[0] + dpos;
			//generez o noua particula sa nu fie confundata cu emitatorul                                              
			velocity1 = vec3(random(randseed) / 450, -random(randseed) / 550, 0);   
			color1 = vec3(random(randseed), random(randseed), random(randseed));                               
            EmitVertex();                                                           
            EndPrimitive();                                                         
        }                                                              
    }                                                                               
    else {//este o particula deja existenta, o modific                                                                          
        vec3 deltaP = delta_time * velocity0[0];                                 
        vec3 deltaV = vec3(delta_time) * (0.0, -9.81, 0.0); 
		
		vec3 newPos = position0[0] + deltaP;                    
                                                                                    
        if (newPos.x <= extremepos.x && newPos.y >= extremepos.y
			&& newPos.z <= extremepos.z) {                                             
	            position1 = newPos;
	            velocity1 = velocity0[0] + deltaV;   
				color1 = color0[0];                               
	            EmitVertex();                                                       
	            EndPrimitive();                                                     
	        }                                                                                                                                               
    }   
	                                                                            
}