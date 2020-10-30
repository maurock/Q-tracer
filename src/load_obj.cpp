/*
 * tinyobjloader.cpp
 *
 *  Created on: 27 set 2020
 *      Author: mauro
 */
#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/load_obj.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

static void PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials) {
  std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
            << std::endl;

  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "# of materials : " << materials.size() << std::endl;

  for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.vertices[3 * v + 0]),
           static_cast<const double>(attrib.vertices[3 * v + 1]),
           static_cast<const double>(attrib.vertices[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
    printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.normals[3 * v + 0]),
           static_cast<const double>(attrib.normals[3 * v + 1]),
           static_cast<const double>(attrib.normals[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
    printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1]));
  }

  // For each shape
  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%ld].name = %s\n", static_cast<long>(i),
           shapes[i].name.c_str());
    printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.indices.size()));
    printf("Size of shape[%ld].lines.indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].lines.indices.size()));
    printf("Size of shape[%ld].points.indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].points.indices.size()));

    size_t index_offset = 0;

    assert(shapes[i].mesh.num_face_vertices.size() ==
           shapes[i].mesh.material_ids.size());

    assert(shapes[i].mesh.num_face_vertices.size() ==
           shapes[i].mesh.smoothing_group_ids.size());

    printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

    // For each face
    for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
      size_t fnum = shapes[i].mesh.num_face_vertices[f];

      printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
             static_cast<unsigned long>(fnum));

      // For each vertex in the face
      for (size_t v = 0; v < fnum; v++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
        printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
               static_cast<long>(v), idx.vertex_index, idx.normal_index,
               idx.texcoord_index);
      }

      printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
             shapes[i].mesh.material_ids[f]);
      printf("  face[%ld].smoothing_group_id = %d\n", static_cast<long>(f),
             shapes[i].mesh.smoothing_group_ids[f]);

      index_offset += fnum;
    }

    printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.tags.size()));
    for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
      printf("  tag[%ld] = %s ", static_cast<long>(t),
             shapes[i].mesh.tags[t].name.c_str());
      printf(" ints: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
        printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
        if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" floats: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
        printf("%f", static_cast<const double>(
                         shapes[i].mesh.tags[t].floatValues[j]));
        if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" strings: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
        printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
        if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");
      printf("\n");
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%ld].name = %s\n", static_cast<long>(i),
           materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].ambient[0]),
           static_cast<const double>(materials[i].ambient[1]),
           static_cast<const double>(materials[i].ambient[2]));
    printf("  material.Kd = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].diffuse[0]),
           static_cast<const double>(materials[i].diffuse[1]),
           static_cast<const double>(materials[i].diffuse[2]));
    printf("  material.Ks = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].specular[0]),
           static_cast<const double>(materials[i].specular[1]),
           static_cast<const double>(materials[i].specular[2]));
    printf("  material.Tr = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].transmittance[0]),
           static_cast<const double>(materials[i].transmittance[1]),
           static_cast<const double>(materials[i].transmittance[2]));
    printf("  material.Ke = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].emission[0]),
           static_cast<const double>(materials[i].emission[1]),
           static_cast<const double>(materials[i].emission[2]));
    printf("  material.Ns = %f\n",
           static_cast<const double>(materials[i].shininess));
    printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
    printf("  material.dissolve = %f\n",
           static_cast<const double>(materials[i].dissolve));
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n",
           materials[i].specular_highlight_texname.c_str());
    printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
    printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
    printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
    printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
    printf("  <<PBR>>\n");
    printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
    printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
    printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
    printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
    printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
    printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
    printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
    printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
    printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
    printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
    printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
    printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(
        materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(
        materials[i].unknown_parameter.end());

    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}

std::vector<Hitable*> load_obj(std::string &name_file, int& NUM_OBJECTS, float& area_light_room){
	std::string inputfile = "misc/obj/" + name_file;
	std::string inputfile_mat = "misc/obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool triangulate = true;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), inputfile_mat.c_str(), triangulate);

	if (!warn.empty()) {
	  std::cout << warn << std::endl;
	}

	if (!err.empty()) {
	  std::cerr << err << std::endl;
	}

	if (!ret) {
	  exit(1);
	}

	//PrintInfo(attrib, shapes, materials);

	// Define object to return
	std::vector<Hitable*> object;
	Vec vertix[3];
	std::array<float, 2> tex[3];
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
	  std::cout << "Shape size: " << shapes.size() <<  std::endl;
	  // Loop over faces(polygon)
	  size_t index_offset = 0;
	  for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {     // Number of triangles
	    int fv = shapes[s].mesh.num_face_vertices[f];                            // Always 3 because there are 3 vertices per triangle
	    // Loop over vertices in the face.
	    for (size_t v = 0; v < fv; v++) {
	      // access to vertex
	      tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
	      tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0] * 2;
	      tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1] * 2;
	      tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2] * 2;
	      tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
	      tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
	      tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
	      tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
	      tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
	      std::cout << "Vertici:    " << vx << " " << " " << vy << " " << vz << std::endl;
	      //std::cout << "Normali:    " << nx << " " << " " << ny << " " << nz << std::endl;
	      std::cout << "Texcoords:    " << tx << " " << " " << ty << std::endl;

	      // Optional: vertex colors
	       tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
	       tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
	       tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
		   std::cout << "Colors:    " << red << " " << " " << green << " " << blue << std::endl;
		   vertix[v] = Vec(vx, vy, vz);
		   tex[v] = {tx, ty};
	    }
	    object.push_back(new Triangle(vertix[0], vertix[1], vertix[2], tex[0], tex[1], tex[2], Vec(), Vec(.95,.95,.95)));
	    index_offset += fv;
	    // per-face material
	    shapes[s].mesh.material_ids[f];
	  }
	}

	Rectangle_yz* light_source;
	//light_source = new Rectangle_yz(40, 70, -10, 10, 30, Vec(12, 4, 2), Vec());      // cube
	light_source = new Rectangle_yz(3, 6, -2, 20, -4.9, Vec(1.2, 1.2, 1.2), Vec());
	area_light_room = light_source->surface();
	object.push_back(light_source);

	std::vector<Hitable*> scene_obj = {
			new Rectangle_xy(-5, 3, 0, 10, -2, Vec(),Vec(.75, .75, .75)),
			//new Rectangle_xy(-5, 3, 0, 10, 20, Vec(),Vec(.75, .75, .75)),
			new Rectangle_yz(0, 10, -2, 20, -5, Vec(), Vec(.25, .75, .25)),	// Left, green
			new Rectangle_yz(0, 10, -2, 20, 3, Vec(), Vec(.75, .25, .25)),	// Red
			new Rectangle_xz(-5, 3, -2, 20, 0, Vec(), Vec(.75, .75, .75)),			// Bottom
			new Rectangle_xz(-5, 3, -2, 20, 10, Vec(), Vec(.75, .75, .75))
	};
	for(Hitable* i : scene_obj) {
		object.push_back(i);
	}

	int x, y, comps;
	unsigned char* texture = stbi_load(".//misc//obj//Astronaut.png", &x, &y, &comps, 3);
	std::cout << "X: " << x << "   Y: " << y <<  "  Comps: " << comps << std::endl;
	int i=557, j=113;
	unsigned bytePerPixel = comps;
	const stbi_uc* pixelOffset = texture + (i + y * j) * bytePerPixel;
	stbi_uc r = pixelOffset[0];
	stbi_uc g = pixelOffset[1];
	stbi_uc b = pixelOffset[2];
    std::cout << "R: " << (int)r << "   G: " << (int)g <<  "  B: " << (int)b << std::endl;
	NUM_OBJECTS = object.size();
	return object;
}
