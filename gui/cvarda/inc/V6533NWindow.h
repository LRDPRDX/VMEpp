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
class QLineEdit;

class QLedIndicatorWithLabel;

class Monitor;

using namespace vmeplus;

class V6533NWindow : public DeviceWindow
{
    Q_OBJECT

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = V6533N::GetChNumber(); // 6 channels

    protected :
        // Widgets
        Monitor     *fMonitor;

        QSpinBox    *fVoltSpin[N_CH], *fCurSpin[N_CH], *fUpSpin[N_CH], *fDownSpin[N_CH], *fSWMaxSpin[N_CH];
        QComboBox   *fOffCombo[N_CH];
        QPushButton *fOnButton, *fOffButton, *fKillButton;

    protected :
        // Creational member functions
        void CreateActions();
        void CreateCentralWidget();
        void CreateDockWidget();

    public slots :
        void Program() override;
        void ReadConfig() override;
        void UpdateMonitor();

        UConfig<V6533N> CollectConfig();
        void SpreadConfig( const UConfig<V6533N>& cfg );

    public :
        V6533NWindow( uint32_t address, V2718Window *parent );
        ~V6533NWindow();
};

class Monitor : public QWidget
{
    Q_OBJECT

    protected :
        static constexpr uint8_t N_CH = V6533N::GetChNumber(); // 6 channels

        V6533NWindow* fContainer;

    protected :
        QLineEdit               *fVoltText, *fCurText;
        QLedIndicatorWithLabel  *fAlarmLED[N_CH], *fPowFailLED, *fOvPowLED, *fMaxVUncLED, *fMaxIUncLED;
        QPushButton             *fUpdateButton, *fStartButton, *fStopButton;
        
    public :
        Monitor( V6533NWindow* parentWindow, QWidget* parent = nullptr );
        virtual ~Monitor();

        void CreateGeneralFrame();

        friend void V6533NWindow::UpdateMonitor();
};
