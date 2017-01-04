#include "../../include/game/Object.hpp"

//*---------------
// Constructors
//---------------*

Object::Object(	const std::vector<std::string>& attributes,
				int cost,
				const std::string& ID,
				const std::string& type,
				const std::vector<std::string>& keywords,
				const std::string& description,
				const std::string& name,
				const std::vector<std::string>& wearflags,
               	int weight)
				:	attributes(attributes)
				,	cost(cost)
				,	ID(ID)
				,	type(type)
				,	keywords(keywords)
				,	description(description)
				,	name(name)
				,	wearflags(wearflags)
				,	weight(weight)
               	{
               		try
			        {
			        	// TOCHECK: Are the parameters right?
			            if (cost < 0 ||
			                ID == "" ||
			                type == "" ||
			                description == "" ||
			                name == "" ||
			                weight <= 0	
			                )
			            {
			                throw InvalidObjectException();
			            }
			        }
			        catch(InvalidObjectException& e)
			        {
			            std::cout << e.what() << std::endl;
			        }
               	}

//Defaults
const std::string Object::defaultObjectID = " ";
const std::string Object::defaultItemType = " ";
const std::string Object::defaultShortDesc = " ";

const std::vector<std::string> Object::defaultAttributes = {"noAttributes", " "};
const std::vector<std::string> Object::defaultExtra = {"noExtraDescription", " "};
const std::vector<std::string> Object::defaultKeywords = {"noKeywords", " "};
const std::vector<std::string> Object::defaultLongDesc = {"noLongDesc", " "};
const std::vector<std::string> Object::defaultWearFlags = {"noWearFlags", " "};


//*---------------
// Getters
//---------------*

std::string Object::getDescription() const {
	return description;
}

std::vector<std::string> Object::getAttributes() const {
	return attributes;
}

int Object::getCost() const {
	return cost;
}

std::vector<std::string> Object::getExtra() const {
	return extra;
}

std::string Object::getID() const {
	return ID;
}

std::string Object::getType() const {
	return type;
}
 //Maybe we can have a subclass for each type of object
std::vector<std::string> Object::getKeywords() const {
	return keywords;
}

std::string Object::getName() const {
	return name;
}

std::vector<std::string> Object::getWearflags() const {
	return wearflags;
}

int Object::getWeight() const {
	return weight;
}