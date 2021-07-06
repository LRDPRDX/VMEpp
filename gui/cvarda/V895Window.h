#pragma once

#include "modules/V895.h"
#include "DeviceWindow.h"

static constexpr uint8_t N_CH = vmeplus::V895::GetChNumber(); // 16 channels
static constexpr uint8_t N_GROUPS = 2;
static constexpr uint8_t N_CH_IN_GROUP = N_CH / N_GROUPS; // 8 channels in group

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;


class V895Window : public DeviceWindow
{
    Q_OBJECT

    protected :
        QAction *fConnectAction, *fDisconnectAction, *fExitAction;
        QSpinBox *fThrSpin[N_CH];
        QCheckBox *fEnableCheck[N_CH];
        QSpinBox *fWidthSpin[N_GROUPS];
        QSpinBox *fMajLevelSpin;
        QPushButton *fTestButton;

    protected :
        void CreateActions();
        void CreateCentralWidget();

    public slots :
        void SendTest();  

    public :
        V895Window( uint32_t address, Controller *parent );
        ~V895Window();
};
