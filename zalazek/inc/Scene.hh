/*!
 * \file Scene.hh
 * \brief Definicja klasy Scene
 *
 * Plik zawiera definicję klasy \link Scene Scene \endlink,
 * która implementuje zarządzanie obiektami mobilnymi na scenie.
 */

#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AccessControl.hh"
#include "AbstractMobileObj.hh"
#include "GeomObject.hh"
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "StateNumber.hh"


/*!
 * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 1
 * i obrazujących jego ruch.
 */

/*
  Dlaczego to działa? 
  Specyfikator inline w C++ oznacza, że zmienna może być definiowana w wielu plikach, 
  ale linker scali wszystkie kopie w jedną podczas procesu linkowania.
*/
 inline const char* Cmds4Obj1[STATES_NUMBER] = {
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,23)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,26)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,29)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,32)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,35)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,38)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,41)\n",
  "UpdateObj Name=Podstawa1 RotXYZ_deg=(0,-45,44)\n"  
 };


/*!
 * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 2
 * i obrazujących jego ruch.
 */
inline const char* Cmds4Obj2[STATES_NUMBER] = {
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-48,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-45,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-42,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-39,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-36,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-33,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-30,0)\n",
  "UpdateObj Name=Podstawa1.Ramie1 RotXYZ_deg=(0,-27,0)\n"
 };

/*!
 * Zestaw poleceń odpowiadających kolejnym stanom obiektu nr 3
 * i obrazujących jego ruch.
 */
inline const char* Cmds4Obj3[STATES_NUMBER] = {
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-48,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-51,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-54,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-57,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-60,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-63,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-66,0)\n",
  "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-69,0)\n"
 };

/*!
 * \class Scene
 * \brief Implementacja klasy zarządzającej obiektami mobilnymi na scenie.
 *
 * Klasa \link Scene Scene \endlink przechowuje obiekty mobilne (MobileObj)
 * i umożliwia ich wyszukiwanie oraz zarządzanie. Klasa implementuje interfejs
 * \link AbstractScene AbstractScene \endlink.
 */
class Scene : public AbstractScene, public AccessControl {
    /*!
     * \brief Mapa przechowująca obiekty mobilne na scenie.
     *
     * Klucz mapy to nazwa obiektu mobilnego, a wartość to wskaźnik (smart pointer)
     * na obiekt mobilny.
     */
    std::map<std::string, std::shared_ptr<AbstractMobileObj>> _objects;
public:
  // /*!
  //  * \brief Prosta kolekcja obiektów sceny
  //  */
  //  std::vector<GeomObject>   _Container4Objects;
    /*!
     * \brief Konstruktor domyślny klasy Scene.
     *
     * Inicjalizuje pustą scenę.
     */
    Scene() = default;

    // Scene(): _Container4Objects(3)
    // {
    //   _Container4Objects[0].SetCmds(Cmds4Obj1);
    //   _Container4Objects[1].SetCmds(Cmds4Obj2);
    //   _Container4Objects[2].SetCmds(Cmds4Obj3);
    // }

    /*!
     * \brief Destruktor klasy Scene.
     *
     * Zwalnia wszystkie zasoby związane z przechowywanymi obiektami.
     */
    ~Scene();

    /*!
     * \brief Wyszukuje obiekt mobilny po nazwie.
     *
     * Wyszukuje obiekt mobilny o podanej nazwie na scenie.
     * \param[in] sName - Nazwa obiektu mobilnego do znalezienia.
     * \return Wskaźnik na obiekt mobilny, jeśli istnieje; nullptr w przeciwnym razie.
     */
    AbstractMobileObj* FindMobileObj(const char *sName) override;

    /*!
     * \brief Dodaje obiekt mobilny do sceny.
     *
     * Dodaje nowy obiekt mobilny do kolekcji obiektów na scenie.
     * \param[in] pMobObj - Wskaźnik na obiekt mobilny do dodania.
     */
    void AddMobileObj(AbstractMobileObj *pMobObj) override;

    /*!
     * \brief Pobiera mapę wszystkich obiektów mobilnych na scenie.
     *
     * Zwraca stałą referencję do mapy przechowującej obiekty mobilne
     * identyfikowane przez ich nazwy.
     * \return Stała referencja do mapy obiektów mobilnych.
     */
    const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& GetObjects() const;

    /*!
     * \brief Ustawia nową mapę obiektów mobilnych na scenie.
     *
     * Ustawia kolekcję obiektów mobilnych na scenie, zastępując poprzednie dane.
     * \param[in] objects - Mapa obiektów mobilnych identyfikowanych przez ich nazwy.
     */
    void SetObjects(const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& objects);

    void PrintAllSceneObjects() const;

private:

};

#endif
