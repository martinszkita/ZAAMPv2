#include "MobileObj.hh"
#include "Vector3D.hh"

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
    return _position;
}

void MobileObj::SetRotXYZ_deg(const Vector3D &rPos)
{
    _position = rPos;
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
    std::cout << "operator [] z klasy mobileObj " << std::endl;
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

// double& MobileObj::operator[](char axis) {
//     switch (axis) {
//         case 'X': return _roll;
//         case 'Y': return _pitch;
//         case 'Z': return _yaw;
//         default:
//             throw std::invalid_argument("Invalid axis. Use 'X', 'Y', or 'Z'.");
//     }
// }

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
    return _Cmd4StatDesc[_StateIdx];
}
