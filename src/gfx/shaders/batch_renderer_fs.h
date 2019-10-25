#pragma once

static char default_fs_glsl_str[] = "" "#version 150 core\n"
"\n"
"uniform sampler2D diffuse;\n"
"\n"
"in vec2 tex;\n"
"in vec4 fragmentColor;\n"
"\n"
"out vec4 color;\n"
"\n"
"void main()\n"
"{\n"
"    color = fragmentColor * texture(diffuse, tex);\n"
"}\n"
;
