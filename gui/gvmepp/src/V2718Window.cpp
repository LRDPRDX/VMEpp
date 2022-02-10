#include <fstream>
#include <map>

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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QDockWidget>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTabWidget>
#include "qnamespace.h"

#include "V2718Window.h"
#include "UConfigurable.h"
#include "Dialogs.h"
#include "Style.h"
#include "Prelude.h"

#include "VException.h"

using namespace vmepp;


V2718Window::V2718Window( QWidget *parent ) :
    DeviceWindow( parent )
{
    CreateMenu();
    CreateCentralWidget();
    CreateDockWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

//**********************************
//****** CONSTRUCTIVE METHODS ******
//**********************************
void V2718Window::CreateMenu()
{
    // Config
    connect( fSaveConfigAction, &QAction::triggered, this, &DeviceWindow::SaveConfig<V2718> );
    connect( fLoadConfigAction, &QAction::triggered, this, &DeviceWindow::LoadConfig<V2718> );

    // Add device 
    fAddDeviceAction = new QAction( "Device" );
        connect( this, &V2718Window::Connected, fAddDeviceAction, &QAction::setEnabled );
    fAddMenu = menuBar()->addMenu( "&Add" );
        fAddMenu->addAction( fAddDeviceAction );
    connect( fAddDeviceAction, &QAction::triggered, this, &V2718Window::OpenDeviceDialog );
}

void V2718Window::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    fMainTab = new QTabWidget();

    CreateIOTab();
    CreatePulserTab();

    vLayout->addWidget( fMainTab );
    vLayout->addWidget( fBottomFrame );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V2718Window::CreateDockWidget()
{
    QDockWidget *dock = new QDockWidget( "Display", this );
    fDisplay = new Display( this, dock );
    dock->setWidget( fDisplay );
    addDockWidget( Qt::RightDockWidgetArea, dock );
    dock->setFeatures( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable );
    fViewMenu->addAction( dock->toggleViewAction() );
    dock->setVisible( false );
}

void V2718Window::CreateIOTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Inputs && Outputs") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    SGroupBox *outGroup = new SGroupBox( tr("Outputs") );
    QGridLayout *outLayout = new QGridLayout();
    QString vmeNames[N_OUTS] = { "DS", "AS", "DTACK", "BERR", "LMON" };
    QString miscNames[N_OUTS] = { "Pulser A", "Pulser A", "Pulser B", "Pulser B", "Scaler END" };
    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        QLabel* srcLabel = new QLabel( tr("Source:") );
        fOutSrcCombo[i] = new QComboBox();
            fOutSrcCombo[i]->addItem( vmeNames[i], cvVMESignals );
            fOutSrcCombo[i]->addItem( "Coincidence", cvCoincidence );
            fOutSrcCombo[i]->addItem( miscNames[i], cvMiscSignals );
            fOutSrcCombo[i]->addItem( "SW", cvManualSW );

        QLabel* polLabel = new QLabel( tr("Polarity:") );
        fOutPolCombo[i] = new QComboBox();
            fOutPolCombo[i]->addItem( "Direct", cvDirect );
            fOutPolCombo[i]->addItem( "Inverted", cvInverted );

        QLabel* ledLabel = new QLabel( tr("LED polarity:") );
        fOutLedCombo[i] = new QComboBox();
            fOutLedCombo[i]->addItem( "Active high", cvActiveHigh );
            fOutLedCombo[i]->addItem( "Active low", cvActiveLow );

        outLayout->addWidget( srcLabel, i + 1, 0, Qt::AlignRight );
        outLayout->addWidget( fOutSrcCombo[i], i + 1, 1 );
        outLayout->addWidget( polLabel, i + 1, 2, Qt::AlignRight );
        outLayout->addWidget( fOutPolCombo[i], i + 1, 3 );
        outLayout->addWidget( ledLabel, i + 1, 4, Qt::AlignRight );
        outLayout->addWidget( fOutLedCombo[i], i + 1, 5 );
    }
    outGroup->setLayout( outLayout );

    SGroupBox *inGroup = new SGroupBox( tr("Inputs") );
    QGridLayout *inLayout = new QGridLayout();
    for( uint8_t i = 0; i < N_INS; ++i )
    {
        if( i == 0 )
        {
            QLabel* dumLabel = new QLabel( tr("Source:") );
            QSizePolicy dumPolicy = dumLabel->sizePolicy();
                dumPolicy.setRetainSizeWhenHidden( true );
            dumLabel->setSizePolicy( dumPolicy );
            dumLabel->hide();

            QComboBox *dumCombo = new QComboBox();
                dumCombo->addItem( "VME", cvVMESignals );
                dumCombo->addItem( "Coincidence", cvCoincidence );
                dumCombo->addItem( "P & S", cvMiscSignals );
                dumCombo->addItem( "SW", cvManualSW );
            dumPolicy = dumCombo->sizePolicy();
                dumPolicy.setRetainSizeWhenHidden( true );
            dumCombo->setSizePolicy( dumPolicy );
            dumCombo->hide();

            inLayout->addWidget( dumLabel, i, 0, Qt::AlignRight );
            inLayout->addWidget( dumCombo, i, 1 );
        }

        QLabel* polLabel = new QLabel( tr("Polarity:") );
        fInPolCombo[i] = new QComboBox();
            fInPolCombo[i]->addItem( "Direct", cvDirect  );
            fInPolCombo[i]->addItem( "Inverted", cvInverted  );

        QLabel* ledLabel = new QLabel( tr("LED polarity:") );
        fInLedCombo[i] = new QComboBox();
            fInLedCombo[i]->addItem( "Active high", cvActiveHigh );
            fInLedCombo[i]->addItem( "Active low", cvActiveLow );

        inLayout->addWidget( polLabel, i, 2, Qt::AlignRight );
        inLayout->addWidget( fInPolCombo[i], i, 3 );
        inLayout->addWidget( ledLabel, i, 4, Qt::AlignRight );
        inLayout->addWidget( fInLedCombo[i], i, 5 );
    }
    inGroup->setLayout( inLayout );

    vLayout->addWidget( outGroup );
    vLayout->addWidget( inGroup );
    tab->setLayout( vLayout );
}


void V2718Window::CreatePulserTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Pulser && Scaler") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    // Pulsers
    for( uint8_t i = 0; i < N_PULSERS; ++ i )
    {
        QString pulserName = (i == cvPulserA) ? "Pulser A" : "Pulser B";

        SGroupBox *pulserGroup = new SGroupBox( pulserName );
        QGridLayout *pulLayout = new QGridLayout();

        QLabel* freqLabel = new QLabel( tr("Frequency(Hz):") );
        fPulFreqSpin[i] = new QSpinBox();
            fPulFreqSpin[i]->setRange( 1, 10000000 );

        QLabel* dutyLabel = new QLabel( tr("Duty(%):" ) );
        fPulDutySpin[i] = new QSpinBox();
            fPulDutySpin[i]->setRange( 1, 99 );

        QLabel* nLabel = new QLabel( tr("Number of pulses:" ) );
            nLabel->setToolTip( "0 = infinite" );
        fPulNSpin[i] = new QSpinBox();
            fPulNSpin[i]->setRange( 0, 1023 );
            fPulNSpin[i]->setToolTip( "0 = infinite" );

        QLabel* startLabel = new QLabel( tr("Start source:") );
        fPulStartCombo[i] = new QComboBox();
            fPulStartCombo[i]->addItem( "SW", cvManualSW );
            fPulStartCombo[i]->addItem( "Input 0", cvInputSrc0 );
            fPulStartCombo[i]->addItem( "Input 1", cvInputSrc1 );
            fPulStartCombo[i]->addItem( "Coincidence", cvCoincidence );

        QLabel* stopLabel = new QLabel( tr("Stop source:") );
        fPulStopCombo[i] = new QComboBox();
            fPulStopCombo[i]->addItem( "SW", cvManualSW );
            fPulStopCombo[i]->addItem( "Input " + QString::number( i ), (i == 0) ? cvInputSrc0 : cvInputSrc1 );

        SFrame *buttonFrame = new SFrame( SColor_t::VIOLET );
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        fPulStartButton[i] = new QPushButton( "START" );
            ColorButton( fPulStartButton[i], QColor( style::yellow ) );
            connect( this, &V2718Window::Connected, fPulStartButton[i], &QPushButton::setEnabled );
            connect( fPulStartButton[i], &QPushButton::clicked, this, &V2718Window::PulserSlot );
        fPulStopButton[i] = new QPushButton( "STOP" );
            ColorButton( fPulStopButton[i], QColor( style::pink ) );
            connect( this, &V2718Window::Connected, fPulStopButton[i], &QPushButton::setEnabled );
            connect( fPulStopButton[i], &QPushButton::clicked, this, &V2718Window::PulserSlot );

        buttonLayout->addWidget( fPulStartButton[i] );
        buttonLayout->addWidget( fPulStopButton[i] );
        buttonFrame->setLayout( buttonLayout );

        pulLayout->addWidget( freqLabel, 0, 0, Qt::AlignRight );
        pulLayout->addWidget( fPulFreqSpin[i], 0, 1 );
        pulLayout->addWidget( dutyLabel, 0, 2, Qt::AlignRight );
        pulLayout->addWidget( fPulDutySpin[i], 0, 3 );
        pulLayout->addWidget( nLabel, 0, 4, Qt::AlignRight );
        pulLayout->addWidget( fPulNSpin[i], 0, 5 );
        pulLayout->addWidget( startLabel, 1, 0, Qt::AlignRight );
        pulLayout->addWidget( fPulStartCombo[i], 1, 1 );
        pulLayout->addWidget( stopLabel, 1, 2, Qt::AlignRight );
        pulLayout->addWidget( fPulStopCombo[i], 1, 3 );
        pulLayout->addWidget( buttonFrame, 1, 4, 1, 2 );

        pulserGroup->setLayout( pulLayout );

        vLayout->addWidget( pulserGroup );
    }
    // Scaler
    SGroupBox *scalerGroup = new SGroupBox( "Scaler" );
    QGridLayout *scalLayout = new QGridLayout();

    QLabel *limitLabel = new QLabel( "Limit:" );
    fScalLimitSpin = new QSpinBox();
        fScalLimitSpin->setRange( 0, 1023 );
    fScalAutoCheck = new QCheckBox( "Auto reset" );


    SFrame *buttonFrame = new SFrame( SColor_t::VIOLET );
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    fScalGateButton = new QPushButton( "GATE" );
        ColorButton( fScalGateButton, style::yellow );
        connect( this, &V2718Window::Connected, fScalGateButton, &QPushButton::setEnabled );
        connect( fScalGateButton, &QPushButton::clicked, this, &V2718Window::StartScaler );
    fScalStopButton = new QPushButton( "STOP" );
        ColorButton( fScalStopButton, style::pink );
        connect( this, &V2718Window::Connected, fScalStopButton, &QPushButton::setEnabled );
        connect( fScalStopButton, &QPushButton::clicked, this, &V2718Window::StopScaler );
    fScalResetButton = new QPushButton( "RESET" );
        ColorButton( fScalResetButton, style::yellow );
        connect( this, &V2718Window::Connected, fScalResetButton, &QPushButton::setEnabled );
        connect( fScalResetButton, &QPushButton::clicked, this, &V2718Window::ResetScaler );

    buttonLayout->addWidget( fScalGateButton );
    buttonLayout->addWidget( fScalStopButton );
    buttonLayout->addWidget( fScalResetButton );
    buttonFrame->setLayout( buttonLayout );

    QLabel *hitLabel = new QLabel( "Hit source:" );
    fScalHitCombo = new QComboBox();
        fScalHitCombo->addItem( "Input 0", cvInputSrc0 );
        fScalHitCombo->addItem( "Coincidence", cvCoincidence );
    QLabel *gateLabel = new QLabel( "Gate source:" );
    fScalGateCombo = new QComboBox();
        fScalGateCombo->addItem( "SW", cvManualSW );
        fScalGateCombo->addItem( "Input 1", cvInputSrc1 );
    QLabel *resetLabel = new QLabel( "Reset source:" );
    fScalResetCombo = new QComboBox();
        fScalResetCombo->addItem( "SW", cvManualSW );
        fScalResetCombo->addItem( "Input 1", cvInputSrc1 );

    scalLayout->addWidget( limitLabel, 0, 0, Qt::AlignRight );
    scalLayout->addWidget( fScalLimitSpin, 0, 1 );
    scalLayout->addWidget( buttonFrame, 0, 3, 1, 3 );
    scalLayout->addWidget( hitLabel, 1, 0, Qt::AlignRight );
    scalLayout->addWidget( fScalHitCombo, 1, 1 );
    scalLayout->addWidget( gateLabel, 1, 2, Qt::AlignRight );
    scalLayout->addWidget( fScalGateCombo, 1, 3 );
    scalLayout->addWidget( resetLabel, 1, 4, Qt::AlignRight );
    scalLayout->addWidget( fScalResetCombo, 1, 5 );
    scalLayout->addWidget( fScalAutoCheck, 2, 0, 1, 6, Qt::AlignRight );

    scalerGroup->setLayout( scalLayout );

    vLayout->addWidget( scalerGroup );

    tab->setLayout( vLayout );
}


//*******************
//****** SLOTS ******
//*******************
void V2718Window::OpenConnectDialog()
{
    Connection *cDialog = new Connection( this );
        cDialog->resize( 300, 150 );
    cDialog->show();
}

void V2718Window::OpenDeviceDialog()
{
    DeviceDialog *devDialog = new DeviceDialog( this );
    devDialog->show();
}

void V2718Window::UpdateDisplay()
{
    try
    {
        CVDisplay displayInfo = fController.ReadDisplay();
        fDisplay->Update( displayInfo );
    }
    catch( const VException &e )
    {
        emit Error( e );
    }
}

void V2718Window::WriteConfig()
{
    try
    {
        UConfig<V2718> cfg = qvariant_cast< UConfig<V2718> >( this->CollectConfig() );
        fController.WriteConfig( cfg );
        Programmed( true );
    }
    catch( const VException &e )
    {
        emit Programmed( false );
        emit Error( e );
    }
}

void V2718Window::ReadConfig()
{
    UConfig<V2718> cfg;

    try
    {
        fController.ReadConfig( cfg );
        QVariant qv; qv.setValue( cfg );
        SpreadConfig( qv );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V2718Window::PulserSlot()
{
    QObject* obj = QObject::sender();
    for( uint8_t i = 0; i < N_PULSERS; ++i )
    {
        if( obj == fPulStartButton[i] )
        {
            StartPulser( (i == 0) ? V2718::Pulser_t::A : V2718::Pulser_t::B );
            break;
        }
        else if( obj == fPulStopButton[i] )
        {
            StopPulser( (i == 0) ? V2718::Pulser_t::A : V2718::Pulser_t::B );
            break;
        }
    }
}

void V2718Window::StartPulser( V2718::Pulser_t p )
{
    try
    {
        fController.GetPulser( p )->Start();
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V2718Window::StopPulser( V2718::Pulser_t p )
{
    try
    {
        fController.GetPulser( p )->Stop();
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V2718Window::StartScaler()
{
    try
    {
        fController.GetScaler()->EnableGate( true );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V2718Window::StopScaler()
{
    try
    {
        fController.GetScaler()->EnableGate( false );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V2718Window::ResetScaler()
{
    try
    {
        fController.GetScaler()->Reset();
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

QVariant V2718Window::CollectConfig()
{
    UConfig<V2718> cfg;

    // In's and Out's
    for( uint8_t i = 0; i < N_INS; ++i )
    {
        cfg.INPUTS.at( i ).POLARITY     = fInPolCombo[i]->currentData().value<V2718::Polarity_t>();
        cfg.INPUTS.at( i ).LED_POLARITY = fInLedCombo[i]->currentData().value<V2718::LEDPolarity_t>();
    }

    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        cfg.OUTPUTS.at( i ).POLARITY        = fOutPolCombo[i]->currentData().value<V2718::Polarity_t>();
        cfg.OUTPUTS.at( i ).LED_POLARITY    = fOutLedCombo[i]->currentData().value<V2718::LEDPolarity_t>();
        cfg.OUTPUTS.at( i ).SOURCE          = fOutSrcCombo[i]->currentData().value<V2718::Src_t>();
    }

    // Pulsers
    for( uint8_t i = 0; i < N_PULSERS; ++i )
    {
        UConfig<V2718>::Pulser& pulser = (i == cvPulserA) ? cfg.PULSER_A : cfg.PULSER_B;
        pulser.FREQUENCY    = fPulFreqSpin[i]->value();
        pulser.DUTY         = fPulDutySpin[i]->value();
        pulser.N_PULSES     = fPulNSpin[i]->value();
        pulser.START_SOURCE = fPulStartCombo[i]->currentData().value<V2718::Src_t>();
        pulser.STOP_SOURCE  = fPulStopCombo[i]->currentData().value<V2718::Src_t>();
    }

    // Scaler
    cfg.SCALER.GATE_SOURCE  = fScalGateCombo->currentData().value<V2718::Src_t>();
    cfg.SCALER.STOP_SOURCE  = fScalResetCombo->currentData().value<V2718::Src_t>();
    cfg.SCALER.HIT_SOURCE   = fScalHitCombo->currentData().value<V2718::Src_t>();
    cfg.SCALER.LIMIT        = fScalLimitSpin->value();
    cfg.SCALER.AUTO_RESET   = fScalAutoCheck->isChecked();

    QVariant qv; qv.setValue( cfg );

    return qv;
}

void V2718Window::SpreadConfig( const QVariant& qConfig )
{
    UConfig<V2718> cfg = qvariant_cast< UConfig<V2718> >( qConfig );
    // In's and Out's
    for( uint8_t i = 0; i < N_INS; ++i )
    {
        ChangeCombo( fInPolCombo[i], cfg.INPUTS.at( i ).POLARITY );
        ChangeCombo( fInLedCombo[i], cfg.INPUTS.at( i ).LED_POLARITY );
    }

    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        ChangeCombo( fOutPolCombo[i], cfg.OUTPUTS.at( i ).POLARITY  );
        ChangeCombo( fOutLedCombo[i], cfg.OUTPUTS.at( i ).LED_POLARITY );
        ChangeCombo( fOutSrcCombo[i], cfg.OUTPUTS.at( i ).SOURCE );
    }

    // Pulsers
    for( uint8_t i = 0; i < N_PULSERS; ++i )
    {
        const UConfig<V2718>::Pulser& pulser = (i == static_cast<uint8_t>(V2718::Pulser_t::A)) ? cfg.PULSER_A : cfg.PULSER_B;

        fPulFreqSpin[i]->setValue( pulser.FREQUENCY );
        fPulDutySpin[i]->setValue( pulser.DUTY );
        fPulNSpin[i]->setValue( pulser.N_PULSES );

        ChangeCombo( fPulStartCombo[i], pulser.START_SOURCE );
        ChangeCombo( fPulStopCombo[i], pulser.STOP_SOURCE );
    }

    // Scaler
    ChangeCombo( fScalGateCombo, cfg.SCALER.GATE_SOURCE );
    ChangeCombo( fScalResetCombo, cfg.SCALER.STOP_SOURCE );
    ChangeCombo( fScalHitCombo, cfg.SCALER.HIT_SOURCE );

    fScalLimitSpin->setValue( cfg.SCALER.LIMIT );
    fScalAutoCheck->setChecked( cfg.SCALER.AUTO_RESET );
}

void V2718Window::Connect( short link, short conet )
{
    //Exception may be thrown here
    fController.Open( link, conet );

    setWindowTitle( "gVME++ (V2718)" );
    statusBar()->showMessage( "Connected..." );

    emit Connected( true );
}

void V2718Window::Connect()
{
    OpenConnectDialog();
}


void V2718Window::Disconnect()
{
    try
    {
        fController.Close();
    }
    catch( const VException& e )
    {
        PrintMessage( Message_t::ERROR, "Error while disconnecting" );
    }

    setWindowTitle( "gVME++" );
    statusBar()->showMessage( "Disconnected..." );

    emit Connected( false );
}

void V2718Window::closeEvent( QCloseEvent *event )
{
    OkCancelDialog* d = new OkCancelDialog( "Are you sure?", this );
        d->setAttribute( Qt::WA_DeleteOnClose, true );
    int ret = d->exec();
    if( ret == QDialog::Accepted )
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void V2718Window::DoOnError( const VException& e )
{
    switch( e.GetErrorCode() )
    {
        case( VError_t::vSuccess ) :
        case( VError_t::vInvalidParam ) :
        case( VError_t::vTimeoutError ) :
        case( VError_t::vAlreadyOpenError ) :
        case( VError_t::vMaxBoardCountError ) :

        case( VError_t::vBadMaster ) :
        case( VError_t::vOrphan ) :
        case( VError_t::vBuffAllocFailed ) :
        case( VError_t::vBadSlave ) :
        case( VError_t::vConfigError ) :
        case( VError_t::vAccessError ) :
            break;

        case( VError_t::vBusError ) :
        case( VError_t::vCommError ) :
        case( VError_t::vGenericError ) :
            Disconnect(); break;
    }
}


/*********************/
/****** DISPLAY ******/
/*********************/
const std::array<QString, Display::N_LED> Display::fLEDNames = {"AM0", "AM1", "AM2", "AM3", "AM4", "AM5",
                                                                "DS0", "DS1",
                                                                "AS", "IACK", "WRITE", "LWORD",
                                                                "IRQ1", "IRQ2", "IRQ3", "IRQ4", "IRQ5",
                                                                "IRQ6", "IRQ7",
                                                                "BREQ", "BGNT", "SRES", "DTK", "BERR" };

Display::Display( V2718Window *controller, QWidget *parent ) :
    QWidget( parent ),
    fController( controller )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    CreateDisplay();
}

Display::~Display()
{
}

void Display::CreateDisplay()
{
    QVBoxLayout *layout = new QVBoxLayout();

    SFrame *upperFrame = new SFrame( SColor_t::VIOLET );
    QGridLayout *upperLayout = new QGridLayout();

    QLabel *addressLabel = new QLabel( "Address:" );
    fAddressText = new QLineEdit();
        fAddressText->setReadOnly( true );

    QLabel *dataLabel = new QLabel( "Data:" );
    fDataText = new QLineEdit();
        fDataText->setReadOnly( true );

    upperLayout->addWidget( addressLabel, 0, 0, Qt::AlignRight );
    upperLayout->addWidget( fAddressText, 0, 1 );
    upperLayout->addWidget( dataLabel, 1, 0, Qt::AlignRight );
    upperLayout->addWidget( fDataText, 1, 1 );

    upperFrame->setLayout( upperLayout );
    layout->addWidget( upperFrame );

    SFrame *lowerFrame = new SFrame( SColor_t::VIOLET );
    QGridLayout *lowerLayout = new QGridLayout();

    for( size_t i = 0; i < N_LED/2; ++i )
    {
        fLED[i] = new SLedIndicatorWithLabel( fLEDNames[i], false );
        fLED[i]->SetColor( style::yellow );
        lowerLayout->addWidget( fLED[i], i, 0 );
    }

    for( size_t i = N_LED/2; i < N_LED; ++i )
    {
        fLED[i] = new SLedIndicatorWithLabel( fLEDNames[i], true );
        fLED[i]->SetColor( style::yellow );
        lowerLayout->addWidget( fLED[i], i - N_LED/2, 1 );
    }
    fLED[GetLED("BERR")]->SetColor( style::red );

    lowerLayout->setSpacing( 1 );
    lowerFrame->setLayout( lowerLayout );
    layout->addWidget( lowerFrame );

    fUpdateButton = new QPushButton( "UPDATE" );
        ColorButton( fUpdateButton, style::yellow );
        connect( fUpdateButton, &QPushButton::clicked, fController, &V2718Window::UpdateDisplay );
        connect( fController, &V2718Window::Connected, fUpdateButton, &QPushButton::setEnabled );

    SFrame *buttonFrame = new SFrame( SColor_t::VIOLET );
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget( fUpdateButton );
    buttonFrame->setLayout( buttonLayout );
    layout->addWidget( buttonFrame );

    setLayout( layout );
}


void Display::Update( const CVDisplay &display )
{
    fAddressText->setText( QString().setNum( display.cvAddress, 16 ) );
    fDataText->setText( QString().setNum( display.cvData, 16 ) );

    std::map<QString, size_t> m;
    for( size_t i = 0; i < fLEDNames.size(); ++i )
    {
        m.insert( std::make_pair( fLEDNames[i], i ) );
    }

    fLED[m["AM0"]]->SetChecked( ((1 << 0) & display.cvAM) );
    fLED[m["AM1"]]->SetChecked( ((1 << 1) & display.cvAM) );
    fLED[m["AM2"]]->SetChecked( ((1 << 2) & display.cvAM) );
    fLED[m["AM3"]]->SetChecked( ((1 << 3) & display.cvAM) );
    fLED[m["AM4"]]->SetChecked( ((1 << 4) & display.cvAM) );
    fLED[m["AM5"]]->SetChecked( ((1 << 5) & display.cvAM) );

    fLED[m["IRQ1"]]->SetChecked( ((1 << 0) & display.cvIRQ) );
    fLED[m["IRQ2"]]->SetChecked( ((1 << 1) & display.cvIRQ) );
    fLED[m["IRQ3"]]->SetChecked( ((1 << 2) & display.cvIRQ) );
    fLED[m["IRQ4"]]->SetChecked( ((1 << 3) & display.cvIRQ) );
    fLED[m["IRQ5"]]->SetChecked( ((1 << 4) & display.cvIRQ) );
    fLED[m["IRQ6"]]->SetChecked( ((1 << 5) & display.cvIRQ) );
    fLED[m["IRQ7"]]->SetChecked( ((1 << 6) & display.cvIRQ) );

    fLED[m["AS"]]->SetChecked( display.cvAS );
    fLED[m["IACK"]]->SetChecked( display.cvIACK );
    fLED[m["DS0"]]->SetChecked( display.cvDS0 );
    fLED[m["DS1"]]->SetChecked( display.cvDS1 );
    fLED[m["WRITE"]]->SetChecked( display.cvWRITE );
    fLED[m["LWORD"]]->SetChecked( display.cvLWORD );
    fLED[m["BREQ"]]->SetChecked( display.cvBR );
    fLED[m["BGNT"]]->SetChecked( display.cvBG );
    fLED[m["SRES"]]->SetChecked( display.cvSYSRES );
    fLED[m["DTK"]]->SetChecked( display.cvDTACK );
    fLED[m["BERR"]]->SetChecked( display.cvBERR );
}

size_t Display::GetLED( const QString& name )
{
    size_t i = 0;
    for( auto it = fLEDNames.cbegin(); it != fLEDNames.cend() && *it != name; )
    {
        ++it; ++i;
    }
    return i;
}
