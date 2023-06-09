# Learning OpenGL
[Learning OpenGL](https://learnopengl.com)

## GLFW
Attempted to install from source. It had issues, so, uh, we're using the distro provided version.

# Graphics Pipeline

The basic graphics pipeline looks like:

Vertex Shader -> Shape Assembly -> Geometry Shader --
-> Rasterization -> Fragment Shader -> Tests and Blending

Open GL has a default Geometry shader, but not for Vertex or Fragment
The Geometry shader has the ability to modify vertices from the Vertex shader

## Vertex Shaders
These are required. We can pass these as an array, with every 3 values representing the XYZ values of a single coordinate.

The values or normalized for the screen port with all values falling between -1.0 and 1.0 being within the screen,
and all that are outside will not be rendered.

We are mainly looking to set `gl_Position`

## Fragment Shaders
Also required, these determine the color of a vertex using red, green, blue, and alpha (opacity).

Here we are looking to set `FragColor`

## Shader variables
We have access to basic types in shaders (bool, int, float, etc.)

We also have vectors for these types:
vec_n_ = vector of _n_ floats ex: vec3
bvec_n_ = vector of _n_ booleans
etc.

Note, n goes up to 4.

### Swizzling
Vectors by can accessed like:
```
vec3 SomeVec = vec3(0.5, 0.7, 0.1);
vec4 AnotherVec = vec4(Somevec.yzxx);
vec4 YetAnother = vec4(someVec, 1.0);
```
