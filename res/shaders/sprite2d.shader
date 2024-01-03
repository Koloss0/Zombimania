// ────────────── Vertex Shader ──────────────
/// VERT
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in vec2 aSize;
layout (location = 4) in vec2 aUVOffset;
layout (location = 5) in vec2 aUVScale;

out vec2 uv;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos*aSize + aOffset, 1.0f, 1.0f);
	uv = aUV*aUVScale + aUVOffset;
}

// ───────────── Fragment Shader ─────────────
/// FRAG
#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D textureImage;

void main()
{
	color = texture(textureImage, uv);
}