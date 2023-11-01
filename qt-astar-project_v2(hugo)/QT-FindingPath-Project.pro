RESOURCES += images.qrc
RC_ICONS = ./Images/PathFinder.ico
HEADERS += mainwindow.h view.h \
    graph.h \
    vertex.h
SOURCES += main.cpp \
    graph.cpp \
    vertex.cpp
SOURCES += mainwindow.cpp view.cpp

QT += widgets
qtHaveModule(printsupport): QT += printsupport

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/chip
INSTALLS += target



