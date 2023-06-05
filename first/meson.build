project('first', ['c', 'cpp'],
  version : '0.1',
  default_options : ['warning_level=3'])
glfw = dependency('glfw3')
exe = executable('first', ['first.cpp', 'glad.c'],
  install : true, dependencies: glfw)

rect = executable('rectangle', ['rectangle.cpp', 'glad.c'],
  dependencies: glfw)

test('basic', exe)
