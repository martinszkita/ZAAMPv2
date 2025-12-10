/*!
 * \file Interp4Set.hh
 * \brief Definicja klasy Interp4Set
 *
 * Plik zawiera definicję klasy \link Interp4Set Interp4Set \endlink,
 * która implementuje polecenie ustawienia pozycji i orientacji obiektu.
 * Klasa dziedziczy po \link AbstractInterp4Command AbstractInterp4Command \endlink
 * i realizuje jego interfejs.
 */

#ifndef INTERP4SET__HH
#define INTERP4SET__HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include "AbstractInterp4Command.hh"
#include <iostream>
#include <string>
#include <sstream>

/*!
 * \class Interp4Set
 * \brief Klasa implementująca polecenie ustawienia pozycji i orientacji obiektu mobilnego.
 *
 * Klasa \link Interp4Set Interp4Set \endlink umożliwia ustawienie pozycji w przestrzeni 3D
 * oraz orientacji obiektu mobilnego w scenie.
 */
class Interp4Set : public AbstractInterp4Command {
private:
    /*!
     * \brief Nazwa obiektu mobilnego, którego dotyczy polecenie.
     */
    std::string _robotName;

    /*!
     * \brief Współrzędne pozycji obiektu w przestrzeni 3D.
     */
    Vector3D pos;

    Vector3D rpy;

public:
    /*!
     * \brief Konstruktor domyślny klasy Interp4Set.
     */
    Interp4Set();

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
     * \brief Wykonuje polecenie ustawienia pozycji i orientacji obiektu.
     *
     * Realizuje ustawienie pozycji i orientacji dla zadanego obiektu mobilnego w scenie.
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
     * \brief Tworzy nową instancję komendy Interp4Set.
     *
     * \return Wskaźnik na nową instancję klasy Interp4Set.
     */
    static AbstractInterp4Command* CreateCmd();

    /*!
     * \brief Pobiera nazwę robota.
     *
     * \return Referencja do nazwy robota.
     */
    const std::string& GetRobotName() const;

    /*!
     * \brief Pobiera współrzędne pozycji obiektu.
     *
     * \return Referencja do wektora pozycji.
     */
    const Vector3D& GetPosition() const;

    const Vector3D& GetRPY() const;

    /*!
     * \brief Ustawia nazwę robota.
     *
     * \param[in] name - Nazwa robota.
     */
    void SetRobotName(const std::string& name);

    /*!
     * \brief Ustawia współrzędne pozycji obiektu.
     *
     * \param[in] position - Wektor pozycji w przestrzeni 3D.
     */
    void SetPosition(const Vector3D& position);

    void SetRPY(const Vector3D& RPY);

    
};

#endif
