cmake_minimum_required(VERSION 3.10)
project(16Games LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-all>=2.5)
find_package(Box2D)

# Set SFML compile flags
add_compile_options(${SFML_CFLAGS})
link_directories(${SFML_LIBRARY_DIRS})

# Function to create game executable
function(add_game GAME_NAME GAME_DIR)
    # Get all cpp files in the directory
    file(GLOB_RECURSE GAME_SOURCES "${GAME_DIR}/*.cpp" "${GAME_DIR}/*.hpp")
    
    if(GAME_SOURCES)
        add_executable(${GAME_NAME} ${GAME_SOURCES})
        
        # Link SFML libraries
        target_link_libraries(${GAME_NAME} ${SFML_LIBRARIES})
        if("${GAME_NAME}" STREQUAL "volleyball" AND Box2D_FOUND)
            target_link_libraries(${GAME_NAME} Box2D::Box2D)
        endif()
        
        # Set output directory
        set_target_properties(${GAME_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/games/${GAME_NAME}"
        )
        
        # Copy assets if they exist - each game gets its own asset directory
        if(EXISTS "${CMAKE_SOURCE_DIR}/${GAME_DIR}/images")
            file(COPY "${CMAKE_SOURCE_DIR}/${GAME_DIR}/images/" 
                 DESTINATION "${CMAKE_BINARY_DIR}/games/${GAME_NAME}/images")
        endif()
        
        if(EXISTS "${CMAKE_SOURCE_DIR}/${GAME_DIR}/files")
            file(COPY "${CMAKE_SOURCE_DIR}/${GAME_DIR}/files/" 
                 DESTINATION "${CMAKE_BINARY_DIR}/games/${GAME_NAME}/files")
        endif()
        
        if(EXISTS "${CMAKE_SOURCE_DIR}/${GAME_DIR}/fonts")
            file(COPY "${CMAKE_SOURCE_DIR}/${GAME_DIR}/fonts/" 
                 DESTINATION "${CMAKE_BINARY_DIR}/games/${GAME_NAME}/fonts")
        endif()
        
        if(EXISTS "${CMAKE_SOURCE_DIR}/${GAME_DIR}/fonts")
            file(COPY "${CMAKE_SOURCE_DIR}/${GAME_DIR}/fonts/" 
                 DESTINATION "${CMAKE_BINARY_DIR}/games/${GAME_NAME}/fonts")
        endif()
        
        # Add custom target to run the game
        add_custom_target(run_${GAME_NAME}
            COMMAND ${CMAKE_BINARY_DIR}/games/${GAME_NAME}/${GAME_NAME}
            DEPENDS ${GAME_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/games/${GAME_NAME}
            COMMENT "Running ${GAME_NAME}"
        )
        
        message(STATUS "Added game: ${GAME_NAME}")
    else()
        message(WARNING "No source files found for ${GAME_NAME} in ${GAME_DIR}")
    endif()
endfunction()

# Add all games
add_game(tetris "01  Tetris")
add_game(doodle_jump "02  Doodle Jump")
add_game(arkanoid "03  Arkanoid")
add_game(snake "04 Snake")
add_game(minesweeper "05 Minesweeper")
add_game(fifteen_puzzle "06 Fifteen-Puzzle")
add_game(racing "07 Racing (Top Down)")
add_game(outrun "08 Outrun")
add_game(xonix "09 Xonix")
add_game(bejeweled "10 Bejeweled")
add_game(netwalk "11 NetWalk (Pipe Puzzle)")
add_game(mahjong "12 Mahjong Solitaire")
add_game(tron "13 Tron")
add_game(chess "14 Chess")
add_game(volleyball "15 Volleyball")
add_game(asteroids "16 Asteroids")

# Create a target to build all games
add_custom_target(all_games DEPENDS 
    tetris doodle_jump arkanoid snake minesweeper fifteen_puzzle
    racing outrun xonix bejeweled netwalk mahjong tron chess volleyball asteroids
)

# Print build instructions
message(STATUS "")
message(STATUS "🎮 16 Games in C++ - Build Instructions:")
message(STATUS "")
message(STATUS "To build all games:     make all_games")
message(STATUS "To build specific game: make <game_name>")
message(STATUS "To run a game:          make run_<game_name>")
message(STATUS "")
message(STATUS "Available games:")
message(STATUS "  tetris, doodle_jump, arkanoid, snake, minesweeper")
message(STATUS "  fifteen_puzzle, racing, outrun, xonix, bejeweled")
message(STATUS "  netwalk, mahjong, tron, chess, volleyball, asteroids")
message(STATUS "")