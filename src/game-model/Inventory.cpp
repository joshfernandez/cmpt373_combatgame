#include <iostream>
#include "../../include/game/Inventory.hpp"

Inventory::Inventory() {
    inventory.reserve(MAX_ITEMS);
}

std::string Inventory::getString() const {
    std::string inventoryString = "";

    if (!inventory.empty()) {
        for (const auto& item : inventory) {
            inventoryString += item.first + " x" + std::to_string(item.second) + "\n";
        }
    } else {
        inventoryString = "Your inventory is empty.";
    }

    return inventoryString;
}

std::unordered_map<std::string, int> Inventory::get() const {
    return inventory;
}

int Inventory::getSize() const {
    return inventory.size();
}

void Inventory::addItem(const std::string &item) {
    bool inventoryNotFull = getSize() < MAX_ITEMS;
    bool itemNotFullyStacked = inventory[item] < MAX_PER_STACKED_ITEM;

    if (inventoryNotFull && itemNotFullyStacked) {
        inventory[item]++;
    }
}

void Inventory::removeItem(const std::string &item) {
    bool stackedItem = inventory[item] > 1;

    if (stackedItem) {
        inventory[item]--;
    } else {
        inventory.erase(item);
    }
}

void Inventory::removeAllItems() {
    inventory.clear();
}
