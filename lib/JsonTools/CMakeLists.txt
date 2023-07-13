add_library(user user.cpp include/user.hpp)
target_link_libraries(user PRIVATE nlohmann_json::nlohmann_json)

set_property(TARGET user PROPERTY CXX_STANDARD 17)

target_include_directories(user PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_library(progress progress.cpp include/progress.hpp)
target_link_libraries(progress PRIVATE nlohmann_json::nlohmann_json)

set_property(TARGET progress PROPERTY CXX_STANDARD 17)

target_include_directories(progress PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_library(file_load file_load.cpp include/file_load.hpp)
target_link_libraries(file_load PRIVATE nlohmann_json::nlohmann_json)

set_property(TARGET file_load PROPERTY CXX_STANDARD 17)

target_include_directories(file_load PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS user progress file_load)