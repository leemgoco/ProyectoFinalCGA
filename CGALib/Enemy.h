#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>
//std includes
#include <string>
#include <iostream>
#include <math.h>

// contains new std::shuffle definition
#include <algorithm>
#include <random>
#include "Headers/Model.h"



class Enemy
{
	public:
		Model personajeControlado;
		Model protagonista;
		float distanceToPersonaje = 1000.0f;
		bool cambiaVelocidad = false;

		/*
			DESDE CERCA VA M�S R�PIDO Y CUANDO EST� A 
			CIERTA DISTANCIA DE LEJOS, VA UN POCO M�S LENTO
		*/

		Enemy(Model pjControlado, Model pjASeguir) {

		}


};