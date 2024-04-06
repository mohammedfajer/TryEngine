#pragma once

class Framebuffer {
public:
	Framebuffer(int32 width, int32 height);
	inline GLuint get_texture_id() const { return m_textureID; }
	inline GLuint get_FBO_id() const { return m_fboID; }
	inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_fboID); }
	inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void rescale(int32 width, int32 height);

  void draw();
private:
	GLuint m_fboID = 0;
	uint32 m_textureID = 0;
	GLuint m_rboID;

	// Geometry
	uint32 m_rect_VAO, m_rect_VBO;
	void setup_geometry();

	// Shader
	Shader m_program;
	void setup_shader();
};
