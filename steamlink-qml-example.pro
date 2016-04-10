CONFIG += marvell
CONFIG += debug

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_LFLAGS += '-Wl,-rpath,\'/home/steam/lib\''

# Add more folders to ship with the application, here
folder_01.source = qml/steamlink-qml-example
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Installation path
# target.path =

marvell {
	INCLUDEPATH += \
		$(MARVELL_ROOTFS)/usr/include/SDL2 \
}

removesteamlink.commands = $(RM) -r steamlink
distclean.depends = removesteamlink
QMAKE_EXTRA_TARGETS += distclean removesteamlink

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
