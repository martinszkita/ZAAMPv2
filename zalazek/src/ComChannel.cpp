#include "ComChannel.hh"
#include <iostream>
#include <unistd.h>  // Dla funkcji close()

void ComChannel::SendMoveCommand(const std::string& objectName, double speed, double distance) {
        std::cout << "Wysyłanie komendy ruchu dla " << objectName
                  << " z prędkością " << speed << " i odległością " << distance << std::endl;
}

bool ComChannel::Init(int Socket){
     // Sprawdzenie poprawności deskryptora
    if (Socket < 0) {
        std::cerr << "Błędny deskryptor połączenia: " << Socket << std::endl;
        return false;
    }

    // Zamknięcie istniejącego deskryptora, jeśli jest ustawiony
    if (socket != -1) {
        close(socket);
    }

    // Przypisanie nowego deskryptora
    socket = Socket;
    std::cout << "Deskryptor połączenia ustawiony na: " << socket << std::endl;
    return true;
}

int ComChannel::GetSocket() const{
    return socket;
}

void ComChannel::LockAccess(){
    access_mutex.lock();
}

void ComChannel::UnlockAccess(){
    access_mutex.unlock();
}

std::mutex & ComChannel::UseGuard() {
    return access_mutex;
}