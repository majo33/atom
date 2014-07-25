#include "resources.h"

#include "../video/image.h"
#include "../video/texture.h"
#include "../video/material.h"
#include "../video/bitmap_font.h"
#include "../audio/sound.h"
#include "../audio/music.h"
#include "../video/mesh.h"
#include "../video/raw_mesh.h"

namespace atom {

String make_resource_name(const String &tag, const String &name)
{
  return tag + RESOURCE_DELIMITER + name;
}

Resource::Resource()
  : my_loader(nullptr)
{
}

void Resource::depend_on_resource(const ResourcePtr &resource)
{
  my_sources.push_back(resource->name());
  my_sources.insert(my_sources.end(), resource->sources().begin(), resource->sources().end());
}

void Resource::depend_on_file(const char *filename)
{
  assert(filename != nullptr);
  my_sources.push_back(make_resource_name(RESOURCE_FILE_TAG, filename));
}

void Resource::depend_on_file(
  const String &filename)
{
  depend_on_file(filename.c_str());
}

void Resource::depend_on_files(const StringArray &files)
{
  for (const String &file : files)
    my_sources.push_back(make_resource_name(RESOURCE_FILE_TAG, file));
}

void Resource::set_loader(
  Loader *loader)
{
  my_loader = loader;
}

StringArray split_resource_name(const String &resource_name)
{
  return split_string(resource_name, ':');
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------

ImageResource::~ImageResource()
{
}

TextureResource::~TextureResource()
{
}

TechniqueResource::~TechniqueResource()
{
}

MaterialResource::~MaterialResource()
{
}

MeshResource::~MeshResource()
{
}

BitmapFontResource::~BitmapFontResource()
{
}

SoundResource::~SoundResource()
{
}

MusicResource::~MusicResource()
{
}

RawMeshResource::~RawMeshResource()
{
}

}