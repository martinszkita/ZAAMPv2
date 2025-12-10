#include "MobileObj.hh"
#include "Vector3D.hh"

#include <sstream>

const Vector3D &MobileObj::GetPosition_m() const
{
    return _position;
}

void MobileObj::SetPosition_m(const Vector3D &rPos)
{
    _position = rPos;
}

const Vector3D &MobileObj::GetRotXYZ_deg() const
{
    return _RotXYZ_deg;
}

void MobileObj::SetRotXYZ_deg(const Vector3D &rRot)
{
    _RotXYZ_deg = rRot;
}

const std::string &MobileObj::GetName() const
{
    return _name;
}

void MobileObj::SetName(const char *sName)
{
    _name = std::string(sName);
}

double MobileObj::operator[](char axis) const
{
    switch (axis)
    {
    case 'X':
        return GetRotXYZ_deg()[0];
    case 'Y':
        return GetRotXYZ_deg()[1];
    case 'Z':
        return GetRotXYZ_deg()[2];
    default:
        throw std::invalid_argument("Invalid axis. Use 'X', 'Y', or 'Z'.");
    }
}



int MobileObj::GetAxisIndex(char axis)
{
    switch (axis)
    {
    case 'X':
        return 0; // Roll
    case 'Y':
        return 1; // Pitch
    case 'Z':
        return 2; // Yaw
    default:
        throw std::invalid_argument("Invalid axis. Use 'X', 'Y', or 'Z'.");
    }
}

std::string MobileObj::GetStateDesc() const
{
    if (_Cmd4StatDesc != nullptr)
    {
        return _Cmd4StatDesc[_StateIdx];
    }

    std::ostringstream oss;
    oss << "UpdateObj Name=" << _name
        << " Trans_m=(" << _position[0] << "," << _position[1] << "," << _position[2] << ")"
        << " RotXYZ_deg=(" << _RotXYZ_deg[0] << "," << _RotXYZ_deg[1] << "," << _RotXYZ_deg[2] << ")\n";
    return oss.str();
}
