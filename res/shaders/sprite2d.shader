// ────────────── Vertex Shader ──────────────
/// VERT
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in vec2 aSize;

out vec2 uv;

uniform mat3 mvp;

void main()
{
	vec2 pos = (mvp * vec3(aPos*aSize + aOffset, 1.0f)).xy;
	gl_Position = vec4(pos, 0.0f, 1.0f);
	uv = aTexcoord;
}

// ───────────── Fragment Shader ─────────────
/// FRAG
#version 330 core

in vec2 uv;

out vec4 color;

void main()
{
	color = vec4(uv.x, uv.y, 0.5f, 1.0f);
}