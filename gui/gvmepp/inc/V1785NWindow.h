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


class V1785NThread : public QThread
{
    Q_OBJECT

    private :
        bool fAbortRequest;
        bool fRestartRequest;
        bool fDumpRequest; 

        QMutex fMutex;
        QWaitCondition fCondition;

        size_t fSize;

    public :
        V1785NThread( QObject* parent = nullptr );
        ~V1785NThread() override;

    public slots :
        void OnStart( size_t size );
        void OnStop();
        //void OnPause();
        //void OnResume();
        void OnDump();

    signals :
        void DataReady( const QVector<QwtIntervalSample>& data, unsigned nEvents );

    protected :
        void run() override;

};// V1785NThread

class V1785NWindow : public SlaveWindow
{
    Q_OBJECT

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = V1785N::GetChNumber(); // 6 channels
        static constexpr size_t  N_BITS = 4096;

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

        V1785NThread    fThread;

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

    protected slots :
        void InitHistogram();
        void StartTimer();
        void StopTimer();
        void UpdateData( const QVector<QwtIntervalSample>& data, unsigned nEvents );
        void UpdateStat( unsigned nEvents );

    public slots :
        void WriteConfig() override;
        void ReadConfig() override;

        QVariant CollectConfig() override;
        void SpreadConfig( const QVariant& qConfig ) override;

    public :
        V1785NWindow( uint32_t address, V2718Window *parent );
        ~V1785NWindow() override = default;
};// V1785NWindow
