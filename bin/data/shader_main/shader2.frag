uniform float u_time;
uniform vec2 u_resolution;
uniform float u_brightness;
uniform vec3 u_color1;
uniform vec3 u_color2;
uniform float u_audio;

uniform float col_depth;
uniform float col_mix;

uniform float fract_num;
uniform vec3 noise1;
uniform vec3 noise2;

uniform vec2 scale;
uniform float pow;

uniform float invert;
uniform vec2 stpos;

uniform float bReflect;


#define PI 3.1415926

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float random(in float x) {
    return fract(sin(x)*1e4);
}

float random(in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(float x) {
    float i = floor(x);
    float f = fract(x);
    
    float u = f * f * (3.0 - 2.0*f);
    
    return mix(random(i), random(i + 1.0), u);
}

float orb (vec2 st, float r, float p) {
    return pow(r / length(st), p);
}

float ring (vec2 st, float r, float s, float p) {
    return pow(r / abs(s - length(st)), p);
}

float snoise(vec2 v) {
    
    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
                        // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,
                        // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,
                        // -1.0 + 2.0 * C.x
                        0.024390243902439);
    // 1.0 / 41.0
    
    // First corner (x0)
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);
    
    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y)? vec2(1.0, 0.0):vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;
    
    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
                     permute( i.y + vec3(0.0, i1.y, 1.0))
                     + i.x + vec3(0.0, i1.x, 1.0 ));
    
    vec3 m = max(0.5 - vec3(
                            dot(x0,x0),
                            dot(x1,x1),
                            dot(x2,x2)
                            ), 0.0);
    
    m = m*m ;
    m = m*m ;
    
    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)
    
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    
    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0+h*h);
    
    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * vec2(x1.x,x2.x) + h.yz * vec2(x1.y,x2.y);
    return 130.0 * dot(m, g);
}

mat2 rotate2D(float angle) {
    return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

vec3 rgb2hsb( in vec3 c ){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

vec3 colMix(vec3 col1, vec3 col2, float pct) {
    return vec3(mix(col1.r, col2.r, pct),
                mix(col1.g, col2.g, pct),
                mix(col1.b, col2.b, pct));
}


void main() {
    // 中心を原点ととして正規化
    vec2 st = (gl_FragCoord.xy*2.0 - u_resolution) / min(u_resolution.x, u_resolution.y);
    // 左下を原点として引き伸ばす場合
//    vec2 st = (gl_FragCoord.xy) / vec2(u_resolution.x, u_resolution.y);
//    st = (st+.5); // stを0.5-1.5
    
//    float angle1 = snoise(st*noise1.x+u_time*0.1)*noise1.y;
//    vec2 pos = st * rotate2D(angle1);
//
//    float angle2 = snoise((st*noise2.x)+u_time)*noise2.y + noise(u_time)*noise(pos.x*1.0);//*mouse.x;
//    pos *= rotate2D(angle2*noise2.z);
    if(bReflect == 1.0) st = -abs(st);
    
    
    

    
    vec3 color = vec3(0.0);
    st *= rotate2D(noise1.z*PI+u_time);
    
    float shape = 0.0;
    
    for (float i = 0.0; i < fract_num; i++) {
        vec2 ist = st*rotate2D(snoise(st*noise1.x+i)*(u_audio*2.0+1.0));
        for (float i = 0.0; i < 10.0; i++) {
            shape += orb((ist*scale + vec2(noise(u_time*i+PI*2.0)-0.5, noise(u_time*0.8*i+PI*2.0)-0.5)*(0.5+u_audio)*noise1.y), 0.2+i*(u_audio), pow/10.0)-0.5;
        }
    }
    shape /= 10.0;

    color += vec3(shape+snoise(st*u_color1.r),
                  shape+snoise(st*u_color1.g),
                  shape+snoise(st*u_color1.b));
    
    gl_FragColor = vec4(vec3(color), 1.0)*u_brightness;
}
