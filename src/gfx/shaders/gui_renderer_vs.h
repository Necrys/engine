#pragma once

static char gui_vs_glsl_str[] = "" "#version 150 core\n"
"\n"
"uniform mat4 projection;\n"
"\n"
"in vec2 position;\n"
"in vec2 texcoords;\n"
"in vec4 color;\n"
"\n"
"out vec2 tex;\n"
"out vec4 fragmentColor;\n"
"\n"
"void main()\n"
"{\n"
"    tex = texcoords;\n"
"    fragmentColor = color;\n"
"    gl_Position = projection * vec4(position.xy, 0, 1);\n"
"}\n"
;
