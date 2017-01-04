#include <gtest/gtest.h>
#include <../src/game-server/commands/CommandConfig.hpp>

class TestCommand : public Command {
public:
    virtual std::unique_ptr<MessageBuilder>
    execute(const gsl::span<std::string, -1> arguments, const PlayerInfo& player) override {
        return nullptr;
    }
};

class CommandConfigTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        root["commands"] = commandNode;
    }

    YAML::Node root;
    YAML::Node commandNode;
    TestCommand testCommand;

    void createCommandConfig(const std::string& name, const std::string& desc,
                             const std::string& usage, const std::vector<std::string>& bindings) {
        YAML::Node configNode;
        configNode["desc"] = desc;
        configNode["usage"] = usage;
        configNode["role"] = "user";
        for (const auto& b : bindings) {
            configNode["bindings"].push_back(b);
        }

        commandNode["command-" + name] = configNode;
    };
};



TEST_F(CommandConfigTest, CreateFromNode) {
    const std::string id = "test";
    const std::string desc = "desc";
    const std::string usage = "usage";
    const std::string binding1 = "bind1";

    createCommandConfig(id, desc, usage, {binding1});
    CommandConfig config{root};

    auto res = config.createInputBindingsForCommand(id, testCommand);

    EXPECT_TRUE(res.getId()  == id);
    EXPECT_TRUE(res.getDescription() == desc);
    EXPECT_TRUE(res.getUsage() == usage);
    EXPECT_TRUE(res.getInputBindings().size() == 1);
    EXPECT_TRUE(res.getInputBindings()[0] == binding1);

    EXPECT_TRUE(res.getDescription() == config.getCommandDescription(id));
    EXPECT_TRUE(res.getUsage() == config.getCommandUsage(id));
    EXPECT_TRUE(res.getInputBindings() == config.getCommandInputBindings(id));
}

TEST_F(CommandConfigTest, InvalidCommandId) {
    const std::string id = "notexist";
    CommandConfig c{root};

    EXPECT_DEATH(c.createInputBindingsForCommand(id, testCommand), ".*Command configuration doesn't exist for: " + id);
}