#pragma once

#include "modules/V6533N.h"
#include "DeviceWindow.h"


class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;


class V6533NWindow : public DeviceWindow
{
    Q_OBJECT

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = vmeplus::V6533N::GetChNumber(); // 6 channels

    protected :
        // Widgets
        QSpinBox    *fVoltSpin[N_CH], *fCurSpin[N_CH], *fUpSpin[N_CH], *fDownSpin[N_CH], *fSWMaxSpin[N_CH];
        QComboBox   *fOffCombo[N_CH];
        QPushButton *fOnButton, *fOffButton, *fKillButton;

    protected :
        // Creational member functions
        void CreateActions();
        void CreateCentralWidget();

    public :
        void Program() override;

        V6533NWindow( uint32_t address, Controller *parent );
        ~V6533NWindow();
};
