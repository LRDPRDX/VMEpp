#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QFileDialog>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QDockWidget>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTabWidget>
#include "qnamespace.h"

#include "Controller.h"
#include "Dialogs.h"
#include "Style.h"
#include "qledindicator.h"

#include "VException.h"

Controller::Controller( QWidget *parent ) :
    QMainWindow( parent ),
    fConfig( vmeplus::V2718::GetDefaultConfig() )
{
    CreateActions();
    CreateCentralWidget();
    CreateDockWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

Controller::~Controller() {}

/****** Constructive methods ******/
void Controller::CreateActions()
{
    // Main actions
    fConnectAction = new QAction( "&Connect", this );
        connect( this, &Controller::Connected, fConnectAction, &QAction::setDisabled );
    fDisconnectAction = new QAction( "&Disconnect", this );
        connect( this, &Controller::Connected, fDisconnectAction, &QAction::setEnabled );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );
    connect( fConnectAction, &QAction::triggered, this, &Controller::OpenConnectDialog );
    connect( fDisconnectAction, &QAction::triggered, this, &Controller::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &Controller::close );

    // View actions
    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    fViewMenu = menuBar()->addMenu( "&View" );
        fViewMenu->addAction( fViewStatusBarAction );
    connect( fViewStatusBarAction, &QAction::triggered, this, &Controller::ToggleStatusBar );

    // Config actions
    fSaveConfigAction = new QAction( "Save config" );
    fLoadConfigAction = new QAction( "Load config" );

    fConfigMenu = menuBar()->addMenu( "&Config" );
        fConfigMenu->addAction( fSaveConfigAction );
        fConfigMenu->addAction( fLoadConfigAction );
    connect( fSaveConfigAction, &QAction::triggered, this, &Controller::SaveConfig );
    connect( fLoadConfigAction, &QAction::triggered, this, &Controller::LoadConfig );

    // Add actions
    fAddDeviceAction = new QAction( "Device" );
        connect( this, &Controller::Connected, fAddDeviceAction, &QAction::setEnabled );
    QMenu *addMenu = menuBar()->addMenu( "&Add" );
        addMenu->addAction( fAddDeviceAction );
    connect( fAddDeviceAction, &QAction::triggered, this, &Controller::OpenDeviceDialog );
}

void Controller::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    fMainTab = new QTabWidget();

    CreateIOTab();
    CreatePulserTab();

    fProgramButton = new QPushButton( "PROGRAM" );
        fProgramButton->setStyleSheet( style::button::neutral );
        connect( this, &Controller::Connected, fProgramButton, &QPushButton::setEnabled );
        connect( fProgramButton, &QPushButton::clicked, this, &Controller::Program );

    vLayout->addWidget( fMainTab );
    vLayout->addWidget( fProgramButton );
    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void Controller::CreateDockWidget()
{
    QDockWidget *dock = new QDockWidget( "Display", this );
    fDisplay = new Display( this, dock );
    dock->setWidget( fDisplay );
    addDockWidget( Qt::RightDockWidgetArea, dock );
    fViewMenu->addAction( dock->toggleViewAction() );
}


void Controller::CreateIOTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Inputs && Outputs") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    QGroupBox *outGroup = new QGroupBox( tr("Outputs") );
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
    outGroup->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );

    QGroupBox *inGroup = new QGroupBox( tr("Inputs") );
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
    inGroup->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );

    vLayout->addWidget( outGroup );
    vLayout->addWidget( inGroup );
    tab->setLayout( vLayout );
}


void Controller::CreatePulserTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Pulser && Scaler") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    // Pulsers
    for( uint8_t i = 0; i < N_PULSERS; ++ i )
    {
        QString pulserName = (i == cvPulserA) ? "Pulser A" : "Pulser B";

        QGroupBox *pulserGroup = new QGroupBox( pulserName );
        QGridLayout *pulLayout = new QGridLayout();

        QLabel* freqLabel = new QLabel( tr("Frequency(Hz):") );
        fPulFreqSpin[i] = new QSpinBox();
            fPulFreqSpin[i]->setRange( 0, 10000000 );

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

        QFrame *buttonFrame = new QFrame(); 
            buttonFrame->setFrameShape( QFrame::StyledPanel );
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        fPulStartButton[i] = new QPushButton( "START" ); 
            fPulStartButton[i]->setStyleSheet( style::button::good );
            connect( this, &Controller::Connected, fPulStartButton[i], &QPushButton::setEnabled );
        fPulStopButton[i] = new QPushButton( "STOP" );
            fPulStopButton[i]->setStyleSheet( style::button::bad );
            connect( this, &Controller::Connected, fPulStopButton[i], &QPushButton::setEnabled );

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
    QGroupBox *scalerGroup = new QGroupBox( "Scaler" );
    QGridLayout *scalLayout = new QGridLayout();

    QLabel *limitLabel = new QLabel( "Limit:" );
    fScalLimitSpin = new QSpinBox();
        fScalLimitSpin->setRange( 0, 1023 );
    fScalAutoCheck = new QCheckBox( "Auto reset" );    


    QFrame *buttonFrame = new QFrame(); 
        buttonFrame->setFrameShape( QFrame::StyledPanel );
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    fScalGateButton = new QPushButton( "GATE" );
        fScalGateButton->setStyleSheet( style::button::neutral );
        connect( this, &Controller::Connected, fScalGateButton, &QPushButton::setEnabled );
    fScalStartButton = new QPushButton( "START" );
        fScalStartButton->setStyleSheet( style::button::good );
        connect( this, &Controller::Connected, fScalStartButton, &QPushButton::setEnabled );
    fScalResetButton = new QPushButton( "RESET" );
        fScalResetButton->setStyleSheet( style::button::neutral );
        connect( this, &Controller::Connected, fScalResetButton, &QPushButton::setEnabled );

    buttonLayout->addWidget( fScalGateButton );
    buttonLayout->addWidget( fScalStartButton );
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

/****** SLOTS ******/
void Controller::OpenConnectDialog()
{
    Connection *cDialog = new Connection( this );
        cDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        cDialog->setModal( true );
        cDialog->resize( 300, 150 );
        cDialog->setWindowTitle( "Connection dialog" );
    cDialog->show();
}

void Controller::OpenDeviceDialog()
{
    DeviceDialog *devDialog = new DeviceDialog( this );
        devDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        devDialog->setModal( true );
        devDialog->setWindowTitle( "Device dialog" );
    devDialog->show();
}

void Controller::UpdateDisplay()
{
    try
    {
        CVDisplay displayInfo = fController.ReadDisplay();
        fDisplay->Update( displayInfo );
    }
    catch( vmeplus::VException &e )
    {
        qInfo() << e.what();
    }
}

void Controller::Program()
{
    CollectConfig();

    try
    {
        fController.WriteConfig( fConfig );
    }
    catch( vmeplus::VException &e )
    {
        qInfo() << e.what();
    }
}

void Controller::CollectConfig()
{
    fConfig = vmeplus::V2718::GetDefaultConfig();

    // In's and Out's
    for( uint8_t i = 0; i < N_INS; ++i )
    {
        fConfig.at("settings").at("inputs").at( i ).at("polarity") = fInPolCombo[i]->currentData().toInt();
        fConfig.at("settings").at("inputs").at( i ).at("led_polarity") = fInLedCombo[i]->currentData().toInt();
    }

    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        fConfig.at("settings").at("outputs").at( i ).at("polarity") = fOutPolCombo[i]->currentData().toInt();
        fConfig.at("settings").at("outputs").at( i ).at("led_polarity") = fOutLedCombo[i]->currentData().toInt();
        fConfig.at("settings").at("outputs").at( i ).at("source") = fOutSrcCombo[i]->currentData().toInt();
    }

    // Pulsers
    for( uint8_t i = 0; i < N_PULSERS; ++i )
    {
        json::json_pointer id( (i == cvPulserA) ? "/A" : "/B" );
        fConfig.at("settings").at("pulsers").at(id).at("frequency") = fPulFreqSpin[i]->value();
        fConfig.at("settings").at("pulsers").at(id).at("duty") = fPulDutySpin[i]->value();
        fConfig.at("settings").at("pulsers").at(id).at("count") = fPulNSpin[i]->value();
        fConfig.at("settings").at("pulsers").at(id).at("start") = fPulStartCombo[i]->currentData().toInt();
        fConfig.at("settings").at("pulsers").at(id).at("stop") = fPulStopCombo[i]->currentData().toInt();
    }

    // Scaler
    fConfig.at("settings").at("scaler").at("gate") = fScalGateCombo->currentData().toInt();
    fConfig.at("settings").at("scaler").at("stop") = fScalResetCombo->currentData().toInt();
    fConfig.at("settings").at("scaler").at("hit") = fScalHitCombo->currentData().toInt();
    fConfig.at("settings").at("scaler").at("limit") = fScalLimitSpin->value();
    fConfig.at("settings").at("scaler").at("auto_reset") = fScalAutoCheck->isChecked();
}

void Controller::SpreadConfig()
{
    auto changeCombo = [this]( QComboBox* c, std::string s ) {
        int data;
        json::json_pointer p( s );
        int index = c->findData( fConfig.at( p ).get_to( data ) );
        c->setCurrentIndex( index );
    };
    // In's and Out's
    for( uint8_t i = 0; i < N_INS; ++i )
    {
        changeCombo( fInPolCombo[i], "/settings/inputs/" + std::to_string(i) + "polarity" );
        changeCombo( fInLedCombo[i], "/settings/inputs/" + std::to_string(i) + "led_polarity" );
    }

    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        changeCombo( fOutPolCombo[i], "/settings/outputs/" + std::to_string(i) + "polarity" );
        changeCombo( fOutLedCombo[i], "/settings/outputs/" + std::to_string(i) + "led_polarity" );
        changeCombo( fOutSrcCombo[i], "/settings/outputs/" + std::to_string(i) + "source" );
    }

    //// Pulsers
    //for( uint8_t i = 0; i < N_PULSERS; ++i )
    //{
    //    json::json_pointer id( (i == cvPulserA) ? "/A" : "/B" );
    //    fConfig.at("settings").at("pulsers").at(id).at("frequency") = fPulFreqSpin[i]->value();
    //    fConfig.at("settings").at("pulsers").at(id).at("duty") = fPulDutySpin[i]->value();
    //    fConfig.at("settings").at("pulsers").at(id).at("count") = fPulNSpin[i]->value();
    //    fConfig.at("settings").at("pulsers").at(id).at("start") = fPulStartCombo[i]->currentData().toInt();
    //    fConfig.at("settings").at("pulsers").at(id).at("stop") = fPulStopCombo[i]->currentData().toInt();
    //}

    //// Scaler
    //fConfig.at("settings").at("scaler").at("gate") = fScalGateCombo->currentData().toInt();
    //fConfig.at("settings").at("scaler").at("stop") = fScalResetCombo->currentData().toInt();
    //fConfig.at("settings").at("scaler").at("hit") = fScalHitCombo->currentData().toInt();
    //fConfig.at("settings").at("scaler").at("limit") = fScalLimitSpin->value();
    //fConfig.at("settings").at("scaler").at("auto_reset") = fScalAutoCheck->isChecked();
}

void Controller::SaveConfig()
{
    QString fileName = QFileDialog::getSaveFileName( this, "Save Config file", "./", "Text files (*.json)" );

    if( fileName.isEmpty() )
    {
        return;
    }
    else
    {
        CollectConfig();

        QFile file( fileName );

        if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {
            return;
        }

        QTextStream out( &file );
        out << fConfig.dump( 2 ).c_str();
        file.close();
    }
}

void Controller::LoadConfig()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Load Config file", "./", "Text files (*.json)" );

    if( fileName.isEmpty() )
    {
        return;
    }
    else
    {
        QFile file( fileName );

        if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            return;
        }

        QTextStream in( &file );

        QString s = in.readAll();

        fConfig = json::parse( s.toStdString() );

        SpreadConfig();
    }
}

void Controller::Connect( short link, short conet )
{
    qInfo() << "Openning controller...";
    //Exception may be thrown here
    fController.Open( link, conet );
    qInfo() << "\n...OK";

    setWindowTitle( "gVME++ (V2718)" );
    statusBar()->showMessage( "Connected..." );

    emit Connected( true );
}

void Controller::Disconnect()
{
    qInfo() << "Disconnecting...";

    try
    {
        fController.Close();
    }
    catch( const vmeplus::VException& e )
    {
        qInfo() << "On disconnection : " << e.what();
    }

    setWindowTitle( "gVME++" );
    statusBar()->showMessage( "Disconnected..." );

    emit Connected( false );
}

void Controller::closeEvent( QCloseEvent *event )
{
    const QMessageBox::StandardButton ret =
        QMessageBox::warning( this,
                              tr( "Exit" ),
                              tr( "Are you sure?" ),
                              QMessageBox::Ok | QMessageBox::Cancel );
    switch( ret )
    {
        case( QMessageBox::Ok ) :
            event->accept();
            break;
        default :
            event->ignore();
            break;
    }
}

void Controller::ToggleStatusBar()
{
    if( fViewStatusBarAction->isChecked() )
    {
        statusBar()->show();
    }
    else
    {
        statusBar()->hide();
    }
}


Display::Display( Controller *controller, QWidget *parent ) :
    QWidget( parent ),
    fController( controller )
{
    CreateDisplay();
}

Display::~Display()
{
}

void Display::CreateDisplay()
{
    QVBoxLayout *layout = new QVBoxLayout();

    QFrame *upperFrame = new QFrame();
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

    QFrame *lowerFrame = new QFrame();
    QGridLayout *lowerLayout = new QGridLayout();

    int ledSize = 14;
    int row = 0;
    for( int i = 0; i < N_AM; ++i, ++row )
    {
        fAddressModLED[i] = new QLedIndicatorWithLabel( ledSize, QString( "AM%1" ).arg( i ) );
        lowerLayout->addWidget( fAddressModLED[i], i, 0 );
    }

    fDS1LED = new QLedIndicatorWithLabel( ledSize, "DS1" );
    fDS2LED = new QLedIndicatorWithLabel( ledSize, "DS2" );
    lowerLayout->addWidget( fDS1LED, row++, 0 );
    lowerLayout->addWidget( fDS2LED, row++, 0 );

    fASLED = new QLedIndicatorWithLabel( ledSize, "AS" );
    lowerLayout->addWidget( fASLED, row++, 0 );

    fIACKLED = new QLedIndicatorWithLabel( ledSize, "IACK" );
    lowerLayout->addWidget( fIACKLED, row++, 0 );

    fWriteLED = new QLedIndicatorWithLabel( ledSize, "WRITE" );
    lowerLayout->addWidget( fWriteLED, row++, 0 );

    fLwordLED = new QLedIndicatorWithLabel( ledSize, "LWORD" );
    lowerLayout->addWidget( fLwordLED, row++, 0 );

    row = 0;
    for( int i = 0; i < N_IRQ; ++i, ++row )
    {
        fIRQLED[i] = new QLedIndicatorWithLabel( ledSize, QString( "IRQ%1" ).arg( i + 1 ) );
        lowerLayout->addWidget( fIRQLED[i], i, 1 );
    }

    fBreqLED = new QLedIndicatorWithLabel( ledSize, "BREQ" );
    lowerLayout->addWidget( fBreqLED, row++, 1 );

    fBgntLED = new QLedIndicatorWithLabel( ledSize, "BGNT" );
    lowerLayout->addWidget( fBgntLED, row++, 1 );

    fSresLED = new QLedIndicatorWithLabel( ledSize, "SRES" );
    lowerLayout->addWidget( fSresLED, row++, 1 );

    fDTKLED = new QLedIndicatorWithLabel( ledSize, "DTK" );
    lowerLayout->addWidget( fDTKLED, row++, 1 );

    fBERRLED = new QLedIndicatorWithLabel( ledSize, "BERR" );
    lowerLayout->addWidget( fBERRLED, row++, 1 );

    lowerLayout->setSpacing( 1 );
    lowerFrame->setLayout( lowerLayout );
    layout->addWidget( lowerFrame );

    fUpdateButton = new QPushButton( "UPDATE" );
        fUpdateButton->setStyleSheet( style::button::neutral );
    connect( fUpdateButton, &QPushButton::clicked, fController, &Controller::UpdateDisplay );
    connect( fController, &Controller::Connected, fUpdateButton, &QPushButton::setEnabled );
    layout->addWidget( fUpdateButton );

    setLayout( layout );
}


void Display::Update( const CVDisplay &display )
{
    fAddressText->setText( QString().setNum( display.cvAddress, 16 ) );
    fDataText->setText( QString().setNum( display.cvData, 16 ) );

    for( unsigned i = 0; i < N_AM; ++i )
    {
        fAddressModLED[i]->SetChecked( ((1 << i) & display.cvAM) ? Qt::Checked : Qt::Unchecked );
    }

    for( unsigned i = 0; i < N_IRQ; ++i )
    {
        fIRQLED[i]->SetChecked( ((1 << i) & display.cvIRQ) ? Qt::Checked : Qt::Unchecked );
    }

    fASLED->SetChecked( display.cvAS ? Qt::Checked : Qt::Unchecked );
    fIACKLED->SetChecked( display.cvIACK ? Qt::Checked : Qt::Unchecked );
    fDS1LED->SetChecked( display.cvDS0 ? Qt::Checked : Qt::Unchecked );
    fDS2LED->SetChecked( display.cvDS1 ? Qt::Checked : Qt::Unchecked );
    fWriteLED->SetChecked( display.cvWRITE ? Qt::Checked : Qt::Unchecked );
    fLwordLED->SetChecked( display.cvLWORD ? Qt::Checked : Qt::Unchecked );
    fBreqLED->SetChecked( display.cvBR ? Qt::Checked : Qt::Unchecked );
    fBgntLED->SetChecked( display.cvBG ? Qt::Checked : Qt::Unchecked );
    fSresLED->SetChecked( display.cvSYSRES ? Qt::Checked : Qt::Unchecked );
    fDTKLED->SetChecked( display.cvDTACK ? Qt::Checked : Qt::Unchecked );
    fBERRLED->SetChecked( display.cvBERR ? Qt::Checked : Qt::Unchecked );
}
