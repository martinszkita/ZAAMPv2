/*!
 * \file ComChannel.hh
 * \brief Definicja klasy ComChannel
 *
 * Plik zawiera definicję klasy \link ComChannel ComChannel \endlink,
 * która implementuje interfejs komunikacyjny \link AbstractComChannel AbstractComChannel \endlink.
 * Klasa obsługuje zarządzanie dostępem do zasobów współdzielonych oraz wysyłanie komend.
 */

#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"
#include <iostream>
#include <mutex>
#include "Vector3D.hh"

/*!
 * \class ComChannel
 * \brief Implementacja kanału komunikacyjnego.
 *
 * Klasa \link ComChannel ComChannel \endlink dziedziczy po \link AbstractComChannel AbstractComChannel \endlink
 * i implementuje mechanizmy obsługi socketów oraz synchronizacji dostępu za pomocą mutexów.
 */
class ComChannel : public AbstractComChannel {
    /*!
     * \brief Gniazdo socketu używane do komunikacji.
     */
    int socket;

    /*!
     * \brief Mutex do synchronizacji dostępu do zasobów współdzielonych.
     */
    std::mutex access_mutex;

public:
    /*!
     * \brief Domyślny konstruktor klasy ComChannel.
     *
     * Tworzy obiekt bez inicjalizacji gniazda socketu.
     */
    ComChannel() {}

    /*!
     * \brief Domyślny destruktor klasy ComChannel.
     */
    ~ComChannel() {}

    /*!
     * \brief Pobiera numer socketu komunikacyjnego.
     * 
     * \return Numer socketu (jako wartość całkowita).
     */
    int GetSocket() const override;

    /*!
     * \brief Inicjalizuje kanał komunikacyjny przy użyciu podanego socketu.
     *
     * \param[in] Socket - Numer socketu do użycia w komunikacji.
     * \return true, jeśli inicjalizacja się powiodła; false w przeciwnym razie.
     */
    bool Init(int Socket) override;

    /*!
     * \brief Blokuje dostęp do zasobów współdzielonych.
     *
     * Używa mutexu do zapewnienia synchronizacji.
     */
    void LockAccess() override;

    /*!
     * \brief Odblokowuje dostęp do zasobów współdzielonych.
     *
     * Zwalnia blokadę mutexu.
     */
    void UnlockAccess() override;

    /*!
     * \brief Zwraca referencję do mutexu używanego w klasie.
     *
     * Umożliwia użycie mutexu w zewnętrznych kontekstach.
     * \return Referencja do mutexu.
     */
    std::mutex &UseGuard() override;

    /*!
     * \brief Wysyła komendę ruchu obiektu przez kanał komunikacyjny.
     *
     * Wysyła polecenie ruchu dla obiektu o podanej nazwie, prędkości i dystansie.
     * \param[in] objectName - Nazwa obiektu, który ma się poruszyć.
     * \param[in] speed - Prędkość ruchu obiektu.
     * \param[in] distance - Odległość, jaką ma pokonać obiekt.
     */
    bool SendMoveCommand(const std::string& objectName, Vector3D trans_m);
    bool SendRotateCommand(const std::string& objectName, char axis, double angSpeed, double angDeg);
    bool SendSetCommand(const std::string& objectName, Vector3D posXYZ, Vector3D RPYdeg);
};

#endif
