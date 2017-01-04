#include "CommandHandle.hpp"

CommandHandle::CommandHandle(const std::string& id, Command& command) : id{id}, command{command} {}

Command& CommandHandle::getCommand() {
    return command;
}

const std::string& CommandHandle::getId() const {
    return id;
}

const std::string& CommandHandle::getDescription() const {
    return description;
}

const std::string& CommandHandle::getUsage() const {
    return usage;
}

const std::vector<std::string>& CommandHandle::getInputBindings() const {
    return inputBindings;
}

void CommandHandle::setDescription(const std::string& description) {
    this->description = description;
}

void CommandHandle::setUsage(const std::string& usage) {
    this->usage = usage;
}

void CommandHandle::addInputBinding(const std::string& inputBinding) {
    inputBindings.push_back(inputBinding);
}

void CommandHandle::setRole(PlayerRole role) {
    this->role = role;
}

PlayerRole CommandHandle::getRole() const {
    return role;
}
