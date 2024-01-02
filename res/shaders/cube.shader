// ────────────── Vertex Shader ──────────────
/// VERT
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	texcoord = aTexcoord;
}

// ───────────── Fragment Shader ─────────────
/// FRAG
#version 330 core

in vec2 texcoord;

out vec4 color;

uniform sampler2D textureImage;

void main()
{
	color = texture(textureImage, texcoord);
}