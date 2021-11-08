#include <algorithm>
#include <random>

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QFileDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QDockWidget>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTabWidget>
#include <QMutexLocker>
#include "qnamespace.h"

#include <qwt_plot_histogram.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot.h>

#include "V2718Window.h"
#include "V1785NWindow.h"
#include "VException.h"
#include "modules/V1785N.h"

#include "Style.h"
#include "Prelude.h"

using namespace vmepp;


V1785NWindow::V1785NWindow( uint32_t address, V2718Window* parent ) :
    SlaveWindow( parent )
{
    fDevice = new V1785N( address );

    setWindowTitle( "V1785N" );

    CreateTimer();
    CreateMenu();
    CreateCentralWidget();

    connect( &fThread, &V1785NThread::DataReady, this, &V1785NWindow::UpdateData );

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

void V1785NWindow::CreateMenu()
{
    connect( fSaveConfigAction, &QAction::triggered, this, &DeviceWindow::SaveConfig<V1785N> );
    connect( fLoadConfigAction, &QAction::triggered, this, &DeviceWindow::LoadConfig<V1785N> );
}

void V1785NWindow::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    fMainTab = new QTabWidget();

    CreateGeneralTab();
    CreateChannelsTab();
    CreatePlotTab();

    vLayout->addWidget( fMainTab );
    vLayout->addWidget( fBottomFrame );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V1785NWindow::CreateGeneralTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, "General" );

    QVBoxLayout *vLayout = new QVBoxLayout();

    SGroupBox *suppGroup = new SGroupBox( "Suppression" );
    QGridLayout *suppLayout = new QGridLayout();

    fZeroSuppCheck = new QCheckBox( "Zero suppression" );
    fOverSuppCheck = new QCheckBox( "Over suppression" );

    QLabel* zeroSuppTypeLabel = new QLabel( "Zero suppression type:" );
    fZeroSuppTypeCombo = new QComboBox();
        fZeroSuppTypeCombo->addItem( "x16", qVariantFromValue( V1785N::ZeroSupp_t::Tx16) );
        fZeroSuppTypeCombo->addItem( "x2", qVariantFromValue( V1785N::ZeroSupp_t::Tx2 ) );

    suppLayout->addWidget( fZeroSuppCheck, 0, 0 );
    suppLayout->addWidget( fOverSuppCheck, 1, 0 );
    suppLayout->addWidget( zeroSuppTypeLabel, 0, 1, Qt::AlignRight );
    suppLayout->addWidget( fZeroSuppTypeCombo, 0, 2 );

    suppGroup->setLayout( suppLayout );

    SGroupBox *irqGroup = new SGroupBox( "Interrupt" );
    QGridLayout *irqLayout = new QGridLayout();

    QLabel* irqLevelLabel = new QLabel( "Level:" );
    fIRQLevelCombo = new QComboBox();
        fIRQLevelCombo->addItem( "DISABLED", 0 );
        fIRQLevelCombo->addItem( "IRQ1", 1 );
        fIRQLevelCombo->addItem( "IRQ2", 2 );
        fIRQLevelCombo->addItem( "IRQ3", 3 );
        fIRQLevelCombo->addItem( "IRQ4", 4 );
        fIRQLevelCombo->addItem( "IRQ5", 5 );
        fIRQLevelCombo->addItem( "IRQ6", 6 );
        fIRQLevelCombo->addItem( "IRQ7", 7 );

    QLabel* irqVectorLabel = new QLabel( "Vector:" );
    fIRQVectorSpin = new QSpinBox();
        fIRQVectorSpin->setMinimum( 0 );
        fIRQVectorSpin->setMaximum( 0xff );

    QLabel* irqEventsLabel = new QLabel( "Events:" );
    fIRQEventsSpin = new QSpinBox();
        fIRQEventsSpin->setMinimum( 1 );
        fIRQEventsSpin->setMaximum( 31 );

    irqLayout->addWidget( irqLevelLabel, 0, 0, Qt::AlignRight );
    irqLayout->addWidget( fIRQLevelCombo, 0, 1 );
    irqLayout->addWidget( irqVectorLabel, 0, 2, Qt::AlignRight );
    irqLayout->addWidget( fIRQVectorSpin, 0, 3 );
    irqLayout->addWidget( irqEventsLabel, 0, 4, Qt::AlignRight );
    irqLayout->addWidget( fIRQEventsSpin, 0, 5 );

    irqGroup->setLayout( irqLayout );

    vLayout->addWidget( suppGroup );
    vLayout->addWidget( irqGroup );

    tab->setLayout( vLayout );
}

void V1785NWindow::CreateChannelsTab()
{
    QWidget* tab = new QWidget();

    QVBoxLayout* vLayout = new QVBoxLayout();

    fChannelTab = new QTabWidget();
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        QWidget* tab = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();

        SGroupBox* lowGroup = new SGroupBox( "Low range" );
        QGridLayout* lowLayout = new QGridLayout();
        fChEnLowCheck[ch] = new QCheckBox( "Enable" );
        QLabel* lowLabel = new QLabel( "Threshold:" );
        fChThrLowSpin[ch] = new QSpinBox();
            fChThrLowSpin[ch]->setMinimum( 0 );
            fChThrLowSpin[ch]->setMaximum( 255 );
        lowLayout->addWidget( fChEnLowCheck[ch], 0, 0 );
        lowLayout->addWidget( lowLabel, 0, 1, Qt::AlignRight );
        lowLayout->addWidget( fChThrLowSpin[ch], 0, 2 );
        lowGroup->setLayout( lowLayout );

        SGroupBox* highGroup = new SGroupBox( "High range" );
        QGridLayout* highLayout = new QGridLayout();
        fChEnHighCheck[ch] = new QCheckBox( "Enable" );
        QLabel* highLabel = new QLabel( "Threshold:" );
        fChThrHighSpin[ch] = new QSpinBox();
            fChThrHighSpin[ch]->setMinimum( 0 );
            fChThrHighSpin[ch]->setMaximum( 255 );
        highLayout->addWidget( fChEnHighCheck[ch], 0, 0 );
        highLayout->addWidget( highLabel, 0, 1, Qt::AlignRight );
        highLayout->addWidget( fChThrHighSpin[ch], 0, 2 );
        highGroup->setLayout( highLayout );

        vLayout->addWidget( lowGroup );
        vLayout->addWidget( highGroup );

        tab->setLayout( vLayout );

        fChannelTab->addTab( tab, QString( "Ch %1" ).arg( ch ) );
    }

    vLayout->addWidget( fChannelTab );
    tab->setLayout( vLayout );

    fMainTab->addTab( tab, "Channels" );
}

void V1785NWindow::CreatePlotTab()
{
    QWidget* tab = new QWidget();

    QVBoxLayout* vLayout = new QVBoxLayout();

    SGroupBox* histGroup = new SGroupBox( "Histogram" );
    QGridLayout* histLayout = new QGridLayout();

    QLabel* binLabel = new QLabel( "# of bins:" );
    fBinSpin = new QSpinBox();
        fBinSpin->setMinimum( 1 );
        fBinSpin->setMaximum( 100000 );
        fBinSpin->setValue( 4096 );

    QLabel* startLabel = new QLabel( "X<sub>1</sub>:" );
    fStartHistSpin = new QSpinBox();
        fStartHistSpin->setMinimum( -1000000000 );
        fStartHistSpin->setMaximum( 1000000000 );
        fStartHistSpin->setValue( 0 );
    QLabel* stopLabel = new QLabel( "X<sub>2</sub>:" );
    fStopHistSpin = new QSpinBox();
        fStopHistSpin->setMinimum( -1000000000 );
        fStopHistSpin->setMaximum( 1000000000 );
        fStopHistSpin->setValue( 4096 );

    histLayout->addWidget( binLabel, 0, 0, Qt::AlignRight );
    histLayout->addWidget( fBinSpin, 0, 1 );
    histLayout->addWidget( startLabel, 0, 2, Qt::AlignRight );
    histLayout->addWidget( fStartHistSpin, 0, 3 );
    histLayout->addWidget( stopLabel, 0, 4, Qt::AlignRight );
    histLayout->addWidget( fStopHistSpin, 0, 5 );

    histGroup->setLayout( histLayout );
    histGroup->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    vLayout->addWidget( histGroup );

    // Buttons
    fStartButton = new QPushButton( "START" );
        ColorButton( fStartButton, style::yellow );
        connect( this, &V1785NWindow::Connected, fStartButton, &QPushButton::setEnabled );
        connect( fStartButton, &QPushButton::clicked, this, &V1785NWindow::InitHistogram );
        connect( fStartButton, &QPushButton::clicked, this, &V1785NWindow::StartTimer );
    fStopButton = new QPushButton( "STOP" );
        ColorButton( fStopButton, style::pink );
        connect( this, &V1785NWindow::Connected, fStopButton, &QPushButton::setEnabled );
        connect( fStopButton, &QPushButton::clicked, this, &V1785NWindow::StopTimer );
        connect( fStopButton, &QPushButton::clicked, &fThread, &V1785NThread::OnStop );

    SFrame *buttonFrame = new SFrame( SColor_t::VIOLET );
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget( fStartButton );
    buttonLayout->addWidget( fStopButton );

    buttonFrame->setLayout( buttonLayout );
    vLayout->addWidget( buttonFrame );

    // Histogram
    fPlot = new QwtPlot();
        //fPlot->setCanvasBackground( QColor( style::pink ) );
        fPlot->setAxisTitle( QwtPlot::yLeft, "Number of events" );
        fPlot->setAxisTitle( QwtPlot::xBottom, "ADC value, [LSB]" );

    QwtPlotGrid *grid = new QwtPlotGrid;
        grid->enableX( true );
        grid->enableY( true );
        grid->attach( fPlot );
        grid->setMajorPen( QPen( QColor( style::pink ), 0, Qt::DotLine ) );

    fHisto = new QwtPlotHistogram( "Data" );
        fHisto->setPen( QPen( QColor( style::red ) ) );
        fHisto->setBrush( QBrush( QColor( style::blue ) ) );
    fHisto->attach( fPlot );

    fStatistics = new QwtPlotMarker();
    fStatistics->attach( fPlot );

    fPlot->replot();
    fPlot->show();

    UpdateStat( 0 );

    vLayout->addWidget( fPlot );

    tab->setLayout( vLayout );
    fMainTab->addTab( tab, "Plots" );
}

void V1785NWindow::CreateTimer()
{
    fTimer = new QTimer( this );
        fTimer->setInterval( 1000 );
    connect( fTimer, &QTimer::timeout, &fThread, &V1785NThread::OnDump );
    connect( this, &V1785NWindow::Connected, this, &V1785NWindow::StopTimer );
    connect( this, &V1785NWindow::Error, this, &V1785NWindow::StopTimer );
}

void V1785NWindow::WriteConfig()
{
    try
    {
        UConfig<V1785N> cfg = qvariant_cast< UConfig<V1785N> >( this->CollectConfig() );
        dynamic_cast<V1785N*>(fDevice)->WriteConfig( cfg );
        emit Programmed( true );
    }
    catch( const VException& e )
    {
        emit Programmed( false );
        emit Error( e );
    }
}

void V1785NWindow::ReadConfig()
{
    UConfig<V1785N> cfg;

    try
    {
        dynamic_cast<V1785N*>(fDevice)->ReadConfig( cfg );
        QVariant qv; qv.setValue( cfg );
        SpreadConfig( qv );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

QVariant V1785NWindow::CollectConfig()
{
    UConfig<V1785N> cfg;
    UConfig<V1785N>::Threshold thr;
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        thr.LOW.VALUE = fChThrLowSpin[ch]->value();
        thr.LOW.KILL = not fChEnLowCheck[ch]->isChecked();
        thr.HIGH.VALUE = fChThrHighSpin[ch]->value();
        thr.HIGH.KILL = not fChEnHighCheck[ch]->isChecked();
        cfg.THRESHOLDS.at( ch ) = thr;
    }

    cfg.ZERO_SUPP = fZeroSuppCheck->isChecked();
    cfg.OVER_SUPP = fOverSuppCheck->isChecked();
    cfg.ZERO_SUPP_TYPE = fZeroSuppTypeCombo->currentData().value<V1785N::ZeroSupp_t>();

    cfg.IRQ_LEVEL = fIRQLevelCombo->currentData().value<uint8_t>();
    cfg.IRQ_VECTOR = fIRQVectorSpin->value();
    cfg.IRQ_EVENTS = fIRQEventsSpin->value();

    QVariant qv; qv.setValue( cfg );

    return qv;
}

void V1785NWindow::SpreadConfig( const QVariant& qConfig )
{
    UConfig<V1785N> cfg = qvariant_cast< UConfig<V1785N> >( qConfig );

    UConfig<V1785N>::Threshold thr;
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        thr = cfg.THRESHOLDS.at( ch );
        fChThrLowSpin[ch]->setValue( thr.LOW.VALUE );
        fChEnLowCheck[ch]->setChecked( not thr.LOW.KILL );
        fChThrHighSpin[ch]->setValue( thr.HIGH.VALUE );
        fChEnHighCheck[ch]->setChecked( not thr.HIGH.KILL );
    }

    fZeroSuppCheck->setChecked( cfg.ZERO_SUPP );
    fOverSuppCheck->setChecked( cfg.OVER_SUPP );
    ChangeCombo( fZeroSuppTypeCombo, cfg.ZERO_SUPP_TYPE );

    ChangeCombo( fIRQLevelCombo, cfg.IRQ_LEVEL );
    fIRQVectorSpin->setValue( cfg.IRQ_VECTOR );
    fIRQEventsSpin->setValue( cfg.IRQ_EVENTS );
}

void V1785NWindow::StartTimer()
{
    if( not fTimer->isActive() )
    {
        fTimer->start();
    }
}

void V1785NWindow::StopTimer()
{
    if( fTimer->isActive() )
    {
        fTimer->stop();
    }
}

void V1785NWindow::UpdateData( const QVector<QwtIntervalSample>& data, unsigned nEvents )
{
    fHisto->setSamples( data );
    fPlot->replot();
    UpdateStat( nEvents );
}

void V1785NWindow::InitHistogram()
{
    QVector<QwtIntervalSample> histData;

    for( size_t i = 0; i < N_BITS; ++i )
    {
        histData.push_back( QwtIntervalSample( 0.0, i, i + 1 ) );
    }

    fHisto->setSamples( histData );
    fThread.OnStart( N_BITS );
}

void V1785NWindow::UpdateStat( unsigned nEvents )
{
    QwtScaleMap xMap = fPlot->canvasMap( QwtPlot::xBottom );
        double x = (xMap.s1() + xMap.s2()) * 0.8;
    QwtScaleMap yMap = fPlot->canvasMap( QwtPlot::yLeft );
        double y = (yMap.s1() + yMap.s2()) * 0.8;
    fStatistics->setValue( x, y );
    QwtText text = QString( "N = %1" ).arg( nEvents );
        text.setColor( style::white );
    fStatistics->setLabel( text );
}


//***************************
//****** V1785N THREAD ******
//***************************

V1785NThread::V1785NThread( QObject* parent ) :
    QThread( parent ),

    fAbortRequest( false ),
    fRestartRequest( false ),
    fDumpRequest( false ),

    fSize( 0 )
{
    qRegisterMetaType<QVector<QwtIntervalSample>>();
}

V1785NThread::~V1785NThread()
{
    fMutex.lock();
        fAbortRequest = true;
        fCondition.wakeOne();
    fMutex.unlock();

    wait();
}

void V1785NThread::OnStart( size_t size )
{
    QMutexLocker locker( &fMutex );
    fAbortRequest = false;
    fSize = size;

    if( not isRunning() )
    {
        start( LowPriority );
    }
    else
    {
        fRestartRequest = true;
        fCondition.wakeOne();
    }
}

void V1785NThread::OnStop()
{
    QMutexLocker locker( &fMutex );
    fAbortRequest = true;
    fCondition.wakeOne();
}

void V1785NThread::OnDump()
{
    QMutexLocker locker( &fMutex );
    fDumpRequest = true;
}

void V1785NThread::run()
{
    forever
    {
        fMutex.lock();
            const size_t size = fSize;
        fMutex.unlock();

        QVector<QwtIntervalSample> data( size );
        unsigned nEvents = 0;

        for( size_t bin = 0; bin < size; ++bin )
        {
            data[bin] = QwtIntervalSample( 0.0, bin, bin + 1 );
        }

        forever
        {
            if( fAbortRequest ) return;

            if( fDumpRequest )
            {
                emit DataReady( data, nEvents );
                fMutex.lock();
                    fDumpRequest = false;
                fMutex.unlock();
            }

            if( fRestartRequest )
            {
                break;
            }
            nEvents++;
        }
        // The only way to break from the above forever loop is
        // the fRestartRequest flag
        fMutex.lock();
            fRestartRequest = false;
        fMutex.unlock();
    }
}
