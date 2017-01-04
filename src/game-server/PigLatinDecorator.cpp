#include <game/protocols/ResponseMessage.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include "PigLatinDecorator.hpp"
#include "game/GameModel.hpp"
#include "Controller.hpp"
#include "PigLatinTranslator.hpp"
#include <random>

namespace {
std::random_device rd;
std::mt19937 mt{rd()};
std::uniform_int_distribution<int> distribution{1, 50};
auto rainbow = std::bind(distribution, mt);
}

std::vector<MessageInfo> PigLatinDecorator::buildMessages() const {
    auto messages = MessageBuilderDecorator::buildMessages();
    for(auto& msg : messages) {
        if (controller.playerMap.right.find(msg.client) == controller.playerMap.right.end()) {
            continue;
        }

        protocols::ResponseMessage responseMessage = msg.message;
        switch (responseMessage.header) {
            case protocols::ResponseHeader::DISPLAY_MESSAGE_RESPONSE: {
                auto displayMsg = protocols::readDisplayResponseMessage(responseMessage);

                auto playername = controller.getPlayerID(msg.client);
                auto character = gameModel.getCharacterByID(playername);
                if (character->hasStatusEffect(StatusType::PIG_LATIN)) {
                    displayMsg.message = translateToPigLatin(displayMsg.message);
                }

                //should be somewhere else but chance of text being rainbow
                if (rainbow() == 1) {
                    displayMsg.message = "&v" + displayMsg.message;
                }

                auto editedResponse = protocols::createDisplayResponseMessage(displayMsg);
                msg.message = editedResponse;
                break;
            }
            default:
                break;
        }
    }

    return messages;
}
