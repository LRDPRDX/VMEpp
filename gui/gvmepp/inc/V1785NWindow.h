#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QMetaType>
#include <QVector>

#include "qwt_samples.h"

#include "modules/V1785N.h"
#include "SlaveWindow.h"

class QAction;
class QPushButton;
class QComboBox;
class QMenu;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;
class SLedIndicatorWithLabel;
class QLineEdit;
class QTimer;

class QwtPlot;
class QwtPlotHistogram;
class QwtPlotMarker;

using namespace vmepp;

Q_DECLARE_METATYPE(QVector<QwtIntervalSample>)
Q_DECLARE_METATYPE(VBuffer);


class V1785NProcessor : public QObject
{
    Q_OBJECT

    private :
        size_t                      fBins;
        QVector<QwtIntervalSample>  fData;
        size_t                      fEntries;

    public :
        V1785NProcessor( QObject* parent = nullptr );
        ~V1785NProcessor() override = default;

    public slots :
        void OnStart();
        void Process( VBuffer data );

    signals :
        void PlotDataReady( const QVector<QwtIntervalSample>& data, unsigned nEvents );
        void DataProcessed( bool );
};// V1785NProcessor

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
        void CreateTimer();

    private :
        QTimer          *fTimer;

        QThread         fThread;
        V1785NProcessor *fProcessor;
        bool            fDataProcessed;
        QMutex          fMutex;

        // Widgets
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

    private :
        QPushButton *fStartButton, *fStopButton;
        QSpinBox *fBinSpin, *fStartHistSpin, *fStopHistSpin;

        QwtPlotHistogram*   fHisto;
        QwtPlot*            fPlot;
        QwtPlotMarker*      fStatistics;

    signals :
        void DataReady( VBuffer data );

    protected slots :
        void StartTimer();
        void StopTimer();
        void ReadData();
        void UpdatePlot( const QVector<QwtIntervalSample>& data, unsigned nEvents );
        void UpdateStat( unsigned nEvents );
        void OnDataProcessed( bool status );

    public slots :
        void WriteConfig() override;
        void ReadConfig() override;

        QVariant CollectConfig() override;
        void SpreadConfig( const QVariant& qConfig ) override;

    public :
        V1785NWindow( uint32_t address, V2718Window *parent );
        ~V1785NWindow() override;
};// V1785NWindow
