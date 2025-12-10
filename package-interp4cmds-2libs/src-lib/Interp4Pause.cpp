#include "Interp4Pause.hh"
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
#include "AccessControl.hh"
#include "ComChannel.hh"

extern "C"
{
    AbstractInterp4Command *CreateCmd(void);
    const char *Interp4Pause::GetCmdName() const { return "Pause"; }
}

AbstractInterp4Command *CreateCmd(void)
{
    return Interp4Pause::CreateCmd();
}

AbstractInterp4Command *Interp4Pause::CreateCmd()
{
    return new Interp4Pause();
}

Interp4Pause::Interp4Pause() : _pause_time_ms(0) {}

const std::string &Interp4Pause::GetRobotName() const
{
    return _robotName;
}

void Interp4Pause::SetPauseTime(const unsigned int &_ms)
{
    _pause_time_ms = _ms;
}

const unsigned int &Interp4Pause::GetPauseTime() const
{
    return _pause_time_ms;
}

void Interp4Pause::SetRobotName(const std::string &name)
{
    _robotName = name;
}

void Interp4Pause::PrintCmd() const
{
    std::cout << "Obecne polecenie:  " << GetCmdName() << GetPauseTime() << std::endl;
}

bool Interp4Pause::ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann)
{
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

    

    pAccess->LockAccess();
    std::cout << "Zaczynam sen " << GetPauseTime() <<" ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(GetPauseTime()));
    std::cout << "obudziłem sie! "<< std::endl;
    pAccess->MarkChange();
    pAccess->UnlockAccess();

    return true;
}

bool Interp4Pause::ReadParams(std::istream &strm)
{
    strm >> _pause_time_ms;
    return !strm.fail();
}

void Interp4Pause::PrintSyntax() const
{
    std::cout << GetCmdName() << " syntax:" << std::endl;
    std::cout << "pause pause_time_ms" << std::endl;
}
void Interp4Pause::PrintParams() const
{
    std::cout << "Pause Command Params:" << std::endl;
    std::cout << "pause_time_ms: " << GetPauseTime() << std::endl;
}
