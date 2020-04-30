#include "Mesh.hpp"

own::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void own::Mesh::setupMesh()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}


void own::Mesh::Draw(Shader shader, int textureOffset) const
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i + textureOffset); // activate proper texture unit before binding
		glBindTexture(GL_TEXTURE_2D, textures.at(i).id);
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures.at(i).type;
		if (name == "diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (name == "emission")
		{
			shader.setInt(("material." + name).c_str(), i + textureOffset);
			shader.setBool("displayEmission", true);
			//glBindTexture(GL_TEXTURE_2D, textures->at(i).id);
			continue;
		}
		//std::cout << "material." + name + number << "\n";
		shader.setInt(("material." + name + number).c_str(), i + textureOffset);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}