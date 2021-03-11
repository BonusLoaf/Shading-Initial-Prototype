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

std::unique_ptr<ObjMesh> pyramid;
std::unique_ptr<ObjMesh> staff;

using glm::vec3;

float x, z;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 18.0f), sky(100.0f)
{
    pyramid = ObjMesh::load("../Project_Template/media/pyramid-simple-design.obj", true);
    staff = ObjMesh::load("../Project_Template/media/staff.obj", true);
}

void SceneBasic_Uniform::initScene()
{

    compile();

    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);
    angle = glm::radians(90.0f);

    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    
    


    prog.setUniform("lights.Position",(view * glm::vec4(x, 0.5f, 5.0f, 0.0f)));

    prog.setUniform("lights.L", vec3(0.8f));

    prog.setUniform("lights.La", vec3(1.0f,0.8f,0.6f));




    GLuint skybox = Texture::loadCubeMap("../Project_Template/media/skybox/sand", ".png");

    GLuint pyBricks = Texture::loadTexture("../Project_Template/media/texture/bricks.jpg");

    GLuint staff = Texture::loadTexture("../Project_Template/media/texture/red.png");


    // Load brick texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skybox);

    // Load texture file into channel 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pyBricks);

    // Load texture file into channel 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, staff);

   


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
    
    float deltaT = t - tPrev;
    if (tPrev == 0.0f)
        deltaT = 0.0f;
    tPrev = t;
    angle += rotSpeed * deltaT;
    if (angle > glm::two_pi<float>())
        angle -= glm::two_pi<float>();


}

void SceneBasic_Uniform::render()
{





    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



   
    prog.setUniform("texID", 1);
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.7f, 0.5f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 20.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.2f));
    setMatrices();
    pyramid->render();


    prog.setUniform("texID", 2);
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    setMatrices();
    staff->render();



    prog.setUniform("texID", 0);
    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
        0.0f));
    // Draw sky
    prog.use();
    model = mat4(1.0f);
    setMatrices();
    sky.render();


    /*prog.setUniform("py", false);
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f));
    setMatrices();
    sky.render();*/
    

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
