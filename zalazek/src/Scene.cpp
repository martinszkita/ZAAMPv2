#include "Scene.hh"

// Destruktor
Scene::~Scene() = default;

// Implementacja FindMobileObj - szuka obiektu po nazwie
AbstractMobileObj* Scene::FindMobileObj(const char *sName) {
    auto it = _objects.find(sName);
    if (it != _objects.end()) {
        return it->second.get();
    }
    return nullptr;
}

// Implementacja AddMobileObj - dodaje nowy obiekt mobilny do mapy
void Scene::AddMobileObj(AbstractMobileObj *pMobObj) {
    _objects[pMobObj->GetName()] = std::shared_ptr<AbstractMobileObj>(pMobObj);
}

// Getter dla _objects - zwraca stałą referencję
const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& Scene::GetObjects() const {
    return _objects;
}

// Setter dla _objects - przypisuje nową mapę obiektów
void Scene::SetObjects(const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& objects) {
    _objects = objects;
}

void Scene::PrintAllSceneObjects() const{
    for (const auto & objName : GetObjects()){
        std::cout << objName.first << " ";
    }
}
