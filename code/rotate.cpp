enum class Space { Self, World };

// Function to rotate an object's orientation
void Rotate(glm::quat& orientation, float xAngle, float yAngle, float zAngle, Space relativeTo = Space::Self) {
    // Convert the Euler angles to quaternions
    glm::quat xRot = glm::angleAxis(glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yRot = glm::angleAxis(glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat zRot = glm::angleAxis(glm::radians(zAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Combine the rotations based on the space
    glm::quat rotation;
    if (relativeTo == Space::World) {
        // Rotate relative to the world axes
        orientation = orientation * xRot;
        orientation = orientation * yRot;
        orientation = orientation * zRot;
    } else {
        // Rotate relative to the object's local axes
        orientation = xRot * orientation ;
        orientation = yRot * orientation ;
        orientation = zRot * orientation ;
    }
    orientation = glm::normalize(orientation);    
}

void Rotate(glm::quat &orientation, glm::vec3 euler_angles, Space relativeTo = Space::Self) {
	// Convert the Euler angles to quaternions
    glm::quat xRot = glm::angleAxis(glm::radians(euler_angles[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yRot = glm::angleAxis(glm::radians(euler_angles[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat zRot = glm::angleAxis(glm::radians(euler_angles[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // Combine the rotations based on the space
    glm::quat rotation;
    if (relativeTo == Space::World) {
        // Rotate relative to the world axes
        orientation = orientation * xRot;
        orientation = orientation * yRot;
        orientation = orientation * zRot;
    } else {
        // Rotate relative to the object's local axes
        orientation = xRot * orientation ;
        orientation = yRot * orientation ;
        orientation = zRot * orientation ;
    }
    orientation = glm::normalize(orientation);    
}

glm::quat Rotate_Euler(float pitch, float yaw, float roll) {
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation_matrix = rotX * rotY * rotZ;
    glm::quat result = glm::quat_cast(rotation_matrix);
    return result;
}