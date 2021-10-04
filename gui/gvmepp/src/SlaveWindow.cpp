#include <QStatusBar>
#include <QAction>

#include "SlaveWindow.h"
#include "V2718Window.h"


SlaveWindow::SlaveWindow( V2718Window *parent ) :
    DeviceWindow( parent ),
    fParent( parent ),
    fDevice( nullptr )
{
    connect( fParent, &V2718Window::Connected, this, &SlaveWindow::OnControllerDisconnect );

    CreateMenu();
}

SlaveWindow::~SlaveWindow()
{
    if( fDevice )
    {
        delete fDevice;
    }
}


//**********************************
//****** CONSTRUCTIVE METHODS ******
//**********************************
void SlaveWindow::CreateMenu()
{
    connect( fParent, &V2718Window::Connected, fConnectAction, &QAction::setEnabled );
}

//*******************
//****** SLOTS ******
//*******************
void SlaveWindow::Connect()
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

void SlaveWindow::Disconnect()
{
    fDevice->Release();
    statusBar()->showMessage( "Disconnected..." );
    emit Connected( false );
}

void SlaveWindow::OnControllerDisconnect( bool status )
{
    if( not status )
    {
        Disconnect();
    }
}

void SlaveWindow::DoOnError( const VException& error )
{
}
