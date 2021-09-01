#pragma once

#include <QString>

namespace style 
{
    namespace button
    {
        static const QString bad =      "QPushButton {background-color:#ff6bff; color:#000000}"
                                        "QPushButton:disabled {background-color:#ffa3ff;}";

        static const QString neutral =  "QPushButton {background-color:#00cc88; color:#000000}"
                                        "QPushButton:disabled {background-color:#a8ffe8; color:#ffffff}";

        static const QString good =     "QPushButton {background-color:#0000ff; color:#ffffff}"
                                        "QPushButton:disabled {background-color:#a3a3ff;}";
    }

    namespace led
    {
        static const QString on = "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 #8888ff, stop:1 #88ffff);";
        static const QString off = "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:0.92, y2:0.988636, stop:0 #88c3ff, stop:0.869347 #8888ff);";
    }
}
