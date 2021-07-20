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

    vec3 result =  bright_color + color;
    
    FragColor = vec4(ExtendedReinhard(result, 1.0), 1.0); //tone mapping

}