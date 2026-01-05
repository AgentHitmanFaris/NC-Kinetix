#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;

// SDF rendering parameters
uniform float smoothing = 0.25;

void main() {
    // Sample the distance field
    float distance = texture(text, TexCoords).r;
    
    // Convert distance to alpha with smoothing
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    
    // Output color with calculated alpha
    FragColor = vec4(textColor.rgb, textColor.a * alpha);
}
