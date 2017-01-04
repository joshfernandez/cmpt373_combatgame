add_library(game Area.cpp
            Character.cpp
            Object.cpp
            GameModel.cpp
            NPCScripts.cpp
            Inventory.cpp
            Resets.cpp
            GameDataImporter.cpp
            Attributes.cpp
            combat/CombatManager.cpp
            combat/CombatInstance.cpp
            combat/CharacterInstance.cpp
            combat/CombatAction.cpp
            Spell.cpp
            StatusEffect.cpp
            ExpressionExtractor.hpp
            YmlSerializer.cpp)

find_package(Boost 1.58 COMPONENTS regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(game
        ${Boost_LIBRARIES}
        glog
        expr-evaluator)

set_property(TARGET game PROPERTY CXX_STANDARD 14)

install(TARGETS game
        ARCHIVE DESTINATION lib
        )

add_subdirectory(protocols)