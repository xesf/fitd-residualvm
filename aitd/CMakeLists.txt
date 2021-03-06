SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)


add_definitions(-DENTRY_CONFIG_IMPLEMENT_MAIN)

find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${ROOT_DIR}/bgfx/examples)
include_directories(${ROOT_DIR}/bgfx/include/)
include_directories(${ROOT_DIR}/bimg/include/)
include_directories(${ROOT_DIR}/bx/include/)
include_directories(${ROOT_DIR}/bgfx/examples/common/)
include_directories(${ROOT_DIR}/bgfx/3rdparty)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(IMGUI_SRC
  ${ROOT_DIR}/bgfx/3rdparty/ocornut-imgui/imgui_draw.cpp
  ${ROOT_DIR}/bgfx/3rdparty/ocornut-imgui/imgui_demo.cpp
  ${ROOT_DIR}/bgfx/3rdparty/ocornut-imgui/imgui.cpp
  ${ROOT_DIR}/bgfx/examples/common/imgui/imgui.cpp
  ${ROOT_DIR}/bgfx/examples/common/imgui/scintilla.cpp
  )

SET(BIMG_LIBRARIES
  ${ROOT_DIR}/bimg/.build/linux64_gcc/bin/libbimgRelease.a
  ${ROOT_DIR}/bimg/.build/linux64_gcc/bin/libbimg_encodeRelease.a
  ${ROOT_DIR}/bimg/.build/linux64_gcc/bin/libbimg_decodeRelease.a
  )

SET(BX_LIBRARIES
  ${ROOT_DIR}/bx/.build/linux64_gcc/bin/libbxRelease.a
  )

SET(BGFX_LIBRARIES
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libexample-commonRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libexample-glueRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbimg_decodeRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbimgRelease.a
  ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/libbxRelease.a
  )

SET(SHADER_COMPILER ${ROOT_DIR}/bgfx/.build/linux64_gcc/bin/shadercRelease)

add_library(AITDCore
  Shader.hpp
  Shader.cpp  
  InputManager.hpp
  InputManager.cpp
  ResourceManager.hpp
  ResourceManager.cpp
  Floor.hpp
  Floor.cpp
  Room.hpp
  Room.cpp
  RoomCamera.hpp
  RoomCamera.cpp
  Actor.hpp
  Actor.cpp
  World.hpp
  World.cpp
  ObjectManager.hpp
  ObjectManager.cpp
  ScriptingManager.hpp
  ScriptingManager.cpp
  GameSystems.hpp
  GameSystems.cpp
  AITDEngine.hpp
  AITDEngine.cpp
  ActorLoader.hpp
  ActorLoader.cpp)

target_link_libraries(AITDCore
  ${OpenCV_LIBS}
  ${SDL_LIBRARY}
  ParsingUtils
  GeometryUtils
  CollisionUtils
  GraphicsCore
  fitd
  pthread
  GL
  X11
  dl
  )

target_include_directories(AITDCore PUBLIC
  ${SDL_INCLUDE_DIR})


add_executable(aitd aitd.cpp)

target_link_libraries(aitd
  fitd  
  AITDCore
  EntitiesCore
  ${BGFX_LIBRARIES}
  ${SDL_LIBRARY}
  rt
  pthread
  GL
  X11
  dl)

target_include_directories(aitd PUBLIC
  ${SDL_INCLUDE_DIR})


add_executable(viewer_aitd viewer_aitd.cpp)
target_link_libraries(viewer_aitd
  fitd  
  AITDCore
  AITDViewerCore
  GraphicsCore
  ${BGFX_LIBRARIES}
  ${SDL_LIBRARY}
  rt
  pthread
  GL
  X11
  dl)


add_dependencies(aitd shaders)

add_subdirectory(graphics)
add_subdirectory(entities)
add_subdirectory(utils)
add_subdirectory(shaders)
add_subdirectory(tools)
#add_subdirectory(scripting)
