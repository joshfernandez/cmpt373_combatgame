#include "../../include/game/Area.hpp"

//*---------------
// Constructors
//---------------*

Area::Area( const std::string &id, 
            const std::string &title, 
            const Doors& connectedAreas, 
            const std::string &description,
			const std::unordered_map<std::string, std::string>& extendedDescriptions)
    : id(id)
    , title(title)
    , connectedAreas(connectedAreas)
    , description(description)
    , extendedDescriptions(extendedDescriptions)
    {
        try
        {
            // TOCHECK: Are the parameters right?
            if (id == "" ||
                title == ""
                )
            {
                throw InvalidAreaException();
            }
        }
        catch(InvalidAreaException& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

Area::Area(const std::string &id, const std::string &title)
    : id(id)
    , title(title)
{
    try
        {
            // TOCHECK: Are the parameters right?
            if (id == "" ||
                title == ""
                )
            {
                throw InvalidAreaException();
            }
        }
        catch(InvalidAreaException& e)
        {
            std::cout << e.what() << std::endl;
        }
}

Area::Area(std::string id) :id{std::move(id)} {}

//*---------------
// Getters
//---------------*


//Accessors
const std::string& Area::getTitle() const {
    return title;
}

const std::string& Area::getID() const {
    return id;
}

const std::string& Area::getDescription() const {
    return description;
}

Area::Doors* Area::getConnectedAreas() const{ 
    return (Doors*) &connectedAreas;
}

std::unordered_map<std::string, std::string> Area::getExtendedDescriptions() const {
    return extendedDescriptions;
}

void Area::setTitle(const std::string& title) {
    this->title = title;
}

void Area::setDescription(const std::string& description) {
    this->description = description;
}

void Area::addConnectedArea(const std::string& exit, const std::string& areaID) {
    if (connectedAreas.count(exit) == 1) {
        connectedAreas.erase(exit);
    }

    connectedAreas.emplace(exit, areaID);
}

void Area::removeConnectedArea(const std::string& exit) {
    connectedAreas.erase(exit);
}
//Setters
void Area::addObjects(std::string object) {
    this->objectNames.push_back(object);
}

const std::vector<std::string>& Area::getObjectNames() const{
	return objectNames;
}