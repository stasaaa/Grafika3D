#include "OBJLoader.h"

std::vector<Vertex> LoadOBJ(const char* file_name) {
	std::vector<glm::vec3> vs_positions;
	std::vector<glm::vec3> vs_normals;
	std::vector<glm::vec2> vs_texcoords;

	std::vector<GLint> vs_position_indices;
	std::vector<GLint> vs_normal_indices;;
	std::vector<GLint> vs_texcoord_indices;

	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::File nije mogao da se otvori";
	}

	while (std::getline(in_file, line))
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") {

		}
		else if (prefix == "o") {

		}
		else if (prefix == "s") {

		}
		else if (prefix == "use_mtl") {

		}
		else if (prefix == "v") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vs_positions.push_back(temp_vec3);
		}
		else if (prefix == "vn") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vs_normals.push_back(temp_vec3);
		}
		else if (prefix == "vt") {
			ss >> temp_vec2.x >> temp_vec2.y;
			vs_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "f") {
			int counter = 0;
			std::vector<GLint> temp_pos_indices;
			std::vector<GLint> temp_tex_indices;
			std::vector<GLint> temp_norm_indices;
			while (ss >> temp_glint)
			{
				if (counter == 0) {
					temp_pos_indices.push_back(temp_glint);
				}
				else if (counter == 1) {
					temp_tex_indices.push_back(temp_glint);
				}
				else if (counter == 2) {
					temp_norm_indices.push_back(temp_glint);
				}

				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				}

				if (counter > 2) {
					counter = 0;
				}
			}

			// Triangulate faces (fan method)
			for (size_t i = 1; i + 1 < temp_pos_indices.size(); ++i)
			{
				vs_position_indices.push_back(temp_pos_indices[0]);
				vs_position_indices.push_back(temp_pos_indices[i]);
				vs_position_indices.push_back(temp_pos_indices[i + 1]);

				vs_texcoord_indices.push_back(temp_tex_indices[0]);
				vs_texcoord_indices.push_back(temp_tex_indices[i]);
				vs_texcoord_indices.push_back(temp_tex_indices[i + 1]);

				vs_normal_indices.push_back(temp_norm_indices[0]);
				vs_normal_indices.push_back(temp_norm_indices[i]);
				vs_normal_indices.push_back(temp_norm_indices[i + 1]);
			}
		}
		else {

		}
	}

	vertices.resize(vs_position_indices.size(), Vertex());

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = vs_positions[vs_position_indices[i] - 1];
		vertices[i].texcoord = vs_texcoords[vs_texcoord_indices[i] - 1];
		vertices[i].normal = vs_normals[vs_normal_indices[i] - 1];
		vertices[i].color = glm::vec3(1.f);
	}

	// debug
	//std::cout << "Nr of vertices: " << vertices.size() << "\n";

	return vertices;
}