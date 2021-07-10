#!/usr/bin/env bash
qmake -project "QT += widgets" "LIBS += -lVMEplusM -lVMEplus" "INCLUDEPATH += ../../inc"
qmake
make -j${nproc}
