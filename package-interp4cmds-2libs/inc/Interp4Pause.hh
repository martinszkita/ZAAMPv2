/*!
 * \file Interp4Pause.hh
 * \brief Definicja klasy Interp4Pause
 *
 * Plik zawiera definicję klasy \link Interp4Pause Interp4Pause \endlink,
 * która implementuje polecenie pauzy w wykonaniu operacji robota.
 * Klasa dziedziczy po \link AbstractInterp4Command AbstractInterp4Command \endlink
 * i realizuje jego interfejs.
 */

#ifndef INTERP4PAUSE_HH
#define INTERP4PAUSE_HH

#include "AbstractInterp4Command.hh"
#include <iostream>
#include <string>
#include <sstream>

/*!
 * \class Interp4Pause
 * \brief Klasa implementująca polecenie pauzy dla robota.
 *
 * Klasa \link Interp4Pause Interp4Pause \endlink umożliwia zatrzymanie operacji
 * na zadany czas w milisekundach. Polecenie może być wykonane na konkretnym
 * obiekcie mobilnym na scenie.
 */
class Interp4Pause : public AbstractInterp4Command {
    /*!
     * \brief Nazwa robota, dla którego wykonywana jest pauza.
     */
    std::string _robotName;

    /*!
     * \brief Czas pauzy w milisekundach.
     */
    unsigned int _pause_time_ms;

public:
    /*!
     * \brief Konstruktor domyślny klasy Interp4Pause.
     */
    Interp4Pause();

    /*!
     * \brief Wypisuje komendę w formacie tekstowym.
     */
    virtual void PrintCmd() const override;

    /*!
     * \brief Pobiera nazwę komendy.
     *
     * \return Ciąg znaków reprezentujący nazwę komendy.
     */
    virtual const char* GetCmdName() const override;

    /*!
     * \brief Wykonuje polecenie pauzy.
     *
     * Realizuje pauzę na zadanym obiekcie mobilnym w scenie.
     * \param[in] rScn - Referencja do sceny.
     * \param[in] sMobObjName - Nazwa obiektu mobilnego.
     * \param[in] rComChann - Kanał komunikacyjny.
     * \return true, jeśli polecenie wykonano pomyślnie; false w przeciwnym razie.
     */
    virtual bool ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann) override;

    /*!
     * \brief Wczytuje parametry polecenia z wejściowego strumienia.
     *
     * \param[in] strm - Strumień wejściowy zawierający parametry.
     * \return true, jeśli parametry wczytano poprawnie; false w przeciwnym razie.
     */
    virtual bool ReadParams(std::istream& strm) override;

    /*!
     * \brief Wypisuje składnię komendy.
     */
    virtual void PrintSyntax() const override;

    /*!
     * \brief Wypisuje aktualne parametry komendy.
     */
    virtual void PrintParams() const override;

    /*!
     * \brief Tworzy nową instancję komendy Interp4Pause.
     *
     * \return Wskaźnik na nową instancję klasy Interp4Pause.
     */
    static AbstractInterp4Command* CreateCmd();

    /*!
     * \brief Pobiera nazwę robota.
     *
     * \return Referencja do nazwy robota.
     */
    const std::string& GetRobotName() const;

    /*!
     * \brief Pobiera czas pauzy w milisekundach.
     *
     * \return Referencja do czasu pauzy.
     */
    const unsigned int& GetPauseTime() const;

    /*!
     * \brief Ustawia nazwę robota.
     *
     * \param[in] name - Nazwa robota.
     */
    void SetRobotName(const std::string& name);

    /*!
     * \brief Ustawia czas pauzy w milisekundach.
     *
     * \param[in] _ms - Czas pauzy w milisekundach.
     */
    void SetPauseTime(const unsigned int &_ms);
};

#endif
