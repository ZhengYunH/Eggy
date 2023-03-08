cmake_minimum_required(VERSION 3.10) 

# set the project name 
set(PROJECT_NAME EggyEngine)
project(${PROJECT_NAME})

# set character set to unicode
ADD_DEFINITIONS(-DUNICODE)

# preset common directory path
set(SOURCE_CODE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source)
set(EXTERNAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Engine/External)
set(LIBRARY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Library)

# preset layer in source core
set(RESOURCE_LAYER_ROOT ${SOURCE_CODE_ROOT}/Resource)
set(CORE_LAYER_ROOT ${SOURCE_CODE_ROOT}/Core)
set(PLATFORM_LAYER_ROOT ${SOURCE_CODE_ROOT}/Drivers)


# get all header / source files
file (GLOB_RECURSE HEADER_LIST "${SOURCE_CODE_ROOT}/*.h")
file (GLOB_RECURSE SOURCE_LIST "${SOURCE_CODE_ROOT}/*.cpp")

# add executable
add_executable(EggyEngine ${SOURCE_LIST} ${HEADER_LIST})

# group source / header files
source_group(TREE ${SOURCE_CODE_ROOT} PREFIX "Source Files" FILES ${SOURCE_LIST})
source_group(TREE ${SOURCE_CODE_ROOT} PREFIX "Header Files" FILES ${HEADER_LIST})


# additional include directories
set(
    ADDITIONAL_INCLUDE_DIR
    ${SOURCE_CODE_ROOT}
    ${CORE_LAYER_ROOT}
    ${RESOURCE_LAYER_ROOT}
    ${PLATFORM_LAYER_ROOT}
    ${EXTERNAL_ROOT}/SPIRV-Reflect
    ${EXTERNAL_ROOT}/FBX/Include
    ${EXTERNAL_ROOT}/tinyobjloader
    ${EXTERNAL_ROOT}/glm
    ${EXTERNAL_ROOT}/glfw/Include
    ${EXTERNAL_ROOT}/VulkanSDK/Include
    ${EXTERNAL_ROOT}/stb-master
    ${EXTERNAL_ROOT}/boost/Include
    ${EXTERNAL_ROOT}/imgui
)
target_include_directories(EggyEngine PUBLIC ${ADDITIONAL_INCLUDE_DIR})

# additional library directories
set(
    ADDITIONAL_LIB_DIR
    ${EXTERNAL_ROOT}/boost/Lib
    ${EXTERNAL_ROOT}/glfw/Lib
    ${EXTERNAL_ROOT}/VulkanSDK/Lib
    ${EXTERNAL_ROOT}/FBX/Lib
)
target_link_directories(${PROJECT_NAME} PUBLIC ${ADDITIONAL_LIB_DIR})

# addtional dependencies
set(
    ADDITIONAL_DEP_LIB
    libfbxsdk.lib
    glfw3.lib
    vulkan-1.lib
# dx11
    d3d11.lib
    dxgi.lib
    dxguid.lib
    D3DCompiler.lib
    winmm.lib
)
target_link_libraries(EggyEngine ${ADDITIONAL_DEP_LIB})

# other feature
target_compile_features(EggyEngine PRIVATE cxx_std_20)


## Plugins
set(SOURCE_PLUGIN_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Plugins)

### ShaderConverter
set(PLUGIN_SOURCE_CODE_ROOT ${SOURCE_PLUGIN_ROOT}/ShaderConverter/Source)


# get all header / source files
file (GLOB_RECURSE HEADER_LIST "${PLUGIN_SOURCE_CODE_ROOT}/*.h")
file (GLOB_RECURSE SOURCE_LIST "${PLUGIN_SOURCE_CODE_ROOT}/*.cpp")

# group source / header files
source_group(TREE ${PLUGIN_SOURCE_CODE_ROOT} PREFIX "Source Files" FILES ${SOURCE_LIST})
source_group(TREE ${PLUGIN_SOURCE_CODE_ROOT} PREFIX "Header Files" FILES ${HEADER_LIST})

add_executable(ShaderConverter ${SOURCE_LIST} ${HEADER_LIST})


set(
    PLUGIN_ADDITIONAL_INCLUDE_DIR
    ${ADDITIONAL_INCLUDE_DIR}
)
target_include_directories(ShaderConverter PUBLIC ${PLUGIN_ADDITIONAL_INCLUDE_DIR})
target_compile_features(ShaderConverter PRIVATE cxx_std_20)



set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Eggy")

# add_subdirectory(${LIBRARY_ROOT}/ShaderConductor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})