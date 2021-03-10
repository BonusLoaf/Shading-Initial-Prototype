#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
using std::string;

#include "helper/texture.h"
#include "helper/objmesh.h"
#include <glm/glm.hpp>
#include <iostream>
using std::cerr;
using std::endl;


#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

#include "helper/glutils.h"

std::unique_ptr<ObjMesh> mesh;

using glm::vec3;

float x, z;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 100, 100) 
{
    mesh = ObjMesh::load("../Project_Template/media/bs_ears.obj", false, true);
}

void SceneBasic_Uniform::initScene()
{

    compile();

    glEnable(GL_DEPTH_TEST);


    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    
    


    prog.setUniform("lights.Position",(view * glm::vec4(x, 0.5f, 5.0f, 0.0f)));

    prog.setUniform("lights.L", vec3(0.8f));

    prog.setUniform("lights.La", vec3(0.8f));




    GLuint bricks = Texture::loadTexture("../Project_Template/media/texture/ogre_diffuse.png");

    GLuint pyBricks = Texture::loadTexture("../Project_Template/media/texture/bricks.jpg");

    GLuint normal = Texture::loadTexture("../Project_Template/media/texture/normalMap.png");



    // Load brick texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bricks);

    // Load texture file into channel 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);

    // Load texture file into channel 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pyBricks);
   


}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();

        
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    

}

void SceneBasic_Uniform::render()
{





    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



   
    prog.setUniform("py", true);
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    setMatrices();
    mesh->render();


    


    prog.setUniform("py", false);
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f));
    setMatrices();
    plane.render();
    

}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv;
    mv = view * model;
    
    

    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);

}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h,
        0.3f, 100.0f);
}
