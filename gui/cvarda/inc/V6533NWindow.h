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

using namespace vmeplus;


class V6533NWindow : public DeviceWindow
{
    Q_OBJECT

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = V6533N::GetChNumber(); // 6 channels

    protected :
        // Widgets
        QSpinBox    *fVoltSpin[N_CH], *fCurSpin[N_CH], *fUpSpin[N_CH], *fDownSpin[N_CH], *fSWMaxSpin[N_CH];
        QComboBox   *fOffCombo[N_CH];
        QPushButton *fOnButton, *fOffButton, *fKillButton;

    protected :
        // Creational member functions
        void CreateActions();
        void CreateCentralWidget();

    public slots :
        void Program() override;
        void ReadConfig() override;

        UConfig<V6533N> CollectConfig();
        void SpreadConfig( const UConfig<V6533N>& cfg );

    public :
        V6533NWindow( uint32_t address, V2718Window *parent );
        ~V6533NWindow();
};
