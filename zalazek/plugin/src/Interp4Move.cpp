#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <thread>
#include "AccessControl.hh"
#include "ComChannel.hh"
#include "Interp4Move.hh"

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
    std::cerr << GetCmdName() << " -- nie znaleziono obiektu o nazwie: " << sMobObjName << endl;
    return false;
  }

  auto *pAccess = dynamic_cast<AccessControl *>(&rScn);
  if (pAccess == nullptr)
  {
    std::cerr << GetCmdName() << " -- scena nie udostępnia synchronizacji dostępu." << std::endl;
    return false;
  }

  auto *pChannel = dynamic_cast<ComChannel *>(&rComChann);
  if (pChannel == nullptr)
  {
    std::cerr << GetCmdName() << " -- kanał komunikacyjny nie obsługuje wysyłania komend ruchu." << std::endl;
    return false;
  }

  const double distance_m = getDistance();
  const double distance_abs = std::abs(distance_m);
  const double speed_m_s = std::abs(getSpeed());

  if (distance_abs <= std::numeric_limits<double>::epsilon())
  {
    return true;
  }

  constexpr double kDegToRad = std::acos(-1.0) / 180.0;
  Vector3D orient_deg = pObj->GetRotXYZ_deg();
  const double pitch_rad = orient_deg[1] * kDegToRad;
  const double yaw_rad = orient_deg[2] * kDegToRad;

  const Vector3D direction({std::cos(yaw_rad) * std::cos(pitch_rad),
                            std::sin(yaw_rad) * std::cos(pitch_rad),
                            -std::sin(pitch_rad)});

  constexpr double kStepDuration_s = 0.05; // 50 ms
  const double step_length = speed_m_s > std::numeric_limits<double>::epsilon() ? speed_m_s * kStepDuration_s : distance_abs;
  const int steps = std::max(1, static_cast<int>(std::ceil(distance_abs / step_length)));
  const Vector3D single_step = direction * (distance_m / static_cast<double>(steps));
  const double total_time_s = speed_m_s > std::numeric_limits<double>::epsilon() ? distance_abs / speed_m_s : 0.0;
  const double step_time_s = steps > 0 ? total_time_s / static_cast<double>(steps) : 0.0;

  Vector3D current_pos = pObj->GetPosition_m();

  for (int i = 0; i < steps; ++i)
  {
    current_pos += single_step;

    pAccess->LockAccess();
    pObj->SetPosition_m(current_pos);
    pAccess->MarkChange();
    pAccess->UnlockAccess();

    pChannel->SendMoveCommand(pObj->GetName(), single_step);

    if (step_time_s > std::numeric_limits<double>::epsilon())
    {
      std::this_thread::sleep_for(std::chrono::duration<double>(step_time_s));
    }
  }

  return true;
}

void Interp4Move::PrintParams() const
{
  std::cout << "Move command parameters: " << endl
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
