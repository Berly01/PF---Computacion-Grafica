#pragma once
#include <vector>
#include "Texture.hpp"
#include "Shader.hpp"
#include "utils.hpp"

class Background {

private:
	Texture texture;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::array<ut::Vertex2D, 4> vertices;
	std::array<unsigned, 6> indices;

	cv::VideoCapture cap;
	cv::Mat frame;


	int windowWidth{};
	int windowHeight{};

public:
	Background() = default;

	explicit Background(const int& width, const int& height, const int& index);

	void draw(const Shader& shader);

	void update_texture();

	GLuint get_texture_id() const { return texture.get_ID(); }

	~Background();
};


