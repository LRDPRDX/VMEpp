#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QDebug>
#include <QFrame>
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
    resize( 300, 150 );

    connect( fParent, &V2718Window::Connected, this, &DeviceWindow::OnControllerDisconnect );

    fProgramButton = new QPushButton( "PROGRAM" );
        fProgramButton->setStyleSheet( style::button::good );
        connect( this, &DeviceWindow::Connected, fProgramButton, &QPushButton::setEnabled );
        connect( fProgramButton, &QPushButton::clicked, this, &DeviceWindow::Program );

    fReadButton = new QPushButton( "READ" );
        fReadButton->setStyleSheet( style::button::good );
        connect( this, &DeviceWindow::Connected, fReadButton, &QPushButton::setEnabled );
        connect( fReadButton, &QPushButton::clicked, this, &DeviceWindow::ReadConfig );

    fBottomFrame = new QFrame(); 
        fBottomFrame->setFrameShape( QFrame::StyledPanel );
    QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget( fProgramButton );
        buttonLayout->addWidget( fReadButton );
    fBottomFrame->setLayout( buttonLayout );

    CreateFileMenu();
}

DeviceWindow::~DeviceWindow()
{
    delete fDevice;
}

void DeviceWindow::CreateFileMenu()
{
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
}

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
