#ifndef ADVENTURE2016_GAMESTRINGS_H
#define ADVENTURE2016_GAMESTRINGS_H


#include <string>
#include "StringResourceImporter.hpp"

namespace GameStringKeys {
const std::string INVALID_DIR = "invalid-direction";
const std::string INVALID_TGT = "invalid-target";

const std::string PLAYERS_AREA = "players-list-area";
const std::string PLAYERS_WORLD = "players-list-world";
const std::string EXITS_AREA = "exits-list";

const std::string UNSPECIFIED_PLAYER = "unspecified-player";
const std::string UNSPECIFIED_TARGET = "unspecified-target";
const std::string UNSPECIFIED_EXIT = "unspecified-exit";

const std::string PLAYER = "player";
const std::string OBJECT = "object";

const std::string GLOBAL_CHANNEL = "global-tag";
const std::string AREA_CHANNEL = "area-tag";
const std::string PRIVATE_CHANNEL = "private-tag";
const std::string SERVER_NAME = "server-name";

const std::string LOGIN_Ok = "login-ok";
const std::string USER_NOT_FOUND = "user-not-found";
const std::string INVALID_CREDENTIALS = "invalid-cred";
const std::string USER_LOGGED_IN = "user-logged-in";

const std::string REG_OK = "reg-ok";
const std::string USER_EXISTS = "user-exists";
const std::string USERNAME_TOO_LONG = "username-too-long";
const std::string USERNAME_INVALD = "username-invalid";

const std::string PLAYER_KICKED = "player-kicked";
const std::string PLAYER_RESPAWNED = "player-respawned";

const std::string PLAYER_ENTER = "game-player-enter-area";
const std::string PLAYER_LEAVE = "game-player-leave-area";
const std::string PLAYER_ENTER_UNKNOWN = "game-player-enters";

const std::string COMBAT_ENGAGE = "combat-engage-issue";
const std::string COMBAT_ENGAGED = "combat-engage-receive";
const std::string PLAYER_ATTACKS = "combat-attack-issue";
const std::string PLAYER_ATTACKED = "combat-attack-receive";
const std::string ATTACK_ACTION_ID = "combat-attack-id";
const std::string COMBAT_VICTORY = "combat-result-victory";
const std::string COMBAT_LOSS = "combat-result-loss";

const std::string SPELL_LIST = "spell-list-header";
const std::string SPELL_INVALID_USAGE = "spell-invalid-usage";
const std::string SPELL_NO_MANA = "spell-insufficient-mana";
const std::string SPELL_UNKNOWN = "spell-unknown";
const std::string SPELL_OFFENSE_SELF = "spell-offense-self";
const std::string SPELL_OFFENSE_SOURCE = "spell-offense-source";
const std::string SPELL_OFFENSE_TARGET = "spell-offense-target";
const std::string SPELL_DEFENSE_SELF = "spell-defense-self";
const std::string SPELL_DEFENSE_SOURCE = "spell-defense-source";
const std::string SPELL_DEFENSE_TARGET = "spell-defense-target";
const std::string SPELL_GENERIC_SELF = "spell-generic-self";
const std::string SPELL_GENERIC_SOURCE = "spell-generic-source";
const std::string SPELL_GENERIC_TARGET = "spell-generic-target";

const std::string STATUS_EFFECT_END = "status-effect-expiry";
const std::string CHAR_STATUS = "character-status";

const std::string UP_DIRECTION = "up";
const std::string UP_RELATIVE_DIRECTION = "up-relative-direction";
const std::string DOWN_DIRECTION = "down";
const std::string DOWN_RELATIVE_DIRECTION = "down-relative-direction";
const std::string NORTH_DIRECTION = "north";
const std::string NORTH_RELATIVE_DIRECTION = "north-relative-direction";
const std::string SOUTH_DIRECTION = "south";
const std::string SOUTH_RELATIVE_DIRECTION = "south-relative-direction";
const std::string EAST_DIRECTION = "east";
const std::string EAST_RELATIVE_DIRECTION = "east-relative-direction";
const std::string WEST_DIRECTION = "west";
const std::string WEST_RELATIVE_DIRECTION = "west-relative-direction";

const std::string SOURCE_KEY = "$n";
const std::string TARGET_KEY = "$N";
const std::string PRONOUN_KEY = "$E";
const std::string POSSESSIVE_PRONOUN_KEY = "$S";
const std::string EFFECT_KEY = "$d";
const std::string ACTION_KEY = "$a";

const std::string GENERIC_PRONOUN = "they";
const std::string GENERIC_POSSESSIVE_PRONOUN = "their";

const std::string MESSAGE_SENDER_SERVER = "&yServer";
const std::string MESSAGE_SENDER_BATTLE = "&rBattle";

const std::string EDIT_IN_USE = "edit-in-use";
const std::string EDIT_CANCEL = "edit-cancel";
const std::string EDIT_ALREADY_EDITING = "edit-already-editing";
const std::string EDIT_NOT_EDITING = "edit-not-editing";
const std::string EDIT_AREA_HELP = "edit-area-help";
const std::string EDIT_INVALID_ARG = "edit-invalid-arg";
const std::string EDIT_SAVED = "edit-saved";
const std::string EDIT_SAVE_ERROT = "edit-save-error";
}

namespace ColorTag {
	const std::string BLACK   = "&l";
	const std::string RED     = "&r";
	const std::string GREEN   = "&g";
	const std::string YELLOW  = "&y";
	const std::string BLUE    = "&b";
	const std::string MAGENTA = "&m";
	const std::string CYAN    = "&c";
	const std::string WHITE   = "&w";
};

struct PlayerMessage {
	std::string text;
	std::string senderID;
	std::string color;
};

struct StringInfo {
	std::string sourceName;
	std::string targetName;
	int effect;
	std::string actionName;
};


class GameStrings {
public:
    static std::string get(const std::string& key) {
        return StringResourceImporter::getInstance().getString(key);
    }

	static std::string getFormatted(const std::string key, const StringInfo stringInfo) {
		std::string formatString = StringResourceImporter::getInstance().getString(key);
		replacePlaceholders(formatString, stringInfo);
		return formatString;
	}

	static std::string format(std::string message, const StringInfo stringInfo) {
		replacePlaceholders(message, stringInfo);
		return message;
	}

	static void replacePlaceholders(std::string &formatString, const StringInfo &stringInfo) {
		std::string::size_type index = 0;
		if ((index = formatString.find(GameStringKeys::SOURCE_KEY, 0)) != std::string::npos) {
			formatString.replace(index, GameStringKeys::SOURCE_KEY.size(), stringInfo.sourceName);
		}
		if ((index = formatString.find(GameStringKeys::TARGET_KEY, 0)) != std::string::npos) {
			formatString.replace(index, GameStringKeys::TARGET_KEY.size(), stringInfo.targetName);
		}
		if ((index = formatString.find(GameStringKeys::POSSESSIVE_PRONOUN_KEY, 0)) != std::string::npos) {
			formatString.replace(index, GameStringKeys::POSSESSIVE_PRONOUN_KEY.size(), GameStringKeys::GENERIC_POSSESSIVE_PRONOUN);
		}
		if ((index = formatString.find(GameStringKeys::PRONOUN_KEY, 0)) != std::string::npos) {
			formatString.replace(index, GameStringKeys::PRONOUN_KEY.size(), GameStringKeys::GENERIC_PRONOUN);
		}
		if ((index = formatString.find(GameStringKeys::EFFECT_KEY, 0)) != std::string::npos) {
			formatString.replace(index, GameStringKeys::EFFECT_KEY.size(), std::to_string(stringInfo.effect));
		}
		if ((index = formatString.find(GameStringKeys::ACTION_KEY, 0)) != std::string::npos) {
			formatString.replace(index, GameStringKeys::ACTION_KEY.size(), stringInfo.actionName);
		}
	}
};




#endif //ADVENTURE2016_GAMESTRINGS_H
