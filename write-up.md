# Shading-Initial-Prototype


## Versions

Visual Studio 2019

Windows 10


## How It Works

Open the executable file called 'Project_Template.exe' under the folder 'Build' to run the build

The program loads a pyramid model and a staff model, and generates a skybox around them.
It then calculates a Blinn-Phong shader for both using a point light(the pyramid also includes normal map calculation).
During which, textures are applied to the models then skybox. All textures are loaded into the fragment shader, then selected using texID.
Finally, a red spot light is created using the Blinn-Phong model and applied to the pyramid.


## Code Functions


Open 'Project_Template.sln' to open the solution.


[scenebasic_uniform.cpp]

initScene - Creates a view, then sets up all the shader properties, then loads and binds all of the necessary texture

update - Constantly rotates the view around the pyramid

render - Sets the parameters for the spotlight, pyramid model, staff model and skybox then renders them

setMatrices - Creates and sets ModelViewMatrix, NormalMatrix and ModelViewProjection uniforms in the shader


[basic_uniform.frag]

blinnPhongModelWithNormal - This method is used to calculate the shader for the pyramid; using the brick.jpg & dirt.png diffuse, point light and normalMap.png normal map

blinnPhongSpot - This method applies the red spotlight to the pyramid

blinnPhongModel - This is a basic version of blinnPhongModelWithNormal which doesn't use a normal map (it is used for the staff)

main - This method uses texID to apply a different texture and shading technique for each model (+skybox)


[basic_uniform.vert]

main - This method converts normal, position to eye space to calculate light, the converts tangent to eye space to calculate normal. As well as pass on parameters to fragment shader (TexCoord etc.)


## Video Walkthrough


https://youtu.be/c4_2BZjsjp4














