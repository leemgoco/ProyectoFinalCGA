#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>
//std includes
#include <string>
#include <iostream>
#include <math.h>

#include "Enemy.h"

float distanceToPersonaje(glm::vec3 coordsEnemigo, glm::vec3 coordsProta) {
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
	glm::vec3 direccion;
	float hip;

	direccion.x = coordsProta.x - coordsEnemigo.x;
	direccion.z = coordsProta.z - coordsEnemigo.z;

	hip = sqrt(pow(direccion.x, 2) + pow(direccion.z, 2));

	direccion.x = direccion.x / hip;
	direccion.z = direccion.z / hip;

	return direccion;
}