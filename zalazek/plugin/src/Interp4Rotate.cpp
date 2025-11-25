#include "Interp4Rotate.hh"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <thread>

#include "AccessControl.hh"
#include "ComChannel.hh"

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
        std::cerr << GetCmdName() << " -- nie znaleziono obiektu o nazwie: " << sMobObjName << std::endl;
        return false;
    }

    auto *pAccess = dynamic_cast<AccessControl *>(&rScn);
    if (pAccess == nullptr)
    {
        std::cerr << GetCmdName() << " -- scena nie udostępnia synchronizacji dostępu." << std::endl;
        return false;
    }

    ComChannel *pChannel = dynamic_cast<ComChannel *>(&rComChann);
    if (pChannel == nullptr)
    {
        std::cerr << GetCmdName() << " -- kanał komunikacyjny nie obsługuje wysyłania komend obrotu." << std::endl;
        return false;
    }

    const char axis = GetAxis();
    std::size_t axisIndex = 0;
    switch (axis)
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
        std::cerr << GetCmdName() << " -- nieprawidłowa oś obrotu: " << axis << std::endl;
        return false;
    }

    const double total_angle_deg = GetAngle();
    const double angle_abs = std::abs(total_angle_deg);
    if (angle_abs <= std::numeric_limits<double>::epsilon())
    {
        return true;
    }

    const double commanded_angular_velocity = GetAngularVelocity();
    const double angular_speed_deg_s = std::abs(commanded_angular_velocity);

    const double kStepDuration_s = 0.05; // 50 ms
    const double step_angle = angular_speed_deg_s > std::numeric_limits<double>::epsilon() ? angular_speed_deg_s * kStepDuration_s : angle_abs;
    const int steps = std::max(1, static_cast<int>(std::ceil(angle_abs / step_angle)));
    const double single_step_deg = total_angle_deg / static_cast<double>(steps);
    const double total_time_s = angular_speed_deg_s > std::numeric_limits<double>::epsilon() ? angle_abs / angular_speed_deg_s : 0.0;
    const double step_time_s = steps > 0 ? total_time_s / static_cast<double>(steps) : 0.0;

    Vector3D orientation = pObj->GetRotXYZ_deg();

    for (int i = 0; i < steps; ++i)
    {
        orientation[axisIndex] += single_step_deg;

        pAccess->LockAccess();
        pObj->SetRotXYZ_deg(orientation);
        pAccess->MarkChange();
        pAccess->UnlockAccess();

        if (!pChannel->SendRotateCommand(pObj->GetName(), axis, commanded_angular_velocity, single_step_deg)){
            std::cerr << "Interp4Rotate::ExecCmd -> SendRotateCommand error!" << std::endl;
            return false;
        }

        if (step_time_s > std::numeric_limits<double>::epsilon())
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(step_time_s));
        }
    }

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
