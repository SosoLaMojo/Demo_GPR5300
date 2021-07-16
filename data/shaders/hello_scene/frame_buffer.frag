#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 out_tex;

uniform sampler2D screenTexture;
uniform sampler2D brightColor;

const float offset = 1.0 / 300.0;


vec3 ExtendedReinhard(vec3 color, float maxWhite)  // Reihnard
{
    vec3 numerator = color * (1.0 + (color / vec3(maxWhite * maxWhite)));
    return numerator / (1.0 + color);
}

void main()
{
	// normal colors
	vec3 color = texture(screenTexture, out_tex).rgb;
	vec3 bright_color = vec3(0);

    // inverse colors
	//FragColor = vec4(vec3(1.0 - texture(screenTexture, out_tex)), 1.0);
	
    // grey colors
	//FragColor = texture(screenTexture, out_tex);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);

    // Kernels
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

//    float kernel[9] = float[](
//        -1, -1, -1,
//        -1,  9, -1,
//        -1, -1, -1
//    );
 
    // blur
//    float kernel[9] = float[](
//    1.0 / 16, 2.0 / 16, 1.0 / 16,
//    2.0 / 16, 4.0 / 16, 2.0 / 16,
//    1.0 / 16, 2.0 / 16, 1.0 / 16 );

//    // Edges detection
////    float kernel[9] = float[](
////        1,  1, 1,
////        1, -8, 1,
////        1,  1, 1  
////    );

//    vec3 sampleTex[9];
//    for(int i = 0; i < 9; i++)
//    {
//        sampleTex[i] = vec3(texture(brightColor, out_tex.st + offsets[i]).rgb);
//    }
//    vec3 col = vec3(0.0);
//    for(int i = 0; i < 9; i++)
//    {
//        bright_color += sampleTex[i] * kernel[i];
//    }
//    
    const int meanBlurMatrixOrder = 5;
    vec2 texelSize = 1.0 / textureSize(brightColor, 0);
    vec3 blurredSamples[meanBlurMatrixOrder * meanBlurMatrixOrder];

    for(int y = 0; y < meanBlurMatrixOrder; y++)
    {
        for(int x = 0; x < meanBlurMatrixOrder; x++)
        {
            vec2 sampledCoord = out_tex + (texelSize * vec2(x - floor(meanBlurMatrixOrder * 0.5), y - floor(meanBlurMatrixOrder * 0.5)));
            blurredSamples[y * meanBlurMatrixOrder + x] = texture(brightColor, sampledCoord).rgb / (meanBlurMatrixOrder * meanBlurMatrixOrder);
        }   
    }

    for(int i = 0; i < meanBlurMatrixOrder * meanBlurMatrixOrder; i++)
    {
        bright_color += blurredSamples[i];
    }

    //FragColor = vec4(bright_color, 1.0);
    vec3 result =  bright_color + color;
    
    FragColor = vec4(ExtendedReinhard(result, 1.0), 1.0);

}