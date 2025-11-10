/*!
 * \file Configuration.hh
 * \brief Definicja struktur Cube i Configuration
 *
 * Plik zawiera definicje struktur \link Cube Cube \endlink i
 * \link Configuration Configuration \endlink, które przechowują dane
 * dotyczące konfiguracji obiektów geometrycznych i wtyczek.
 */

#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <vector>

/*!
 * \struct Cube
 * \brief Przechowuje dane dotyczące pojedynczego obiektu geometrycznego typu sześcian.
 *
 * Struktura \link Cube Cube \endlink opisuje obiekt geometryczny zdefiniowany
 * w konfiguracji XML. Zawiera informacje o nazwie obiektu, jego pozycji, skalowaniu,
 * orientacji i kolorze.
 */
struct Cube {
    /*!
     * \brief Nazwa obiektu.
     *
     * Obowiązkowe pole, które identyfikuje obiekt.
     */
    std::string name;

    /*!
     * \brief Przesunięcie obiektu w przestrzeni 3D.
     *
     * Domyślna wartość: "0 0 0".
     */
    std::string shift = "0 0 0";

    /*!
     * \brief Skalowanie obiektu w przestrzeni 3D.
     *
     * Domyślna wartość: "1 1 1".
     */
    std::string scale = "1 1 1";

    /*!
     * \brief Orientacja obiektu w przestrzeni (rotacja wokół osi XYZ).
     *
     * Wartości podawane w stopniach. Domyślna wartość: "0 0 0".
     */
    std::string rotXYZ = "0 0 0";

    /*!
     * \brief Translacja obiektu w metrach.
     *
     * Domyślna wartość: "0 0 0".
     */
    std::string trans_m = "0 0 0";

    /*!
     * \brief Kolor obiektu w przestrzeni RGB.
     *
     * Wartości RGB oddzielone spacjami. Domyślna wartość: "128 128 128".
     */
    std::string color = "128 128 128";
};

/*!
 * \struct Configuration
 * \brief Przechowuje dane konfiguracji całej sceny.
 *
 * Struktura \link Configuration Configuration \endlink zawiera informacje
 * o wtyczkach dynamicznych oraz obiektach geometrycznych definiowanych
 * w konfiguracji XML.
 */
struct Configuration {
    /*!
     * \brief Lista nazw wtyczek dynamicznych do załadowania.
     */
    std::vector<std::string> plugins;

    /*!
     * \brief Lista obiektów geometrycznych zdefiniowanych na scenie.
     */
    std::vector<Cube> cubes;
};

/*!
    \brief Funkcja zmieniająca string w formie "x y z" na string w formie wektora "(x,y,z)"

    \param[in] string w postaci "x y z"
    \param[out] string w postaci "(x,y,z)"
*/
inline std::string stringToVecString(std::string str){
    std::string result = str;
    
    // Zamień wszystkie spacje na przecinki
    for (size_t pos = 0; (pos = result.find(' ', pos)) != std::string::npos; ++pos) {
        result.replace(pos, 1, ",");
    }
    
    // Dodaj nawiasy
    result.insert(0, "(");
    result.append(")");
    
    return result;

}

/*!

*/

#endif

