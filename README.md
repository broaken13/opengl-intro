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
