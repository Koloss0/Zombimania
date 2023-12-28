// ────────────── Vertex Shader ──────────────
/// VERT
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uv;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	uv = aUV;
}

// ───────────── Fragment Shader ─────────────
/// FRAG
#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	color = texture(screenTexture, uv);
}