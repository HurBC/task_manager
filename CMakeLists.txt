cmake_minimum_required(VERSION 3.14)

# Establecer el nombre del proyecto y la versión
project(TaskManager VERSION 1.0)

# Especificar el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Exportar comandos de compilación para clangd (LSP)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- MAGIA DE JSON ---
# Descargar e integrar nlohmann/json automáticamente
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
# ---------------------

# Añadir el ejecutable
add_executable(TaskManager src/main.cpp src/tasks/task.cpp src/cli/tm_cli.cpp)

# --- ENLAZAR LA LIBRERÍA ---
# Esto le dice al compilador dónde están los headers y actualiza tu compile_commands.json
target_link_libraries(TaskManager PRIVATE nlohmann_json::nlohmann_json)
