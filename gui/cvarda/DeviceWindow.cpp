#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QStatusBar>

#include <VException.h>

#include "DeviceWindow.h"
#include "Controller.h"


DeviceWindow::DeviceWindow( Controller *parent ) :
    QMainWindow( parent ),
    fParent( parent ),
    fDevice( nullptr )
{
    resize( 300, 150 );
    setWindowTitle( "V895" );

    connect( fParent, &Controller::Connected, this, &DeviceWindow::OnControllerDisconnect );

    fProgramButton = new QPushButton( "PROGRAM" );
        connect( this, &DeviceWindow::Connected, fProgramButton, &QPushButton::setEnabled );

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

DeviceWindow::~DeviceWindow()
{
    delete fDevice;
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
