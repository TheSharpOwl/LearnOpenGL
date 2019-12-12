# LearnOpenGL
My codes while learning OpenGL from learnopengl.com, TheCherno on youtube and other resources


![](https://media.giphy.com/media/UVBDc7NgBRuIlZxi8K/giphy.gif)


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
        1. Size is 1,2,3,4 (for example in position, are you using 2d or 3d coordinates to make it 2 or 3)
        2. Type : the type of the attribute
        3. Normalized : is it between -1 and 1 ? (No need to normalize anything just set it to GL_FALSE and the GPU will do it for us)
        4. Stride : the offset between each vertex and the another (how much should the pointer jump to go to the next vertex?)
        5. Pointer : Where does this attribute start inside the vertex (offset inside the vertex). !! you should cast the number to (void*) !!
    Third, don't forget to call `glEnableVertexAttribArray(i)` where i is the i'th call of the function or it stands for the i'th attribute...
    
4. We should activate/**use the shader** **before** setting up the **uniforms' values** !


## References and places to look up

1. [docs.gl](http://docs.gl/)
		 