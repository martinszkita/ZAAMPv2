/*!
 * \file XMLInterp4Config.hh
 * \brief Definicja klasy XMLInterp4Config
 *
 * Plik zawiera definicję klasy \link XMLInterp4Config XMLInterp4Config \endlink,
 * która obsługuje parsowanie plików XML za pomocą SAX2 i wypełnia strukturę
 * \link Configuration Configuration \endlink odpowiednimi danymi.
 */

#ifndef XMLINTERP4CONFIG_HH
#define XMLINTERP4CONFIG_HH

#include "Configuration.hh"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <string>

/*!
 * \class XMLInterp4Config
 * \brief Klasa obsługująca parsowanie plików XML przy użyciu SAX2.
 *
 * Klasa \link XMLInterp4Config XMLInterp4Config \endlink dziedziczy po
 * \link xercesc::DefaultHandler DefaultHandler \endlink i implementuje metody
 * obsługi zdarzeń SAX2, takich jak początek dokumentu, elementu czy atrybutów.
 * Dane wczytywane z pliku XML są zapisywane do obiektu klasy 
 * \link Configuration Configuration \endlink.
 */
class XMLInterp4Config : public xercesc::DefaultHandler {
public:
    /*!
     * \brief Konstruktor klasy XMLInterp4Config.
     * 
     * Inicjalizuje obiekt i wiąże referencję do struktury \link Configuration Configuration \endlink,
     * w której będą przechowywane dane z parsowanego pliku XML.
     * \param[in] rConfig - Referencja do struktury Configuration.
     */
    XMLInterp4Config(Configuration &rConfig);

    /*!
     * \brief Konstruktor domyślny klasy XMLInterp4Config.
     *
     * Używany, jeśli nie przekazano struktury Configuration.
     */
    XMLInterp4Config();

    /*!
     * \brief Obsługa zdarzenia początkowego dokumentu.
     *
     * Wywoływana na początku parsowania dokumentu XML.
     */
    virtual void startDocument() override;

    /*!
     * \brief Obsługa zdarzenia końcowego dokumentu.
     *
     * Wywoływana po zakończeniu parsowania dokumentu XML.
     */
    virtual void endDocument() override;

    /*!
     * \brief Obsługa zdarzenia początku elementu XML.
     *
     * Wywoływana na początku każdego elementu XML.
     * \param[in] pURI - URI przestrzeni nazw.
     * \param[in] pLocalName - Lokalne imię elementu.
     * \param[in] pQName - Kwalifikowane imię elementu.
     * \param[in] rAttrs - Atrybuty elementu.
     */
    virtual void startElement(const XMLCh *const pURI,
                              const XMLCh *const pLocalName,
                              const XMLCh *const pQName,
                              const xercesc::Attributes &rAttrs) override;

    /*!
     * \brief Obsługa zdarzenia końca elementu XML.
     *
     * Wywoływana na końcu każdego elementu XML.
     * \param[in] pURI - URI przestrzeni nazw.
     * \param[in] pLocalName - Lokalne imię elementu.
     * \param[in] pQName - Kwalifikowane imię elementu.
     */
    virtual void endElement(const XMLCh *const pURI,
                            const XMLCh *const pLocalName,
                            const XMLCh *const pQName) override;

    /*!
     * \brief Obsługa krytycznego błędu podczas parsowania.
     *
     * Wywoływana w przypadku napotkania nieusuwalnego błędu podczas parsowania XML.
     * \param[in] exception - Szczegóły błędu SAX.
     */
    void fatalError(const xercesc::SAXParseException &exception) override;

    /*!
     * \brief Statyczna metoda wczytująca konfigurację z pliku XML.
     *
     * Tworzy obiekt klasy Configuration, wypełnia go danymi z pliku XML
     * i zwraca jako wynik.
     * \param[in] fileName - Ścieżka do pliku XML z konfiguracją.
     * \return Struktura Configuration zawierająca dane z pliku XML.
     */
    static Configuration redConfigurationFromXML(const char *fileName);

private:
    /*!
     * \brief Referencja do struktury Configuration.
     *
     * Referencja do obiektu \link Configuration Configuration \endlink, w którym
     * będą przechowywane dane z pliku XML.
     */
    Configuration &_config;

    /*!
     * \brief Przetwarza atrybuty elementu \c Lib.
     *
     * Wyciąga informacje o bibliotece dynamicznej z atrybutów elementu XML.
     * \param[in] rAttrs - Atrybuty elementu \c Lib.
     */
    void ProcessLibAttrs(const xercesc::Attributes &rAttrs);

    /*!
     * \brief Przetwarza atrybuty elementu \c Cube.
     *
     * Wyciąga informacje o obiektach geometrycznych z atrybutów elementu XML.
     * \param[in] rAttrs - Atrybuty elementu \c Cube.
     */
    void ProcessCubeAttrs(const xercesc::Attributes &rAttrs);
};

#endif
