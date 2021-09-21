#pragma once

#include "modules/V895.h"

#include "DeviceWindow.h"


class QAction;
class SButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;

using namespace vmeplus;

class V895Window : public DeviceWindow
{
    Q_OBJECT

    protected :
        static constexpr uint8_t N_CH = vmeplus::V895::GetChNumber(); // 16 channels
        static constexpr uint8_t N_GROUPS = 2;
        static constexpr uint8_t N_CH_IN_GROUP = N_CH / N_GROUPS; // 8 channels in group

    protected :
        QSpinBox    *fThrSpin[N_CH];
        QCheckBox   *fEnableCheck[N_CH];
        QSpinBox    *fWidthSpin[N_GROUPS];
        QSpinBox    *fMajLevelSpin;
        SButton     *fTestButton;

    protected :
        void CreateActions();
        void CreateCentralWidget();

    public slots :
        void SendTest();

        QVariant CollectConfig() override;
        void SpreadConfig( const UConfig<V895>& cfg );

        void Program() override;
        void ReadConfig() override;

    public :
        V895Window( uint32_t address, V2718Window *parent );
        ~V895Window();
};
