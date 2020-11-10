cmake_minimum_required(VERSION 3.12)
#-------------------------------------------------------------------------------------------
# I'm going to use vcpk in most cases for our install of 3rd party libs
# this is going to check the environment variable for CMAKE_TOOLCHAIN_FILE and this must point to where
# vcpkg.cmake is in the University this is set in your .bash_profile to
# export CMAKE_TOOLCHAIN_FILE=/public/devel/2020/vcpkg/scripts/buildsystems/vcpkg.cmake
#-------------------------------------------------------------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# Name of the project
project(OctreeAbstractBuild)
# This is the name of the Exe change this and it will change everywhere
set(TargetName OctreeAbstract)
# Instruct CMake to run moc automatically when needed (Qt projects only)
set(CMAKE_AUTOMOC ON)
# find Qt libs
find_package(Qt5Widgets)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
# use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
# Set the name of the executable we want to build
add_executable(${TargetName})

# Add NGL include path
include_directories(include $ENV{HOME}/NGL/include)
target_sources(${TargetName} PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp  
			${PROJECT_SOURCE_DIR}/src/NGLScene.cpp  
			${PROJECT_SOURCE_DIR}/src/Particle.cpp  
			${PROJECT_SOURCE_DIR}/src/ParticleOctree.cpp  
			${PROJECT_SOURCE_DIR}/src/Scene.cpp
			${PROJECT_SOURCE_DIR}/include/NGLScene.h  
			${PROJECT_SOURCE_DIR}/include/AbstractOctree.h  
			${PROJECT_SOURCE_DIR}/include/Particle.h  
			${PROJECT_SOURCE_DIR}/include/ParticleOctree.h  
			${PROJECT_SOURCE_DIR}/include/Scene.h
)
add_compile_definitions(ADDLARGEMODELS)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# see what platform we are on and set platform defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	find_library(MACGL OpenGL)
	find_package(IlmBase CONFIG REQUIRED)
	find_package(OpenEXR CONFIG REQUIRED)
	set(EXTRALIBS  ${MACGL})
	target_link_libraries(${TargetName} PRIVATE  IlmBase::Iex IlmBase::Half IlmBase::Imath IlmBase::IexMath)
	target_link_libraries(${TargetName} PRIVATE OpenEXR::IlmImf OpenEXR::IlmImfUtil OpenEXR::IlmImfConfig)
	add_definitions(-DGL_SCILENCE_DEPRICATION)
	set ( PROJECT_LINK_LIBS ${MACGL} NGL)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set ( PROJECT_LINK_LIBS -lGL NGL)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	include_directories(include $ENV{HOMEDRIVE}/$ENV{HOMEPATH}/NGL/include)
	link_directories($ENV{HOMEDRIVE}/$ENV{HOMEPATH}/NGL/lib)	
	target_link_libraries(${TargetName} PRIVATE $ENV{HOMEDRIVE}/$ENV{HOMEPATH}/NGL/lib/NGL.lib)
	add_compile_definitions(_USE_MATH_DEFINES)
	add_compile_definitions(NOMINMAX)
	target_compile_definitions(${TargetName} PRIVATE /wd4251)
	# Need to define this when building shared library or suffer dllimport errors
	add_compile_definitions(BUILDING_DLL)
endif()

# now add NGL specific values
target_link_directories(${TargetName} PRIVATE $ENV{HOME}/NGL/lib )

# add exe and link libs that must be after the other defines
target_link_libraries(${TargetName} PRIVATE OpenImageIO::OpenImageIO OpenImageIO::OpenImageIO_Util)
target_link_libraries(${TargetName} PRIVATE ${PROJECT_LINK_LIBS}  Qt5::Widgets fmt::fmt-header-only freetype )


add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
    $<TARGET_FILE_DIR:${TargetName}>/fonts
) 