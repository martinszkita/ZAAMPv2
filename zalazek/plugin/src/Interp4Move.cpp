#include <iostream>
#include "Interp4Move.hh"

#define DEG_TO_RAD(deg) 180 / M_PI

using std::cout;
using std::endl;

extern "C"
{
  AbstractInterp4Command *CreateCmd(void);
  const char *GetCmdName() { return "Move"; }
}

AbstractInterp4Command *CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}

Interp4Move::Interp4Move() : _Speed_mS(0), _Distance_m(0)
{
}

void Interp4Move::setRobotName(std::string _name)
{
  _Robot_name = _name;
}

std::string Interp4Move::getRobotName() const
{
  return _Robot_name;
}

void Interp4Move::setSpeed(const double _speed)
{
  _Speed_mS = _speed;
}
void Interp4Move::setDistance(const double _distance)
{
  _Distance_m = _distance;
}
const double &Interp4Move::getSpeed() const
{
  return _Speed_mS;
}
const double &Interp4Move::getDistance() const
{
  return _Distance_m;
}

void Interp4Move::PrintCmd() const
{
  cout << GetCmdName() << " " << getRobotName() << " " << _Speed_mS << " " << _Distance_m << " " << endl;
}

const char *Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}

bool Interp4Move::ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann)
{
  AbstractMobileObj *pObj = rScn.FindMobileObj(sMobObjName);

  if (pObj == nullptr)
  {
    std::cerr << GetCmdName() << "nie znaleziono obiektu o nazwie: " << sMobObjName << endl;
    return false;
  }

  // przeliczenie stopni na radiany
  double roll_rad = DEG_TO_RAD(GetAng_Roll_deg());
  double pitch_rad = DEG_TO_RAD(GetAng_Pitch_deg());
  double yaw_rad = DEG_TO_RAD(GetAng_Yaw_deg());

  double distance_m = getDistance();
  double speed_m_s = getSpeed();

  // wektor przesunięcia, zakładamy oś X to prosto
  Vector3D VRot = Vector3D({cos(yaw_rad) * cos(pitch_rad),
                            sin(yaw_rad) * cos(pitch_rad),
                            -sin(pitch_rad)});

  // obliczenie nowej pozycji
  Vector3D VTrans = VRot * distance_m;
  Vector3D VCurrPos = pObj->GetPosition_m();
  VCurrPos += VTrans;

  pObj->SetPosition_m(VCurrPos);

  return true;
}

void Interp4Move::PrintParams() const
{
  std::cout << "Move command parameters: " <<  endl
            << "robot name: " << getRobotName() << endl
            << "speed [m/s]" << getSpeed() << endl
            << "distance [m]: " << getDistance() << endl;
}

bool Interp4Move::ReadParams(std::istream &Strm_CmdsList)
{
  Strm_CmdsList >> _Robot_name >> _Speed_mS >> _Distance_m;

  if (!Strm_CmdsList)
  {
    std::cerr << "Błąd wczytywania danych!" << std::endl;
    return false;
  }

  return true;
}

AbstractInterp4Command *Interp4Move::CreateCmd()
{
  return new Interp4Move();
}

void Interp4Move::PrintSyntax() const
{
  cout << "Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
