#include "Interp4Rotate.hh"
#include <thread>
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include "Sender.hh"
#include "AbstractComChannel.hh"

extern "C"
{
    AbstractInterp4Command *CreateCmd(void);
    const char *Interp4Rotate::GetCmdName() const { return "Rotate"; }
}

AbstractInterp4Command *CreateCmd(void)
{
    return Interp4Rotate::CreateCmd();
}

AbstractInterp4Command *Interp4Rotate::CreateCmd()
{
    return new Interp4Rotate();
}

Interp4Rotate::Interp4Rotate()
    : _robotName(""), _axis('Z'), _angularVelocity(0.0), _angle(0.0) {}

const std::string &Interp4Rotate::GetRobotName() const
{
    return _robotName;
}

char Interp4Rotate::GetAxis() const
{
    return _axis;
}

double Interp4Rotate::GetAngularVelocity() const
{
    return _angularVelocity;
}

double Interp4Rotate::GetAngle() const
{
    return _angle;
}

void Interp4Rotate::SetRobotName(const std::string &name)
{
    _robotName = name;
}

void Interp4Rotate::SetAxis(char axis)
{
    if (axis == 'X' || axis == 'Y' || axis == 'Z')
    {
        _axis = axis;
    }
    else
    {
        std::cerr << "Invalid axis: " << axis << ". Using default 'Z' axis." << std::endl;
        _axis = 'Z';
    }
}

void Interp4Rotate::SetAngularVelocity(double angularVelocity)
{
    _angularVelocity = angularVelocity;
}

void Interp4Rotate::SetAngle(double angle)
{
    _angle = angle;
}

void Interp4Rotate::PrintCmd() const
{
    std::cout << GetCmdName() << " " << _robotName << " "
              << _axis << " " << _angularVelocity << " " << _angle << std::endl;
}

bool Interp4Rotate::ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann)
{
    AbstractMobileObj *pObj = rScn.FindMobileObj(sMobObjName);

    if (pObj == nullptr)
    {
        std::cerr << GetCmdName() << " -- Object not found: " << sMobObjName << std::endl;
        return false;
    }

    double dt = 50;                                           // Step duration in milliseconds
    double deltaAngle = GetAngularVelocity() * (dt / 1000.0); // Angular change per step
    double remainingAngle = GetAngle();                       // Total angle to rotate
    Vector3D currentOrientation = pObj->GetRotXYZ_deg();
    unsigned int axisIndex;

    // Determine the axis index based on the rotation axis
    switch (GetAxis())
    {
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

    // Perform rotation in steps
    while (std::abs(remainingAngle) > std::abs(deltaAngle / 2.0))
    {
        currentOrientation[axisIndex] += deltaAngle;
        remainingAngle -= deltaAngle;

        // Update the object's orientation in the scene
        pObj->SetRotXYZ_deg(currentOrientation);

        // Create the update command
        std::ostringstream oss;
        oss << "UpdateObj Name=" << sMobObjName
            << " RotXYZ_deg=(" << currentOrientation[0] << ", "
            << currentOrientation[1] << ", "
            << currentOrientation[2] << ")\n";

        // Send the command to the server
        // std::string command = oss.str();
        // if (!rComChann.Send(command.c_str())) {
        //     std::cerr << "Failed to send command to server: " << command << std::endl;
        //     return false;
        // }

        // // Delay for smooth animation
        // std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dt)));
    }

    // Final adjustment for the remaining angle (to avoid precision errors)
    currentOrientation[axisIndex] += remainingAngle;
    pObj->SetRotXYZ_deg(currentOrientation);

    // Send the final state update
    std::ostringstream finalOss;
    finalOss << "UpdateObj Name=" << sMobObjName
             << " RotXYZ_deg=(" << currentOrientation[0] << ", "
             << currentOrientation[1] << ", "
             << currentOrientation[2] << ")\n";

    // if (!rComChann.Send(finalOss.str().c_str())) {
    //     std::cerr << "Failed to send final command to server: " << finalOss.str() << std::endl;
    //     return false;
    // }

    return true;
}

bool Interp4Rotate::ReadParams(std::istream &strm)
{
    strm >> _robotName >> _axis >> _angularVelocity >> _angle;
    if (strm.fail() || (_axis != 'X' && _axis != 'Y' && _axis != 'Z'))
    {
        std::cerr << "Error reading parameters for Rotate command." << std::endl;
        return false;
    }
    return true;
}

void Interp4Rotate::PrintSyntax() const
{
    std::cout << GetCmdName() << " syntax: Rotate <object_name> <axis> <angular_velocity_deg/s> <angle_deg>" << std::endl;
}
void Interp4Rotate::PrintParams() const
{
    std::cout << "Rotate command parameters:" << std::endl;
    std::cout << "Object Name: " << GetRobotName() << std::endl;
    std::cout << "Axis of Rotation: " << GetAxis() << std::endl;
    std::cout << "Angular Velocity [deg/s] : " << GetAngularVelocity() << std::endl;
    std::cout << "Angle of Rotation [deg]: " << GetAngle() << std::endl;
}
