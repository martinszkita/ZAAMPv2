#include "XMLInterp4Config.hh"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <iostream>
#include <stdexcept>

using namespace xercesc;

XMLInterp4Config::XMLInterp4Config(Configuration &rConfig) : _config(rConfig) {
    for (const auto &conf : rConfig.cubes) {
        if (conf.name.empty()) { // nazwa prostopadloscianu jest obowiazkowa
            std::cerr << "Error: No cube name provided for the Config constructor!" << std::endl;
            throw std::runtime_error("Missing cube name in configuration.");
        }
    }
}

void XMLInterp4Config::startDocument() {
    std::cout << "Starting XML document parsing.\n";
}

void XMLInterp4Config::endDocument() {
    std::cout << "Finished XML document parsing.\n";
}

void XMLInterp4Config::startElement(const XMLCh *const, const XMLCh *const pLocalName,
                                    const XMLCh *const, const Attributes &rAttrs) {
    char *elementName = XMLString::transcode(pLocalName);

    if (strcmp(elementName, "Lib") == 0) {
        ProcessLibAttrs(rAttrs);
    } else if (strcmp(elementName, "Cube") == 0) {
        ProcessCubeAttrs(rAttrs);
    }

    XMLString::release(&elementName);
}

void XMLInterp4Config::endElement(const XMLCh *const, const XMLCh *const pLocalName,
                                  const XMLCh *const) {
    char *elementName = XMLString::transcode(pLocalName);
    //std::cout << "End element: " << elementName << "\n";
    XMLString::release(&elementName);
}

void XMLInterp4Config::fatalError(const SAXParseException &exception) {
    char *message = XMLString::transcode(exception.getMessage());
    std::cerr << "Fatal XML error: " << message << "\n";
    XMLString::release(&message);
}

void XMLInterp4Config::ProcessLibAttrs(const Attributes &rAttrs) {
    char *name = XMLString::transcode(rAttrs.getValue(XMLString::transcode("Name")));
    if (name) {
        _config.plugins.push_back(name);
        std::cout << "Loaded plugin: " << name << "\n";
        XMLString::release(&name);
    }
}

void XMLInterp4Config::ProcessCubeAttrs(const Attributes &rAttrs) {
    Cube cube;

    char *name = XMLString::transcode(rAttrs.getValue(XMLString::transcode("Name")));
    if (name) {
        cube.name = name;
        XMLString::release(&name);
    }

    if (rAttrs.getValue(XMLString::transcode("Shift"))) {
        char *shift = XMLString::transcode(rAttrs.getValue(XMLString::transcode("Shift")));
        cube.shift = shift;
        XMLString::release(&shift);
    }

    if (rAttrs.getValue(XMLString::transcode("Scale"))) {
        char *scale = XMLString::transcode(rAttrs.getValue(XMLString::transcode("Scale")));
        cube.scale = scale;
        XMLString::release(&scale);
    }

    if (rAttrs.getValue(XMLString::transcode("RotXYZ_deg"))) {
        char *rotXYZ = XMLString::transcode(rAttrs.getValue(XMLString::transcode("RotXYZ_deg")));
        cube.rotXYZ = rotXYZ;
        XMLString::release(&rotXYZ);
    }

    if (rAttrs.getValue(XMLString::transcode("Trans_m"))) {
        char *trans = XMLString::transcode(rAttrs.getValue(XMLString::transcode("Trans_m")));
        cube.trans_m = trans;
        XMLString::release(&trans);
    }

    if (rAttrs.getValue(XMLString::transcode("RGB"))) {
        char *rgb = XMLString::transcode(rAttrs.getValue(XMLString::transcode("RGB")));
        cube.color = rgb;
        XMLString::release(&rgb);
    }

    _config.cubes.push_back(cube);
    std::cout << "Loaded cube: " << cube.name << "\n";
}

Configuration XMLInterp4Config::redConfigurationFromXML(const char * fileName){
        Configuration config;

        try {
        xercesc::XMLPlatformUtils::Initialize();

        // Tworzenie parsera SAX2
        xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
        XMLInterp4Config handler(config);

        // Konfiguracja parsera
        parser->setContentHandler(&handler);
        parser->setErrorHandler(&handler);

        std::cout << "Parsing configuration file: " << fileName << "\n";
        parser->parse(fileName);

        
        delete parser;
        xercesc::XMLPlatformUtils::Terminate();

    } catch (const xercesc::XMLException &e) {
        char *message = xercesc::XMLString::transcode(e.getMessage());
        std::cerr << "XML Parsing error: " << message << "\n";
        xercesc::XMLString::release(&message);
        return config;
    }

    return config;
}

