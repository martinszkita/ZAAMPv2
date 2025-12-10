#include "Sender.hh"
#include "MobileObj.hh"
#include <map>


using namespace std;

using ::cout;
using ::endl;


Sender::Sender(int Socket, Scene *pScn): _Socket(Socket), _pScn(pScn) {}

bool Sender::ShouldCountinueLooping() const { return _ContinueLooping; }

void Sender::CancelCountinueLooping() { _ContinueLooping = false; }

void Sender::Watching_and_Sending() {
    while (ShouldCountinueLooping()) {
      if (!_pScn->IsChanged())  { usleep(10000); continue; }
      _pScn->LockAccess();
      

      for (const auto &rPair : _pScn->GetObjects()) {
        const auto &rObj = rPair.second; // Wartość mapy (std::shared_ptr<AbstractMobileObj>)
        std::string stateDesc = rObj->GetStateDesc(); // Pobranie opisu stanu obiektu
        std::cout << stateDesc;
        Send(_Socket, stateDesc.c_str()); // Wysłanie opisu stanu na serwer
      }
      
      _pScn->CancelChange();
      _pScn->UnlockAccess();
    }
}

int Send(int Sk2Server, const char *sMesg)
{
  ssize_t  IlWyslanych;
  ssize_t  IlDoWyslania = (ssize_t) strlen(sMesg);

  while ((IlWyslanych = write(Sk2Server,sMesg,IlDoWyslania)) > 0) {
    IlDoWyslania -= IlWyslanych;
    sMesg += IlWyslanych;
  }
  if (IlWyslanych < 0) {
    cerr << "*** Blad przeslania napisu." << endl;
  }
  return 0;
}

void Fun_CommunicationThread(Sender  *pSender)
{
  pSender->Watching_and_Sending();
}

bool OpenConnection(int &rSocket)
{
  struct sockaddr_in  DaneAdSerw;

  bzero((char *)&DaneAdSerw,sizeof(DaneAdSerw));

  DaneAdSerw.sin_family = AF_INET;
  DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
  DaneAdSerw.sin_port = htons(PORT);


  rSocket = socket(AF_INET,SOCK_STREAM,0);

  if (rSocket < 0) {
     cerr << "*** Blad otwarcia gniazda." << endl;
     return false;
  }

  if (connect(rSocket,(struct sockaddr*)&DaneAdSerw,sizeof(DaneAdSerw)) < 0)
   {
     cerr << "*** Brak mozliwosci polaczenia do portu: " << PORT << endl;
     return false;
   }
  return true;
}

bool ChangeState(MobileObj *pObj, AccessControl *pAccCtrl) {
    if (!pObj || !pAccCtrl) {
        std::cerr << "Błąd: Nieprawidłowy wskaźnik na obiekt lub kontroler dostępu.\n";
        return false;
    }

    while (true) {
        pAccCtrl->LockAccess(); // Blokujemy dostęp do sceny

        if (!pObj->IncStateIndex()) { // Jeśli obiekt nie może zmienić stanu
            pAccCtrl->UnlockAccess();
            return false; // Kończymy działanie funkcji
        }

        pAccCtrl->MarkChange(); // Oznaczamy, że nastąpiła zmiana
        pAccCtrl->UnlockAccess(); // Otwieramy dostęp do sceny

        // Wysłanie aktualizacji na serwer lub symulacja opóźnienia
        // usleep(300000); // 300 ms przerwy na animację
    }

    return true;
}



