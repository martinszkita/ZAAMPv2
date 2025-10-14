/*!
 * \file MobileObj.hh
 * \brief Definicja klasy MobileObj
 *
 * Plik zawiera definicję klasy \link MobileObj MobileObj \endlink,
 * która modeluje prostopadłościan reprezentujący części robota.
 * Klasa dziedziczy po \link AbstractMobileObj AbstractMobileObj \endlink
 * i implementuje jej interfejs.
 */

#ifndef MOBILEOBJ_HH
#define MOBILEOBJ_HH

#include "AbstractMobileObj.hh"
#include "Vector3D.hh"
#include "StateNumber.hh"

/*!
 * \class MobileObj
 * \brief Modeluje prostopadłościan reprezentujący części robota.
 *
 * Klasa \link MobileObj MobileObj \endlink przechowuje informacje
 * o pozycji, orientacji oraz nazwie bryły w przestrzeni 3D. Służy
 * do reprezentacji części robota i pozwala na odczyt i modyfikację
 * tych właściwości.
 */
class MobileObj : public AbstractMobileObj
{
private:
  /*!
   * \brief Identyfikuje aktualny stan obiektu.
   *
   * Identyfikuje aktualny stan obiektu. Indeks ten pozwala
   * na wybór z tablicy \link GeomObject::_Cmd4StatDesc _Cmd4StatDesc\endlink
   * odpowiedniego polecenia dla serwera, które reprezentuje aktualne
   * położenie i orientację obiektu.
   */
  int _StateIdx = 0;
  /*!
   * \brief Zestaw sekwencji poleceń symulujących ruch obiektu.
   *
   * Zestaw sekwencji poleceń symulujących ruch obiektu.
   * Każde polecenie odpowiada kolejnym stanom obiektu.
   */
  const char **_Cmd4StatDesc = nullptr;

  /*!
   * \brief Nazwa bryły.
   *
   * Unikalna nazwa identyfikująca obiekt.
   */
  std::string _name;

  /*!
   * \brief Wektor pozycji bryły w przestrzeni 3D.
   *
   * Przechowuje współrzędne pozycji środka bryły w metrach.
   */
  Vector3D _position;

  Vector3D _RotXYZ_deg;

public:
  /*!
   * \brief Konstruktor parametryczny.
   *
   * Inicjalizuje obiekt mobilny z nazwą oraz opcjonalnymi wartościami pozycji i orientacji.
   *
   * \param name - Nazwa obiektu (wymagana).
   * \param position - Pozycja obiektu (domyślnie {0, 0, 0}).
   * \param RotXYZ_deg - Kąty RPY obiektu w stopniach(domyślnie {0, 0, 0}).
   */
  MobileObj(const std::string &name,
            const Vector3D &position = {0.0, 0.0, 0.0},
            const Vector3D &RotXYZ_deg = {0.0, 0.0, 0.0})

      : _name(name), _position(position), _RotXYZ_deg(RotXYZ_deg)
  {
  }

  /*!
   * \brief Pobiera pozycję obiektu w metrach.
   * \return Wektor 3D zawierający współrzędne pozycji.
   */
  const Vector3D &GetPosition_m() const override;

  /*!
   * \brief Ustawia pozycję obiektu w metrach.
   * \param[in] rPos - Wektor pozycji w przestrzeni 3D.
   */
  void SetPosition_m(const Vector3D &rPos) override;

  const Vector3D &GetRotXYZ_deg() const override;
  void SetRotXYZ_deg(const Vector3D &rPos) override;

  /*!
   * \brief Pobiera nazwę obiektu.
   * \return Ciąg znaków reprezentujący nazwę obiektu.
   */
  const std::string &GetName() const override;

  /*!
   * \brief Ustawia nazwę obiektu.
   * \param[in] sName - Wskaźnik na ciąg znaków reprezentujący nazwę.
   */
  void SetName(const char *sName) override;

  /*!
   * \brief Ustawia zestaw poleceń odpowiadających kolejnym stanom
   *        obiektu.
   */
  void SetCmds(const char *CmdsTab[STATES_NUMBER]) { _Cmd4StatDesc = CmdsTab; }
  /*!
   * \brief Udostępnia kolejny zestaw poleceń umożliwiających
   *        zespołu obiektu.
   *
   * Udostępnia kolejny zestaw poleceń umożliwiających
   * zespołu obiektu. Ta metoda "udaje" metodę, która w oryginalnym
   * rozwiązaniu powinna wygenerować odpowiednie polecenie na podstawie
   * przechowywanej informacji o położeniu i orientacji obiektu.
   */
  std::string GetStateDesc() const;
  /*!
   * \brief Zwiększa indeks stanu, o ile aktualny opis nie jest pusty.
   *
   *  Zwiększa indeks stanu, o ile aktualny opis nie jest pusty.
   *  Ta metoda "udaje" metodę, która w oryginalnym rozwiązaniu
   *  jest odpowiedzialna za zmianę stanu obiektu, tzn. zmianę jego
   *  położenia lub orientacji.
   */
  bool IncStateIndex()
  {
    if (_StateIdx >= STATES_NUMBER - 1)
      return false;
    ++_StateIdx;
    return true;
  }

  double operator[](char axis) const;
  double &operator[](char axis);
  static int GetAxisIndex(char axis);
};

#endif
