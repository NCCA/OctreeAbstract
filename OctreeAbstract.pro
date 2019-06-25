# This specifies the exe name
TARGET=OctreeAbstract
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
	cache()
	DEFINES +=QT5BUILD
}
# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/NGLScene.cpp \
					$$PWD/src/ParticleOctree.cpp \
					$$PWD/src/main.cpp \
					$$PWD/src/Particle.cpp \
					$$PWD/src/Scene.cpp
# same for the .h files
HEADERS+= $$PWD/include/AbstractOctree.h \
					$$PWD/include/Particle.h \
					$$PWD/include/Scene.h \
					$$PWD/include/NGLScene.h \
					$$PWD/include/ParticleOctree.h
# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= shaders/*.glsl \
							README.md
# were are going to default to a console app
CONFIG += console
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or not easiest way is to check out against current

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        !win32:message("including $HOME/NGL")
        !win32:include($(HOME)/NGL/UseNGL.pri)
        win32:include($(HOMEDRIVE)\$(HOMEPATH)\NGL\UseNGL.pri)
}
else{ # note brace must be here
	message("Using custom NGL location")
	include($(NGLDIR)/UseNGL.pri)
}
