#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>
#include "SOIL2/SOIL2.h"
#include "camera.h"
#include "Model.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings-----------------------------------------------------------------------------------------------------------------------------------------
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);

void myData(void);
void display(Shader, Model, Model);
void display2(Shader);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Texture
unsigned int	t_unam,
				t_mesa,
				t_patas,
				t_pizarron,
				t_texture;

//Ciclos de mesas
int i, j;

//For model
bool animacion = false;
float movKit_z = 0.0f;
//--------------------------------------------------------------------------------------------------------------------------------------------------

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------
unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
	t_unam = generateTextures("Texturas/escudo_unam.jpg", 0);
	t_patas = generateTextures("Texturas/patas.png", 1);
	t_mesa = generateTextures("Texturas/mesa.png", 1);
	t_pizarron = generateTextures("Texturas/Pizarron.png", 1);
	t_texture = generateTextures("Texturas/awesomeface.png", 1);
	

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_unam);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_patas);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_mesa);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, t_pizarron);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, t_texture);
	
}
//Termina la parte de las texturas -----------------------------------------------------------------------------------------------------------------

void myData()
{
	float vertices[] = {
		//Cubo para texturas únicas
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  //Cara Trasera 0
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  //Cara Frontal 4
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, //Cara Izquierda 8
		-0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  //Cara Derecha 12
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  //Cara Inferior 16
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  //Cara Superior 20
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
		
		//Cubo para el pizarron
		-0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  //Cara Trasera 24
		 0.5f, -0.5f, -0.5f,  1.0f,  0.5f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  //Cara Frontal 28
		 0.5f, -0.5f,  0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.5f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.5f,

		-0.5f,  0.5f,  0.5f,  0.0f,  0.5f, //Cara Izquierda 32
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f,  0.5f,  //Cara Derecha 36
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  0.5f,  //Cara Inferior 40
		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  0.5f,  //Cara Superior 44
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,


	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3, // second triangle

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
//Termina la definición de los vertices ------------------------------------------------------------------------------------------------------------

void animate(void)
{
	if (animacion)
	{
		movKit_z += 0.03f;
		if (movKit_z >= 15.0f)
			animacion = false;
	}
}
//Termina la parte de a animación ------------------------------------------------------------------------------------------------------------------

void display(Shader shader, Model modelo, Model llantas, Model piso)
{
	shader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.8f, -1.0f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	shader.setMat4("model", model);
	piso.Draw(shader);

	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(15.0f, -1.75f, movKit_z));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	modelo.Draw(shader);

	model = glm::translate(tmp, glm::vec3(0.85f, 0.25f, 1.29f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Izq delantera

	model = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, 1.29f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Der delantera

	model = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, -1.45f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Der trasera

	model = glm::translate(tmp, glm::vec3(0.85f, 0.25f, -1.45f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Izq trase
}
//Termina la parte del dibujo de modelos -----------------------------------------------------------------------------------------------------------

void display2(Shader projectionShader) {
	projectionShader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//Dibujo del salón de cómputo
	/*
	//Dibujo de la mesa
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, 0.375f, -2.5f));

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			tmp = model;
			model = glm::scale(model, glm::vec3(20.5f, 0.25f, 5.5f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_mesa);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			model = tmp;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 15.0f));
		}
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -45.0f));
	}

	//Dibujo de las patas de la mesa
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			tmp = model;
			model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_patas);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			model = tmp;

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
			tmp = model;
			model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_patas);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			model = tmp;

			model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
			tmp = model;
			model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_patas);
			projectionShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			model = tmp;

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
			tmp = model;
			model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_patas);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			model = tmp;

			model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 15.0f));
		}
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -45.0f));
	}
	//Escritorio
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, -1.0f, -10.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = tmp;

	model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	projectionShader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, 0.375f, -12.5f));
	model = glm::scale(model, glm::vec3(10.5f, 0.25f, 5.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_mesa);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Pizarrón
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-5.0f, 4.0f, -20.0f));
	model = glm::scale(model, glm::vec3(40.0f, 6.0f, 0.25f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_pizarron);
	glDrawArrays(GL_QUADS, 24, 24);	*/

	//Ediciio central
	tmp = model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(12.0f, 4.0f, 12.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_unam);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 4.0f, 12.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_mesa);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 4.0f, 12.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.0f, 4.0f, 12.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_pizarron);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Edificio Derecho
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, -8.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 38.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 38.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_pizarron);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 38.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_mesa);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 38.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_unam);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	//Edificio Izquierdo	
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-11.0f, 0.0f, 18.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 30.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_mesa);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 30.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_pizarron);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 30.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_unam);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 4.0f, 30.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
//Termina la parte del dibujado de primitivas ------------------------------------------------------------------------------------------------------

void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		animacion = true;
}
//Termina el uso de las teclas ---------------------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 10", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	Shader modelShader("Shaders/modelLoading.vs", "Shaders/modelLoading.fs");
	// Load models
	Model ourModel = ((char *)"Models/Lambo/carroseria.obj");
	Model llantasModel = ((char *)"Models/Lambo/Wheel.obj");
	Model pisoModel = ((char *)"Models/piso/piso.obj");


	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(0.0f, 0.95f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel, pisoModel);
		display2(projectionShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}