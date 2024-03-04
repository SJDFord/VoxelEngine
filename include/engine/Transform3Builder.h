#pragma once

#include <glad/glad.h>
#include <string>
#include <engine/Vertex.h>
#include <engine/Texture.h>
#include <vector>
#include <engine/Shader.h>
#include <engine/BasicCamera.h>
#include <engine/Transform3.h>

class Transform3Builder
{
private:
	glm::vec3 _scaleFactor = glm::vec3(1.0f);
	float _rotationAngle = glm::radians(0.0f);
	glm::vec3 _rotationAxis = glm::vec3(1.0f);
	glm::vec3 _position = glm::vec3(1.0f);



public:
    // Create a transformation builder. Defaults to not applying any transformations
    Transform3Builder();

    // Scale by a given factor in all directions. This will overwrite any other scale operations applied
    Transform3Builder& scaleBy(float scaleFactor);
    // Scale x, y, z independently by given factor. This will overwrite any other scale operations applied
    Transform3Builder& scaleBy(glm::vec3 scaleFactor);
    // Rotate around the X-axis. This will overwrite any other rotation operations applied.
    Transform3Builder& rotateAroundXAxis(float rotationAngle);
    // Rotate around the X-axis. This will overwrite any other rotation operations applied.
    Transform3Builder& rotateAroundYAxis(float rotationAngle);
    // Rotate around the X-axis. This will overwrite any other rotation operations applied.
    Transform3Builder& rotateAroundZAxis(float rotationAngle);
    // Rotate around the the given axis. This will overwrite any other rotation operations applied.
    Transform3Builder& rotateAroundAxis(float rotationAngle, glm::vec3 rotationAxis);
    // Rotate around the the given axis. This will overwrite any other rotation operations applied.
    Transform3Builder& translateTo(glm::vec3 position);

    Transform3 build();
};