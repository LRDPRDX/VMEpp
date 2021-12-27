#pragma once

#include "modules/V895.h"

#include "SlaveWindow.h"


class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;

using namespace vmepp;

class V895Window : public SlaveWindow
{
    Q_OBJECT

    protected :
        static constexpr uint8_t N_CH = V895::GetChNumber(); // 16 channels
        static constexpr uint8_t N_GROUPS = 2;
        static constexpr uint8_t N_CH_IN_GROUP = N_CH / N_GROUPS; // 8 channels in group

    protected :
        QSpinBox    *fThrSpin[N_CH];
        QCheckBox   *fEnableCheck[N_CH];
        QSpinBox    *fWidthSpin[N_GROUPS];
        QSpinBox    *fMajLevelSpin;
        QPushButton     *fTestButton;

    protected :
        void CreateActions();
        void CreateCentralWidget();

    public slots :
        void SendTest();

        QVariant CollectConfig() override;
        void SpreadConfig( const QVariant& qConfig ) override;

        void WriteConfig() override;
        void ReadConfig() override;

    public :
        V895Window( uint32_t address, V2718Window *parent );
        ~V895Window() override = default;
};