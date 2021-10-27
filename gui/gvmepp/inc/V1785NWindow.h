#pragma once

#include "modules/V1785N.h"
#include "SlaveWindow.h"

class QAction;
class SButton;
class QComboBox;
class QMenu;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;
class SLedIndicatorWithLabel;
class QLineEdit;

using namespace vmepp;


class V1785NWindow : public SlaveWindow
{
    Q_OBJECT

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = V1785N::GetChNumber(); // 6 channels

    private :
        // Creational member functions
        void CreateMenu();
        void CreateCentralWidget();
        void CreateGeneralTab();
        void CreateChannelsTab();
        void CreatePlotTab();

    private :
        QTabWidget *fMainTab, *fChannelTab;

        QCheckBox *fZeroSuppCheck, *fOverSuppCheck;
        QComboBox *fZeroSuppTypeCombo;

        QComboBox *fIRQLevelCombo;
        QSpinBox  *fIRQVectorSpin, *fIRQEventsSpin;

        QCheckBox *fChEnLowCheck[N_CH];
        QSpinBox  *fChThrLowSpin[N_CH];

        QCheckBox *fChEnHighCheck[N_CH];
        QSpinBox  *fChThrHighSpin[N_CH];

    public slots :
        void WriteConfig() override;
        void ReadConfig() override;

        QVariant CollectConfig() override;
        void SpreadConfig( const QVariant& qConfig ) override;

    public :
        V1785NWindow( uint32_t address, V2718Window *parent );
        ~V1785NWindow() override = default;
};// V1785NWindow
