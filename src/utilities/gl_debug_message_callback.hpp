//
// Created by Gianni on 19/02/2024.
//

#ifndef PROJECTILEMOTIONSIM_GL_DEBUG_MESSAGE_CALLBACK_HPP
#define PROJECTILEMOTIONSIM_GL_DEBUG_MESSAGE_CALLBACK_HPP

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <stdexcept>


void APIENTRY gl_debug_output(GLenum source,
                              GLenum type,
                              unsigned int id,
                              GLenum severity,
                              GLsizei length,
                              const char* message,
                              const void* userParam);



#endif //PROJECTILEMOTIONSIM_GL_DEBUG_MESSAGE_CALLBACK_HPP
