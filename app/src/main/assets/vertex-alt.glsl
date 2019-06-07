precision highp int;
precision highp float;

//VERTEX ATTRIBUTES
attribute vec3 a_position;
attribute vec4 a_blendWeight;

uniform int a_blendIndex[4];
uniform vec4 u_matrixPalette[12];

void setPosition()
{
    float blendWeight = a_blendWeight[0];
    int matrixIndex = a_blendIndex[0] * 3;
    vec4 matrixPalette1 = u_matrixPalette[matrixIndex] * blendWeight;
    vec4 matrixPalette2 = u_matrixPalette[matrixIndex + 1];
    vec4 matrixPalette3 = u_matrixPalette[matrixIndex + 2];
    blendWeight = a_blendWeight[1];
    if (blendWeight > 0.0)
    {
        matrixIndex = a_blendIndex[1] * 3;
        matrixPalette1 += u_matrixPalette[matrixIndex];
        matrixPalette2 += u_matrixPalette[matrixIndex + 1];
        matrixPalette3 += u_matrixPalette[matrixIndex + 2];
        blendWeight = a_blendWeight[2];
        if (blendWeight > 0.0)
        {
            matrixIndex = a_blendIndex[2] * 3;
            matrixPalette1 += u_matrixPalette[matrixIndex];
            matrixPalette2 += u_matrixPalette[matrixIndex + 1];
            matrixPalette3 += u_matrixPalette[matrixIndex + 2];
            blendWeight = a_blendWeight[3];
            if (blendWeight > 0.0)
            {
                matrixIndex = a_blendIndex[3] * 3;
                matrixPalette1 += u_matrixPalette[matrixIndex];
                matrixPalette2 += u_matrixPalette[matrixIndex + 1];
                matrixPalette3 += u_matrixPalette[matrixIndex + 2];
            }
        }
    }
    vec4 position = vec4(a_position, 1.0);
    gl_Position = vec4(
        dot(position, matrixPalette1),
        dot(position, matrixPalette2),
        dot(position, matrixPalette3),
        position.w
    );
}

void main()
{
    setPosition();
}