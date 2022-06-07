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
		float distanceToPersonaje = 1000.0f;
		float velocidad = 0.06f;
		glm::vec3 direccion = glm::vec3(0.0f);
		bool cambiaVelocidad = false;
		glm::mat4 matrixEnemigo = glm::mat4(1.0f);

		/*
			DESDE CERCA VA MÁS RÁPIDO Y CUANDO ESTÁ A 
			CIERTA DISTANCIA DE LEJOS, VA UN POCO MÁS LENTO
		*/

		Enemy() {

		}

		Enemy(glm::mat4 matrix) :  matrixEnemigo(matrix) {
			direccion = matrixEnemigo[3];
		}

		float distanciaAProta(glm::vec3 coordsEnemigo, glm::vec3 coordsProta) {
			float distancia = sqrt(pow(coordsProta.x - coordsEnemigo.x, 2)
				+ pow(coordsProta.y - coordsEnemigo.y, 2));
			return distancia;

		}

		bool cercaDeProta(float distancia) {

			if (distancia <= 10) {
				return true;
			}
			else {
				return false;
			}

			return false;

		}

		glm::vec3 calcularDireccionDeMovimiento(glm::vec3 coordsProta, glm::vec3 coordsEnemigo) {
			glm::vec3 direccion = glm::vec3(1.0f);
			float hip;

			direccion.x = coordsProta.x - coordsEnemigo.x;
			direccion.z = coordsProta.z - coordsEnemigo.z;

			hip = sqrt(direccion.x * direccion.x + direccion.z * direccion.z);

			direccion.x /= hip;
			direccion.z /= hip;

			return direccion;
		}

		void setMatrixEnemigo(glm::mat4 x) {
			matrixEnemigo = x;
			direccion = matrixEnemigo[3];
		}

		float productoPunto(glm::vec3 a, glm::vec3 b) {
			float res;

			res = a.x * b.x + a.z * b.z;

			return res;
		}


};