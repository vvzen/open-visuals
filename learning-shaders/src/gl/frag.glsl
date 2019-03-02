uniform sampler2D texture0;
uniform float time;
uniform float x_frequency;
uniform float y_frequency;
uniform float displacement;

varying vec2 vUv;

void main(){
    
    vec4 tex=texture2D(texture0,vUv);
    vec4 newTex=vec4(tex.r,(tex.g+tex.b)*.5,(tex.g+tex.b)*.5,1.);
    
    //gl_FragColor = vec4(vUv.x, vUv.y, 0.0, 1.0);
    gl_FragColor=newTex;
    
}