
add_executable(game-client
        GameClient.cpp
        MainMenuWindow.cpp MainMenuWindow.hpp
        Window.hpp
        Application.cpp Application.hpp
        AuthenticationWindow.cpp AuthenticationWindow.hpp
        ChatWindow.cpp ChatWindow.hpp
        StrColorizer.cpp StrColorizer.hpp
        WorldBuildingWindow.cpp WorldBuildingWindow.hpp
        CombatWindow.hpp CombatWindow.cpp
        CharacterAsciis.hpp
        StartupSplashScr.cpp StartupSplashScr.hpp ArtWindow.cpp ArtWindow.hpp)

set_target_properties(game-client
        PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        PREFIX ""
        )

find_package(Boost 1.58 COMPONENTS system REQUIRED)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(
        ${Boost_INCLUDE_DIR}
        ${CURSES_INCLUDE_DIR}
)

target_link_libraries(game-client
        networking
        protocols
        game
        ${Boost_LIBRARIES}
        ${CURSES_LIBRARIES}
        menu
        ${CMAKE_THREAD_LIBS_INIT}
        glog
        ascii-converter
        )

install(TARGETS game-client
        RUNTIME DESTINATION bin
        )

