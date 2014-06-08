#include "technique.h"
#include "core/utils/gl_utils.h"

namespace atom {

Technique::Technique(VideoService &vs)
  : my_vs(vs)
{
  my_gl_program = glCreateProgram();
}

Technique::~Technique()
{
  glDeleteProgram(my_gl_program);
}

uptr<Technique> Technique::create(VideoService &vs, const String &name)
{
  Shader pixel_shader(vs, VideoShaderType::PIXEL);
  Shader vertex_shader(vs, VideoShaderType::VERTEX);
//  VideoShader geometry_shader(vs, VideoShaderType::GEOMETRY);

  String pixel_src, vertex_src, geometry_src;
  String pixel_src_filename = String(SHADER_RESOURCE_DIR) + "/" + name + ".ps";
  String vertex_src_filename = String(SHADER_RESOURCE_DIR) + "/" + name + ".vs";
//  String geometry_src_filename = String(SHADER_RESOURCE_DIR) + "/" + name + ".gs";

  if (!utils::load_file_into_string(vertex_src_filename, vertex_src)) {
    log::error("Can't load source file \"%s\"", vertex_src_filename.c_str());
    return nullptr;
  }

  if (!utils::load_file_into_string(pixel_src_filename, pixel_src)) {
    log::error("Can't load source file \"%s\"", pixel_src_filename.c_str());
    return nullptr;
  }

  if (!vertex_shader.compile(vertex_src)) {
    log::error("Can't compile vertex shader \"%s\"", vertex_src_filename.c_str());
    return nullptr;
  }

  if (!pixel_shader.compile(pixel_src)) {
    log::error("Can't compile pixel shader \"%s\"", pixel_src_filename.c_str());
    return nullptr;
  }

  uptr<Technique> program(new Technique(vs));
  if (!program->link(pixel_shader, vertex_shader)) {
    log::error("Can't link program \"%s\"", name.c_str());
    return nullptr;
  }

  program->locate_uniforms();
  return program;
}

bool Technique::link(const Shader &a, const Shader &b)
{
  const Shader *shaders[] = { &a, &b };
  return link(shaders, 2);
}

bool Technique::link(const Shader &a, const Shader &b, const Shader &c)
{
  const Shader *shaders[] = { &a, &b, &c };
  return link(shaders, 3);
}

bool Technique::link(const Shader *shaders[], int count)
{
  for (int i = 0; i < count; ++i) {
    if (!shaders[i]->is_compiled()) {
      return false;
    }
    glAttachShader(my_gl_program, shaders[i]->gl_shader());
  }

  glLinkProgram(my_gl_program);
  GLint status;
  glGetProgramiv(my_gl_program, GL_LINK_STATUS, &status);

  if (status == GL_FALSE) {
    const int INFO_LENGTH = 2048;
    char link_info[INFO_LENGTH];
    glGetProgramInfoLog(my_gl_program, INFO_LENGTH, nullptr, link_info);
    log::error("Can't link shader program. Link info:\n%s\n", link_info);
    return false;
  }

  return true;
}

void Technique::set_param(const char *name, const Vec3f &v) const
{
  GL_ERROR_GUARD;
  const VideoUniform *uniform = find_param(name);

  if (uniform != nullptr) {
    glUniform3f(uniform->location, v.x, v.y, v.z);
  } else {
    log::warning("Uniform not found \"%s\"", name);
  }
}

void Technique::set_param(const char *name, const Mat4f &m) const
{
  GL_ERROR_GUARD;
  const VideoUniform *uniform = find_param(name);

  if (uniform != nullptr) {
    glUniformMatrix4fv(uniform->location, 1, false, reinterpret_cast<const GLfloat *>(&m));
  } else {
    log::warning("Uniform not found \"%s\"", name);
  }
}

GLuint Technique::gl_program() const
{
  return my_gl_program;
}

void Technique::locate_uniforms()
{
  GL_ERROR_GUARD;
  my_uniforms.clear();


  GLint count = 0;
  glGetProgramiv(my_gl_program, GL_ACTIVE_UNIFORMS, &count);

  const int BUFFER_SIZE = 512;
  char name[BUFFER_SIZE];

  for (int i = 0; i < count; ++i) {
    GLsizei length = 0;
    GLint size = 0;
    GLenum type = GL_INVALID_ENUM;
    glGetActiveUniform(my_gl_program, i, BUFFER_SIZE, &length, &size, &type, name);
    // skip uniforms with long names
    if (length == 0) {
      log::warning("Too long uniform name, skipping");
      continue;
    }

    GLint location = glGetUniformLocation(my_gl_program, name);

    if (location < 0) {
      log::warning("Can't find uniform location");
      continue;
    }

    Type uniform_type = get_type_from_gl_type(type);

    if (uniform_type == Type::UNKNOWN) {
      log::warning("Unknown uniform type \"%s\" (%i)", name, type);
      continue;
    }

    my_uniforms.push_back(VideoUniform{ name, uniform_type, location });
  }
}

Type Technique::get_type_from_gl_type(GLenum type)
{
  switch (type) {
    case GL_FLOAT:
      return Type::FLOAT;

    case GL_FLOAT_VEC2:
      return Type::VEC2F;

    case GL_FLOAT_VEC3:
      return Type::VEC3F;

    case GL_FLOAT_VEC4:
      return Type::VEC4F;

    case GL_FLOAT_MAT2:
      return Type::MAT2F;

    case GL_FLOAT_MAT3:
      return Type::MAT3F;

    case GL_FLOAT_MAT4:
      return Type::MAT4F;

    case GL_SAMPLER_2D:
      return Type::SAMPLER_2D;

    default:
      return Type::UNKNOWN;
  }
}

const VideoUniform *Technique::find_param(const char *name) const
{
  assert(name != nullptr);

  for (const VideoUniform &uniform : my_uniforms) {
    if (!strcmp(name, uniform.name.c_str())) {
      return &uniform;
    }
  }

  return nullptr;
}

}