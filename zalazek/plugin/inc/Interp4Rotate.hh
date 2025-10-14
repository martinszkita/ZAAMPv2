/*!
 * \file Interp4Rotate.hh
 * \brief Definicja klasy Interp4Rotate
 *
 * Plik zawiera definicję klasy \link Interp4Rotate Interp4Rotate \endlink,
 * która implementuje polecenie obrotu obiektu wokół osi.
 * Klasa dziedziczy po \link AbstractInterp4Command AbstractInterp4Command \endlink
 * i realizuje jego interfejs.
 */

#ifndef INTERP4ROTATE__HH
#define INTERP4ROTATE__HH

#include "AbstractInterp4Command.hh"
#include "Vector3D.hh"
#include <string>
#include <iostream>

/*!
 * \class Interp4Rotate
 * \brief Klasa implementująca polecenie obrotu obiektu wokół osi.
 *
 * Klasa \link Interp4Rotate Interp4Rotate \endlink umożliwia wykonanie polecenia obrotu
 * obiektu mobilnego wokół zadanej osi z określoną prędkością kątową i kątem obrotu.
 */
class Interp4Rotate : public AbstractInterp4Command {
private:
    /*!
     * \brief Nazwa obiektu mobilnego, dla którego wykonywany jest obrót.
     */
    std::string _robotName;

    /*!
     * \brief Oś obrotu ('x', 'y', lub 'z').
     */
    char _axis;

    /*!
     * \brief Prędkość kątowa obrotu w stopniach na sekundę.
     */
    double _angularVelocity;

    /*!
     * \brief Kąt obrotu w stopniach.
     */
    double _angle;

public:
    /*!
     * \brief Konstruktor domyślny klasy Interp4Rotate.
     */
    Interp4Rotate();

    /*!
     * \brief Pobiera nazwę robota.
     *
     * \return Referencja do nazwy robota.
     */
    const std::string& GetRobotName() const;

    /*!
     * \brief Pobiera oś obrotu.
     *
     * \return Znak reprezentujący oś ('x', 'y', 'z').
     */
    char GetAxis() const;

    /*!
     * \brief Pobiera prędkość kątową obrotu.
     *
     * \return Prędkość kątowa w stopniach na sekundę.
     */
    double GetAngularVelocity() const;

    /*!
     * \brief Pobiera kąt obrotu.
     *
     * \return Kąt obrotu w stopniach.
     */
    double GetAngle() const;

    /*!
     * \brief Ustawia nazwę robota.
     *
     * \param[in] name - Nazwa robota.
     */
    void SetRobotName(const std::string& name);

    /*!
     * \brief Ustawia oś obrotu.
     *
     * \param[in] axis - Znak reprezentujący oś ('x', 'y', 'z').
     */
    void SetAxis(char axis);

    /*!
     * \brief Ustawia prędkość kątową obrotu.
     *
     * \param[in] angularVelocity - Prędkość kątowa w stopniach na sekundę.
     */
    void SetAngularVelocity(double angularVelocity);

    /*!
     * \brief Ustawia kąt obrotu.
     *
     * \param[in] angle - Kąt obrotu w stopniach.
     */
    void SetAngle(double angle);

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
     * \brief Wykonuje polecenie obrotu.
     *
     * Realizuje obrót na zadanym obiekcie mobilnym w scenie.
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
     * \brief Tworzy nową instancję komendy Interp4Rotate.
     *
     * \return Wskaźnik na nową instancję klasy Interp4Rotate.
     */
    static AbstractInterp4Command* CreateCmd();

};

#endif
