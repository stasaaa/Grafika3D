#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in float Height;

uniform sampler2D terrainTexture;

void main()
{
    //float h = (Height + 16)/32.0f;	// shift and scale the height into a grayscale value
    
    //vec3 darkForestGreen = vec3(0.05, 0.2, 0.05);  // very dark green
    //vec3 lightForestGreen = vec3(0.2, 0.5, 0.2);   // muted lighter green

    //vec3 color = mix(darkForestGreen, lightForestGreen, h);
    
    //FragColor = vec4(color, 1.0);


    FragColor = texture(terrainTexture, TexCoord);

    //vec4 tex = texture(terrainTexture, TexCoord);
    //FragColor = mix(vec4(color, 1.0), tex, 0.7);
}