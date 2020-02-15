# LearnOpenGL

My codes while learning OpenGL from learnopengl.com, TheCherno on youtube and other resources

[![Crisis NanoSuit](http://img.youtube.com/vi/N2o2K2RJj7g/0.jpg)](https://www.youtube.com/watch?v=N2o2K2RJj7g "My Assimp Import Library ShowCase")

![](https://media.giphy.com/media/UVBDc7NgBRuIlZxi8K/giphy.gif)

# Notes for other compiler versions

Assimp might not work here because I built it with VS 2019 (check how to build assimp in the FAQs)

## Important and Frequently Asked Questions for OpenGL

1. [Use of Vertex Array Objects, Vertex Buffer Objects and Differences Between Them](https://stackoverflow.com/questions/23314787/use-of-vertex-array-objects-and-vertex-buffer-objects)


2. **Stride**: The size of the vertex attributes' in bytes. It's necessary for OpenGL to know the offset when it wants to index vertex i.

3. ```
	void glVertexAttribPointer(GLuint index,

	GLint size,

	GLenum type,

	GLboolean normalized,

	GLsizei stride,

	const GLvoid * pointer);
	```
    This function is confusing a little bit... :sweat:
    - First of all, this function is called for each vertex attribute (which means if I have poistion and a texture I should call it twice)
    - Second, Parameters:
        1. Index : what's the index of the vertex attribute ? if it's i, you have to write in vertex shader : 
            `layout (location = i) in vecj someName;` where j is the size of the vector (same as Size parameter).
            Also, take a look at the Third part of this function explaination.
        2. Size is 1,2,3,4 (for example in position, are you using 2d or 3d coordinates to make it 2 or 3)
        3. Type : the type of the attribute
        4. Normalized : is it between -1 and 1 ? (No need to normalize anything just set it to GL_FALSE and the GPU will do it        for us)
        5. Stride : the offset between each vertex and the another (how much should the pointer jump to go to the next vertex?)
        6. Pointer : Where does this attribute start inside the vertex (offset inside the vertex). !! you should cast the number      to (void*) !!
    - Third, don't forget to call `glEnableVertexAttribArray(i)` where i is the i'th attribute (in the index parameter).
    
4. We should activate/**use the shader** **before** setting up the **uniforms' values** !

5. [How vertex and fragment shaders communicate in OpenGL? Since we have in a triangle (for example) three vertices and much more fragments, how does each fragment deal with the info it gets from each vertex differently?](https://stackoverflow.com/questions/28333019/how-vertex-and-fragment-shaders-communicate-in-opengl)

6. **Gouraud vs Phong models** :

    When the Phong lighting model is implemented in the vertex shader it is called Gouraud shading instead of Phong shading. Note that due to the interpolation the lighting looks a bit off. The Phong shading gives much smoother lighting results. 

7. In view Space the viewer is always at position (0, 0, 0).

8. When you get the location of the uniform in the cpp, you get -1 if it did't find that uniform **or if it's declared in the shader but not used!**

9. [glfwSwapInterval](https://www.glfw.org/docs/3.1/window.html#buffer_swap) : which makes the movement smoother because it enables [V-Sync](https://www.digitaltrends.com/computing/what-is-vsync/). For additional info check [this question](https://discourse.glfw.org/t/newbie-questions-trying-to-understand-glfwswapinterval/1287)

10. **Components of Phong shading:**

![](https://imgur.com/z95AgKq.png)

11. [Sampler2D](https://stackoverflow.com/questions/10868958/what-does-sampler2d-store) (which is used to store images for textures) in OpenGL is an [**Opaque Type**](https://stackoverflow.com/questions/2301454/what-defines-an-opaque-type-in-c-and-when-are-they-necessary-and-or-useful) which is why you can only use it as a uniform in the shaders.

12. **Attenuation** : Reducing the intensity of light, over the distance a light ray travels.

13. [How does glDrawArrays know what to draw?](https://stackoverflow.com/questions/19102180/how-does-gldrawarrays-know-what-to-draw)

15. [Adding Assimp to VS (building from source to get .lib and .dll files)](https://www.youtube.com/watch?v=W_Ey_YPUjMk)

16. **offsetof(s,m)** : Preprocessor directive that takes as its first argument a struct and as its second argument a variable                              name of the struct. The macro returns the byte offset of that variable from the start of the struct.

17. **Depth Testing**: When depth testing is enabled OpenGL tests the depth value of a fragment against the content of the depth buffer. OpenGL performs a depth test and if this test passes, the depth buffer is updated with the new depth value. If the depth test fails, the fragment is discarded. The screen space coordinates relate directly to the viewport defined by OpenGL's glViewport function and can be accessed via GLSL's built-in gl_FragCoord variable in the fragment shader. The x and y components of gl_FragCoord represent the fragment's screen-space coordinates (with (0,0) being the bottom-left corner). The gl_FragCoord also contains a z-component which contains the actual depth value of the fragment. This z value is the value that is compared to the depth buffer's content. 

18. **Z-fighting** : Z-fighting is a common problem with depth buffers and is generally stronger when objects are at a further distance (because the depth buffer has less precision at larger z-values). Z-fighting can't be completely prevented, but there are generally a few tricks that will help to mitigate or completely prevent z-fighting.

[Z-fighting example between the floor and the box base part](https://i.imgur.com/oYAkgTU.png)

## Issues I had :

1. Importing Assimp was not compiling at first then I got linking errors (At the end I got .dll not found then solved also)
    **Solution:**
    - First, when you **cmake** the Assimp like the video in 15, it might do it for x64 config only so to force it on doing it for x86 [read here] (https://stackoverflow.com/questions/28350214/how-to-build-x86-and-or-x64-on-windows-from-command-line-with-cmake)

    - Second, After doing the build step through VS, copying files to the dictionary and configrating include and linker setting,  it will complain about not finding **config.h** which you should copy from the include folder **but in the projext you built to make the libray files** to the include dictionary with its other friends :P

    - Third, Now it will compile (without linking errors because you built the library for 86x supposing that your project was x86)
    but it will give an error after compiling and running which says that it couldn't find the .dll and the solution is copying the .dll to the .exe dictionary (which is in projectFolder\Debug) and now it should work (hope it helps)


## References and useful resources

1. [docs.gl](http://docs.gl/)

2. [Light House 3D](http://www.lighthouse3d.com/)

3. [Real World Material Table](http://devernay.free.fr/cours/opengl/materials.html)

    **Note that the table's ambient values are not the same as the diffuse values; they didn't take light intensities into account.    To correctly set their values you'd have to set all the light intensities to vec3(1.0) to get the same output...**

    ![](https://imgur.com/adMlLCr.png)




