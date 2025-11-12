#include "ComChannel.hh"
#include <iostream>
#include <unistd.h> // Dla funkcji close()
#include "Sender.hh"
#include <sstream>

void ComChannel::SendMoveCommand(const std::string &objectName, Vector3D trans_m)
{
    int socket = GetSocket();
    ostringstream oss;
    oss << "UpdateObj Name=" << objectName
        << " Trans_m=" << trans_m << "\n";

    if (Send(socket, oss.str().c_str()) < 0)
    {
        std::cerr << "Error: Nie udalo sie wyslac na serwer!\n";
    }
    else{
        std::cout << "Wysłano polecenie move na serwer! \n";
    }
}
void ComChannel::SendRotateCommand(const std::string &objectName, char axis, double angSpeed, double angDeg)
{
    int socket = GetSocket();
    std::ostringstream oss;
    oss << "Rotate Name=" << objectName
        << " Axis=" << axis
        << " AngSpeed_deg_s=" << angSpeed
        << " Angle_deg=" << angDeg << "\n";

    if (Send(socket, oss.str().c_str()) < 0)
    {
        std::cerr << "SendRotateCommand: Error: Nie udalo sie wyslac komendy rotate na serwer!\n";
    }
    else
    {
        std::cout << "SendRotateCommand: Wysłano polecenie rotate na serwer! \n";
    }
}
// void ComChannel::SendSetCommand(const std::string &objectName, Vector3D posXYZ, Vector3D RPYdeg);
// void ComChannel::SendPauseCommand(int pause_ms);

bool ComChannel::Init(int Socket)
{
    // Sprawdzenie poprawności deskryptora
    if (Socket < 0)
    {
        std::cerr << "Błędny deskryptor połączenia: " << Socket << std::endl;
        return false;
    }

    // Zamknięcie istniejącego deskryptora, jeśli jest ustawiony
    if (socket != -1)
    {
        close(socket);
    }

    // Przypisanie nowego deskryptora
    socket = Socket;
    std::cout << "Deskryptor połączenia ustawiony na: " << socket << std::endl;
    return true;
}

int ComChannel::GetSocket() const
{
    return socket;
}

void ComChannel::LockAccess()
{
    access_mutex.lock();
}

void ComChannel::UnlockAccess()
{
    access_mutex.unlock();
}

std::mutex &ComChannel::UseGuard()
{
    return access_mutex;
}
