// #include <iostream>
// #include <iomanip>
// #include <cstring>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <thread>
// #include <mutex>
// #include <vector>
// #include "AccessControl.hh"
// #include "Port.hh"


// using namespace std;



// /*!
//  * \brief Wysyła napis do poprzez gniazdo sieciowe.
//  *
//  * Wysyła napis do poprzez gniazdo sieciowe.
//  * \param[in] Sk2Server - deskryptor gniazda sieciowego, poprzez które 
//  *                        ma zostać wysłany napis w sensie języka C.
//  * \param[in] sMesg - zawiera napis, który ma zostać wysłany poprzez
//  *                    gniazdo sieciowe.
//  */
// int Send(int Sk2Server, const char *sMesg)
// {
//   ssize_t  IlWyslanych;
//   ssize_t  IlDoWyslania = (ssize_t) strlen(sMesg);

//   while ((IlWyslanych = write(Sk2Server,sMesg,IlDoWyslania)) > 0) {
//     IlDoWyslania -= IlWyslanych;
//     sMesg += IlWyslanych;
//   }
//   if (IlWyslanych < 0) {
//     cerr << "*** Blad przeslania napisu." << endl;
//   }
//   return 0;
// }






// /*!
//  * \brief Ilość stanów każdego z obiektów.
//  *
//  * Ilość stanów każdego z obiektów.
//  * Podział na kolejne stany obiektów wykorzystywany jest
//  * do animacji ruchu obiektów.
//  */
// #define STATES_NUMBER   8



// /*!
//  * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 1
//  * i obrazujących jego ruch.
//  */
// const char* Cmds4Obj1[STATES_NUMBER] = {
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,23)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,26)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,29)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,32)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,35)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,38)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,41)\n",
//   "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,44)\n"  
//  };


// /*!
//  * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 2
//  * i obrazujących jego ruch.
//  */
// const char* Cmds4Obj2[STATES_NUMBER] = {
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-48,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-45,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-42,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-39,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-36,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-33,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-30,0)\n",
//   "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-27,0)\n"
//  };

// /*!
//  * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 3
//  * i obrazujących jego ruch.
//  */
// const char* Cmds4Obj3[STATES_NUMBER] = {
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-48,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-51,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-54,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-57,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-60,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-63,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-66,0)\n",
//   "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-69,0)\n"
//  };






// /*!
//  * \brief Ta klasa trochę udaję klasę MobileObj, która jest w zadaniu.
//  *
//  */
// class GeomObject {
//   /*!
//    * \brief Identyfikuje aktualny stan obiektu.
//    *
//    * Identyfikuje aktualny stan obiektu. Indeks ten pozwala
//    * na wybór z tablicy \link GeomObject::_Cmd4StatDesc _Cmd4StatDesc\endlink
//    * odpowiedniego polecenia dla serwera, które reprezentuje aktualne
//    * położenie i orientację obiektu.
//    */
//    int _StateIdx = 0;
//   /*!
//    * \brief Zestaw sekwencji poleceń symulujących ruch obiektu.
//    *
//    * Zestaw sekwencji poleceń symulujących ruch obiektu.
//    * Każde polecenie odpowiada kolejnym stanom obiektu.
//    */
//    const char** _Cmd4StatDesc = nullptr;
  
//  public:

//   /*!
//    * \brief Ustawia zestaw poleceń odpowiadających kolejnym stanom
//    *        obiektu.
//    */
//   void SetCmds(const char *CmdsTab[STATES_NUMBER]) { _Cmd4StatDesc = CmdsTab; }
//   /*!
//    * \brief Udostępnia kolejny zestaw poleceń umożliwiających
//    *        zespołu obiektu.
//    *
//    * Udostępnia kolejny zestaw poleceń umożliwiających
//    * zespołu obiektu. Ta metoda "udaje" metodę, która w oryginalnym
//    * rozwiązaniu powinna wygenerować odpowiednie polecenie na podstawie
//    * przechowywanej informacji o położeniu i orientacji obiektu.
//    */
//   const char* GetStateDesc() const
//   {
//     return _Cmd4StatDesc[_StateIdx];
//   }
//   /*!
//    * \brief Zwiększa indeks stanu, o ile aktualny opis nie jest pusty.
//    *
//    *  Zwiększa indeks stanu, o ile aktualny opis nie jest pusty.
//    *  Ta metoda "udaje" metodę, która w oryginalnym rozwiązaniu
//    *  jest odpowiedzialna za zmianę stanu obiektu, tzn. zmianę jego
//    *  położenia lub orientacji.
//    */
//   bool IncStateIndex() {
//     if (_StateIdx >= STATES_NUMBER-1) return false;
//     ++_StateIdx;
//     return true;
//   }
// };










// /*!
//  * \brief Funkcja jest treścią wątku komunikacyjnego
//  * 
//  * Funkcja jest treścią wątku komunikacyjnego.
//  * \param[in] rSender - odpowiada za śledenie zmian na scenie 
//  *                      i przesyłanie do serwera graficznego
//  *                      aktualnego stanu sceny, gdy uległ on zmianie.
//  */
// void Fun_CommunicationThread(Sender  *pSender)
// {
//   pSender->Watching_and_Sending();
// }


// /*!
//  * Otwiera połączenie sieciowe
//  * \param[out]  rSocket - deskryptor gniazda, poprzez które może być
//  *                        realizowana komunikacja sieciowa.
//  */
// bool OpenConnection(int &rSocket)
// {
//   struct sockaddr_in  DaneAdSerw;

//   bzero((char *)&DaneAdSerw,sizeof(DaneAdSerw));

//   DaneAdSerw.sin_family = AF_INET;
//   DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
//   DaneAdSerw.sin_port = htons(PORT);


//   rSocket = socket(AF_INET,SOCK_STREAM,0);

//   if (rSocket < 0) {
//      cerr << "*** Blad otwarcia gniazda." << endl;
//      return false;
//   }

//   if (connect(rSocket,(struct sockaddr*)&DaneAdSerw,sizeof(DaneAdSerw)) < 0)
//    {
//      cerr << "*** Brak mozliwosci polaczenia do portu: " << PORT << endl;
//      return false;
//    }
//   return true;
// }




// /*!
//  * \brief Przykład wykonywania pojedynczej operacji z animacją.
//  *
//  * Przykład wykonywania pojedynczej operacji z animacją.
//  * \param[in] pObj - wskaźnik na obiekt, dla którego ma być wykonana dana operacja
//  *                   zmiany jego stanu.
//  * \param[in] pAccCtrl - wskaźnik na obiekt, poprzez który przekazywana jest informacja
//  *                   o zmianie na scenie, zaś w trakcie dokonywania zmianay
//  *                   zamykany jest dostęp do całej sceny.
//  * \retval true - Jeśli dokonan zosała zmiana stanu wszystkich obiektów.
//  * \retval false - w przypadku przeciwnym.
//  */
// bool ChangeState(Scene &Scn) //GeomObject *pObj, AccessControl  *pAccCtrl)
// {
//   bool Changed;

//   while (true) {
//     Scn.LockAccess(); // Zamykamy dostęp do sceny, gdy wykonujemy
//                             // modyfikacje na obiekcie.
//     for (GeomObject &rObj : Scn._Container4Objects) {
//        if (!(Changed = rObj.IncStateIndex())) { Scn.UnlockAccess();  return false; }
//     }
//     Scn.MarkChange();
//     Scn.UnlockAccess();
//     usleep(300000);
//   }
//   return true;
// }

