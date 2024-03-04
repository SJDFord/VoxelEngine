#include <engine/Transform3Builder.h>

Transform3Builder::Transform3Builder() {

}

Transform3Builder& Transform3Builder::scaleBy(float scaleFactor) {
	_scaleFactor = glm::vec3(scaleFactor);
	return *this;
}

Transform3Builder& Transform3Builder::scaleBy(glm::vec3 scaleFactor) {
	_scaleFactor = scaleFactor;
	return *this;
}

Transform3Builder& Transform3Builder::rotateAroundXAxis(float rotationAngle) {
	_rotationAngle = rotationAngle;
	_rotationAxis = glm::vec3(1, 0, 0);
	return *this;
}


Transform3Builder& Transform3Builder::rotateAroundYAxis(float rotationAngle) {
	_rotationAngle = rotationAngle;
	_rotationAxis = glm::vec3(0, 1, 0);
	return *this;
}

Transform3Builder& Transform3Builder::rotateAroundZAxis(float rotationAngle) {
	_rotationAngle = rotationAngle;
	_rotationAxis = glm::vec3(0, 0, 1);
	return *this;
}

Transform3Builder& Transform3Builder::rotateAroundAxis(float rotationAngle, glm::vec3 rotationAxis) {
	_rotationAngle = rotationAngle;
	_rotationAxis = rotationAxis;
	return *this;
}


Transform3Builder& Transform3Builder::translateTo(glm::vec3 position) {
	_position = position;
	return *this;
}

Transform3 Transform3Builder::build() {
	Transform3 meshTransformations = { _scaleFactor, _rotationAngle, _rotationAxis, _position };
	return meshTransformations;
}
