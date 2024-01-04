// ────────────── Vertex Shader ──────────────
/// VERT
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aOffset;
layout (location = 3) in vec2 aSize;
layout (location = 4) in vec2 aUVOffset;
layout (location = 5) in vec2 aUVScale;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

void main()
{
	//vec3 cameraRight_ = vec3(view[0][0], view[0][1], view[0][2]);
	//vec3 cameraUp_    = vec3(view[1][0], view[1][1], view[1][2]);

	vec3 vertPosWorldspace = aOffset;//vec3(aPos.xy*aSize + aOffset.xy, aPos.z + aOffset.z);
	vertPosWorldspace += cameraRight * aPos.x * aSize.x;
	vertPosWorldspace += cameraUp    * aPos.y * aSize.y;

	gl_Position = projection * view * model * vec4(vertPosWorldspace, 1.0f);
	texcoord = aUV*aUVScale + aUVOffset;
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