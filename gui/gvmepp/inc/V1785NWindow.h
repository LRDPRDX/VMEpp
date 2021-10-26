#pragma once

#include "modules/V1785N.h"
#include "SlaveWindow.h"

using namespace vmepp;


class V1785NWindow : public SlaveWindow
{
    private :
        // Creational member functions
        void CreateMenu();
        void CreateCentralWidget();
        void CreateDockWidget();

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = V1785N::GetChNumber(); // 6 channels
        
};
