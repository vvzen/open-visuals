varying vec2 vUv;

vec2 texcoord;
vec2 varyingtexcoord;

uniform float time;
uniform float x_frequency;
uniform float y_frequency;
uniform float displacement;

float map(float value,float inputMin,float inputMax,float outputMin,float outputMax){
    
    float outVal=((value-inputMin)/(inputMax-inputMin)*(outputMax-outputMin)+outputMin);
    
    if(outputMax<outputMin){
        if(outVal<outputMax)outVal=outputMax;
        else if(outVal>outputMin)outVal=outputMin;
    }
    else{
        if(outVal>outputMax)outVal=outputMax;
        else if(outVal<outputMin)outVal=outputMin;
    }
    return outVal;
}

void main(){
    
    vUv=uv;
    
    // here we move the texture coordinates
    varyingtexcoord=vec2(texcoord.x,texcoord.y);
    
    //float x_frequency = 4.25;
    //float y_frequency = 1.5;
    
    float displacement_height=map(uv.x,0.,.5,0.,displacement);
    
    float y_offset=sin(time+position.x*y_frequency)*displacement_height;
    float z_offset=sin(time+position.x*x_frequency)*displacement_height;
    
    vec3 modified_position=position;
    modified_position.y+=y_offset;
    modified_position.z+=z_offset;
    
    gl_Position=projectionMatrix*modelViewMatrix*vec4(modified_position,1.);
    
}