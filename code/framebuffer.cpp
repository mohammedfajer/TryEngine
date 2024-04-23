
#include "framebuffer.h"

global_variable float g_rectangle_vertices[] = {
	1.0f, -1.0f, 1.0f, 0.0f,
 -1.0f, -1.0f, 0.0f, 0.0f,
 -1.0f,  1.0f, 0.0f, 1.0f,

  1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
 -1.0f,  1.0f, 0.0f, 1.0f
};

Framebuffer::Framebuffer(int32 width, int32 height) :
  m_program("../data/shaders/framebuffer.vs", "../data/shaders/framebuffer.fs") {

	glGenFramebuffers(1, &m_fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

	glGenRenderbuffers(1, &m_rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboID);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "framebuffer error " << fboStatus << std::endl;
	}

  setup_geometry();
  setup_shader();
}

void Framebuffer::rescale(int32 width, int32 height) {
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboID);
}

void Framebuffer::setup_geometry() {
  glGenVertexArrays(1, &m_rect_VAO);
  glGenBuffers(1, &m_rect_VBO);
  glBindVertexArray(m_rect_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_rect_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_rectangle_vertices), &g_rectangle_vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void Framebuffer::setup_shader() {
  m_program.use();
  m_program.setInt("screenTexture", 0);
}

void Framebuffer::draw() {
  m_program.use();
  glBindVertexArray(m_rect_VAO);
  // glClearColor(0,0,0, 0.0f);
  // glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
