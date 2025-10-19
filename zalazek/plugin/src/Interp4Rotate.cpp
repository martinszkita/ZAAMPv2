#include "Interp4Rotate.hh"
#include <thread>
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include "Sender.hh"
#include "AbstractComChannel.hh"

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* Interp4Rotate::GetCmdName() const { return "Rotate"; }
}

AbstractInterp4Command* CreateCmd(void) {
    return Interp4Rotate::CreateCmd();
}

AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}

Interp4Rotate::Interp4Rotate()
    : _robotName(""), _axis('Z'), _angularVelocity(0.0), _angle(0.0) {}

// Getters
const std::string& Interp4Rotate::GetRobotName() const {
    return _robotName;
}

char Interp4Rotate::GetAxis() const {
    return _axis;
}

double Interp4Rotate::GetAngularVelocity() const {
    return _angularVelocity;
}

double Interp4Rotate::GetAngle() const {
    return _angle;
}

// Setters
void Interp4Rotate::SetRobotName(const std::string& name) {
    _robotName = name;
}

void Interp4Rotate::SetAxis(char axis) {
    if (axis == 'X' || axis == 'Y' || axis == 'Z') {
        _axis = axis;
    } else {
        std::cerr << "Invalid axis: " << axis << ". Using default 'Z' axis." << std::endl;
        _axis = 'Z';
    }
}

void Interp4Rotate::SetAngularVelocity(double angularVelocity) {
    _angularVelocity = angularVelocity;
}

void Interp4Rotate::SetAngle(double angle) {
    _angle = angle;
}

void Interp4Rotate::PrintCmd() const {
    std::cout << GetCmdName() << " " << _robotName << " "
              << _axis << " " << _angularVelocity << " " << _angle << std::endl;
}

bool Interp4Rotate::ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann) {
    AbstractMobileObj *pObj = rScn.FindMobileObj(sMobObjName);

    if (pObj == nullptr) {
        std::cerr << GetCmdName() << " -- Object not found: " << sMobObjName << std::endl;
        return false;
    }

    double dt = 50; // Step duration in milliseconds
    double deltaAngle = GetAngularVelocity() * (dt / 1000.0); // Angular change per step
    double remainingAngle = GetAngle(); // Total angle to rotate
    Vector3D currentOrientation = pObj->GetRotXYZ_deg();
    unsigned int axisIndex;

    // Determine the axis index based on the rotation axis
    switch (GetAxis()) {
        case 'X':
            axisIndex = 0;
            break;
        case 'Y':
            axisIndex = 1;
            break;
        case 'Z':
            axisIndex = 2;
            break;
        default:
            std::cerr << "Rotation axis error!" << std::endl;
            return false;
    }


}



bool Interp4Rotate::ReadParams(std::istream& strm) {
    strm >> _robotName >> _axis >> _angularVelocity >> _angle;
    if (strm.fail() || (_axis != 'X' && _axis != 'Y' && _axis != 'Z')) {
        std::cerr << "Error reading parameters for Rotate command." << std::endl;
        return false;
    }
    return true;
}

void Interp4Rotate::PrintSyntax() const {
     std::cout <<GetCmdName() << " syntax: Rotate <object_name> <axis> <angular_velocity_deg/s> <angle_deg>" << std::endl;
}
void Interp4Rotate::PrintParams() const {
    std::cout << GetCmdName() <<" command parameters:" << std::endl;
    std::cout << "  Object Name       : " << GetRobotName() << std::endl;
    std::cout << "  Axis of Rotation  : " << GetAxis() << std::endl;
    std::cout << "  Angular Velocity  : " << GetAngularVelocity() << " deg/s" << std::endl;
    std::cout << "  Angle of Rotation : " << GetAngle() << " deg" << std::endl;
}


