#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <exception>

#include "Object.hpp"


class Area 
{

	using Doors =  std::unordered_map<std::string, std::string>;

public:

	//*---------------
	// Constructors
	//---------------*

	Area(const std::string &id, const std::string &title, const Doors& connectedAreas, const std::string &description, const std::unordered_map<std::string, std::string>& extendedDescriptions);
	Area(const std::string &id, const std::string &title); //For testing
    Area(std::string id);
	Area(){};

	//*---------------
	// Getters
	//---------------*

	const std::string& getID() const;
	const std::string& getTitle() const;
	const std::string& getDescription() const;
    const std::vector<Object>& getObjectList() const;
    Doors* getConnectedAreas() const;
	std::unordered_map<std::string, std::string> getExtendedDescriptions() const;

	void addObjects(std::string);
	const std::vector<std::string>& getObjectNames() const;

    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void addConnectedArea(const std::string& exit, const std::string& areaID);
	void removeConnectedArea(const std::string& exit);
    void addObject(const Object& object);

private:
    // Id of the area object, may not be needed depending on networking implementation

	std::string id;
    std::string title;
	Doors connectedAreas;

    std::string description;
	std::unordered_map<std::string, std::string> extendedDescriptions;
	std::vector<std::string> objectNames;
};

/*
 *  VARIOUS AREA EXCEPTIONS
 */

struct InvalidAreaException : public std::exception
{

    virtual const char* what() const throw()
    {
        return "This is an invalid area.";
    }

}; //Invalid character exception

#endif