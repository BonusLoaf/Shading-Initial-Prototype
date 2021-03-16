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
using glm::vec4;
using glm::mat4;
using glm::mat3;

#include "helper/glutils.h"

std::unique_ptr<ObjMesh> pyramid;
std::unique_ptr<ObjMesh> staff;

using glm::vec3;

float x, z;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 18.0f), sky(100.0f)
{
    pyramid = ObjMesh::load("media/pyr.obj", false, true);
    staff = ObjMesh::load("media/staff.obj", true);
}

void SceneBasic_Uniform::initScene()
{

    compile();

    glEnable(GL_DEPTH_TEST);

    angle = glm::radians(90.0f);

    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    
    
    //ADD SPOTLIGHT
    prog.setUniform("Spot.L", vec3(1.0f, 0.0f, 0.3f));
    prog.setUniform("Spot.La", vec3(1.0f));
    prog.setUniform("Spot.Exponent", 50.0f);
    prog.setUniform("Spot.Cutoff", glm::radians(2.0f));

    //Add point light
    prog.setUniform("Light.Position",(view * glm::vec4(x, 5.0f, 0.0, 0.0f)));

    prog.setUniform("Light.L", vec3(0.9f));

    prog.setUniform("Light.La", vec3(0.5f, 0.2f, 0.1f));




    //Locate all textures
    GLuint skybox = Texture::loadCubeMap("media/skybox/sand", ".png");

    GLuint pyBricks = Texture::loadTexture("media/texture/bricks.jpg");

    GLuint staff = Texture::loadTexture("media/texture/red.png");

    GLuint normalMap = Texture::loadTexture("media/texture/normalMap.png");

    GLuint dirt = Texture::loadTexture("media/texture/dirt.png");



    // Load brick texture file into channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skybox);

    // Load texture file into channel 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pyBricks);
   

    // Load texture file into channel 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, staff);

    // Load texture file into channel 3
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalMap);
   
    // Load texture file into channel 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, dirt);

   


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
   
    //Set parameters for spotlight
    vec4 lightPos = vec4(15.0f, 15.0f, 0.0f, 1.0f);
    prog.setUniform("Spot.Position", vec3(view * lightPos));
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    prog.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));

    //Sets parameters for model and renders them
    prog.setUniform("texID", 1);
    prog.setUniform("Material.Kd", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Shininess", 5.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    setMatrices();
    pyramid->render();

    //Sets parameters for model and renders them
    //Also Changes texID to choose a different texture
    prog.setUniform("texID", 2);
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(4.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(25.0f, 12.0f, 25.0f));
    setMatrices();
    staff->render();


    

    //Sets parameters for skybox and renders it
   prog.setUniform("texID", 0);
    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
        0.0f));
    
    
    
    prog.use();
    model = mat4(1.0f);
    setMatrices();
    sky.render();


   
    

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
