#!/usr/bin/env bash
qmake-qt5 -project "QT += widgets" "LIBS += -lVMEplusM -lVMEplus" "INCLUDEPATH += ../../inc"
qmake-qt5
make
