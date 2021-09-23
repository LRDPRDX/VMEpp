#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QDebug>
#include <QHBoxLayout>

#include <VException.h>

#include "DeviceWindow.h"
#include "V2718Window.h"
#include "Style.h"


DeviceWindow::DeviceWindow( V2718Window *parent ) :
    QMainWindow( parent ),
    fParent( parent ),
    fDevice( nullptr )
{
    connect( fParent, &V2718Window::Connected, this, &DeviceWindow::OnControllerDisconnect );

    CreateFileMenu();
    CreateBottomFrame();

    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

DeviceWindow::~DeviceWindow()
{
    delete fDevice;
}


//**********************************
//****** CONSTRUCTIVE METHODS ******
//**********************************
void DeviceWindow::CreateBottomFrame()
{
    fWriteButton = new SButton( "WRITE", SColor_t::VIOLET );
        connect( this, &DeviceWindow::Connected, fWriteButton, &QPushButton::setEnabled );
        connect( fWriteButton, &QPushButton::clicked, this, &DeviceWindow::WriteConfig );

    fReadButton = new SButton( "READ", SColor_t::VIOLET );
        connect( this, &DeviceWindow::Connected, fReadButton, &QPushButton::setEnabled );
        connect( fReadButton, &QPushButton::clicked, this, &DeviceWindow::ReadConfig );

    fBottomFrame = new SFrame( SColor_t::VIOLET ); 

    QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget( fWriteButton );
        buttonLayout->addWidget( fReadButton );
    fBottomFrame->setLayout( buttonLayout );
}

void DeviceWindow::CreateFileMenu()
{
    // File menu
    fConnectAction = new QAction( "&Connect", this );
        connect( this, &DeviceWindow::Connected, fConnectAction, &QAction::setDisabled );
    fDisconnectAction = new QAction( "&Disconnect", this );
        connect( this, &DeviceWindow::Connected, fDisconnectAction, &QAction::setEnabled );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );
    connect( fConnectAction, &QAction::triggered, this, &DeviceWindow::Connect );
    connect( fDisconnectAction, &QAction::triggered, this, &DeviceWindow::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &DeviceWindow::close );
    // View
    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    fViewMenu = menuBar()->addMenu( "View" );
        fViewMenu->addAction( fViewStatusBarAction );
    connect( fViewStatusBarAction, &QAction::triggered, this, &DeviceWindow::ToggleStatusBar );
    // Config actions
    fSaveConfigAction = new QAction( "Save" );
    fLoadConfigAction = new QAction( "Load" );

    fConfigMenu = menuBar()->addMenu( "&Config" );
        fConfigMenu->addAction( fSaveConfigAction );
        fConfigMenu->addAction( fLoadConfigAction );
}

//*******************
//****** SLOTS ******
//*******************
void DeviceWindow::Connect()
{
    bool success = true;
    try
    {
        fParent->fController.RegisterSlave( fDevice );
    }
    catch( const vmeplus::VException &e )
    {
        success = false;
        QMessageBox::warning( this,
                              tr( "Connection FAILED" ),
                              tr( e.what() ),
                              QMessageBox::Ok );
    }
    statusBar()->showMessage( success ? "Connected..." : "Disconnected..." );
    emit Connected( success );
}

void DeviceWindow::OnControllerDisconnect( bool status )
{
    if( not status )
    {
        Disconnect();
    }
}

void DeviceWindow::Disconnect()
{
    fDevice->Release();
    statusBar()->showMessage( "Disconnected..." );
    emit Connected( false );
}

void DeviceWindow::closeEvent( QCloseEvent *event )
{
    const QMessageBox::StandardButton ret =
        QMessageBox::warning( this,
                              tr( "Exit" ),
                              tr( "Are you sure?" ),
                              QMessageBox::Ok | QMessageBox::Cancel );
    switch( ret )
    {
        case( QMessageBox::Ok ) :
            Disconnect();
            event->accept();
            break;
        default :
            event->ignore();
            break;
    }
}

void DeviceWindow::ToggleStatusBar()
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
