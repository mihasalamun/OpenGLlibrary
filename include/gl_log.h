#ifndef GL_LOG_H
#define GL_LOG_H
#include <time.h>
#include <stdarg.h>
#include <iostream>

bool restart_gl_log();

bool gl_log(const char *message, ...);

bool gl_log_err(const char *message, ...);

void glfw_error_callback(int error, const char *description);

#endif