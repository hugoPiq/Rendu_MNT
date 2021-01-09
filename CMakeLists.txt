cmake_minimum_required(VERSION 2.6) # compatibilités de CMake
project(create_raster) # nom du projet 

set(CMAKE_BUILD_TYPE debug) # compilation en mode debug
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11") # options

file( # liste des fichiers à compiler
  GLOB_RECURSE # recherche récursive
  source_files # fichiers listés dans une variable source_files
  src/*.cpp # forme des fichiers à rechercher
)

add_executable( # création de l’exécutable binaire du projet
  create_raster # nom du binaire
  main.cpp # liste des fichiers compilés pour le linking
  point.cpp
  tool.cpp
) 



target_link_libraries(create_raster proj)
