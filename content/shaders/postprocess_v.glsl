#version 410 core

// Fullscreen triangle — no vertex buffer needed, use gl_VertexID
out vec2 vUV;

void main()
{
	// Generate a fullscreen triangle from vertex ID (0,1,2)
	vUV = vec2( (gl_VertexID << 1) & 2, gl_VertexID & 2 );
	gl_Position = vec4( vUV * 2.0 - 1.0, 0.0, 1.0 );
}
