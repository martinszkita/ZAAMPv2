#include "Interp4Set.hh"
#include <cmath>
#include <iostream>

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* Interp4Set::GetCmdName() const { return "Set"; }
}

AbstractInterp4Command* CreateCmd(void){
    return Interp4Set::CreateCmd();
}

AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


Interp4Set::Interp4Set(): pos(Vector3D()), rpy(Vector3D()) {}

const std::string& Interp4Set::GetRobotName() const {
    return _robotName;
}

const Vector3D& Interp4Set::GetPosition() const {
    return pos;
}

const Vector3D& Interp4Set::GetRPY() const {
    return rpy;
}

void Interp4Set::SetRobotName(const std::string& name) {
    _robotName = name;
}

void Interp4Set::SetPosition(const Vector3D& position) {
    pos = position;
}

void Interp4Set::SetRPY(const Vector3D& _RPY) {
    rpy = _RPY;
}

void Interp4Set::PrintCmd() const {
    std::cout << GetCmdName() << " " << _robotName << GetPosition() << GetRPY() << std::endl;
}

bool Interp4Set::ExecCmd( AbstractScene &rScn, const char * sMobObjName,AbstractComChannel &rComChann)  {
    AbstractMobileObj * pObj = rScn.FindMobileObj(sMobObjName);

  if (pObj == nullptr){
    std::cerr << GetCmdName() <<"  -- nie znaleziono obiektu o nazwie: "<< sMobObjName << std::endl;
    return false;
  }

  pObj->SetPosition_m(pos);
  pObj->SetRotXYZ_deg(rpy);

  return true;
}

bool Interp4Set::ReadParams(std::istream& strm) {
    strm >> _robotName >> pos[0] >> pos[1] >> pos[2] >> rpy[0] >> rpy[1] >> rpy[2];
    return !strm.fail();
}

void Interp4Set::PrintSyntax() const{
    std::cout << "Set Command Syntax:" << std::endl;
    std::cout << "Set nazwa_obiektu wsp_x wsp_y wsp_z kat_OX kat_OY kat_OZ" << std::endl;
}
void Interp4Set::PrintParams() const{
    std::cout << "Set Command Params: " << std::endl;
    std::cout << "nazwa_obiektu: " << GetRobotName() << std::endl;
    std::cout << "wsp_XYZ [m,m,m]: " << GetPosition() << std::endl;
    std::cout << "kat_RPY [deg, deg, deg]: " <<  GetRPY() << std::endl;
}


