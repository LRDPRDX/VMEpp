#pragma once

#include <QString>

namespace style 
{
    namespace button
    {
        static const QString bad =      "QPushButton {background-color:#ff0000; color:#ffffff}"
                                        "QPushButton:disabled {background-color:#ff8888;}";

        static const QString neutral =  "QPushButton {background-color:#00cc88; color:#000000}"
                                        "QPushButton:disabled {background-color:#a8ffe8; color:#ffffff}";

        static const QString good =     "QPushButton {background-color:#0000ff; color:#ffffff}"
                                        "QPushButton:disabled {background-color:#8888ff;}";
    }

    namespace led
    {
        static const QString on = "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 #00ffa9, stop:1 #007a51);";
        static const QString off = "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:0.92, y2:0.988636, stop:0 #007a51, stop:0.869347 #003322);";
    }
}
