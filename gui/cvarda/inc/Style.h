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
}
