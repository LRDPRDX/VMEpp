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
#include "Dialogs.h"
#include "Style.h"

using namespace vmepp;


DeviceWindow::DeviceWindow( QWidget *parent ) :
    QMainWindow( parent )
{
    CreateMenu();
    CreateBottomFrame();

    connect( this, &DeviceWindow::Error, this, &DeviceWindow::OnError );

    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

//**********************************
//****** CONSTRUCTIVE METHODS ******
//**********************************
void DeviceWindow::CreateBottomFrame()
{
    fWriteButton = new QPushButton( "WRITE" );
        ColorButton( fWriteButton, QColor( style::yellow ) );
        connect( this, &DeviceWindow::Connected, fWriteButton, &QPushButton::setEnabled );
        connect( fWriteButton, &QPushButton::clicked, this, &DeviceWindow::WriteConfig );

    fReadButton = new QPushButton( "READ" );
        ColorButton( fReadButton, QColor( style::yellow ) );
        connect( this, &DeviceWindow::Connected, fReadButton, &QPushButton::setEnabled );
        connect( fReadButton, &QPushButton::clicked, this, &DeviceWindow::ReadConfig );

    fBottomFrame = new SFrame( SColor_t::VIOLET );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget( fWriteButton );
        buttonLayout->addWidget( fReadButton );
    fBottomFrame->setLayout( buttonLayout );
}

void DeviceWindow::CreateMenu()
{
    // File
    fConnectAction = new QAction( "&Connect", this );
        connect( this, &DeviceWindow::Connected, fConnectAction, &QAction::setDisabled );
    fDisconnectAction = new QAction( "&Disconnect", this );
        connect( this, &DeviceWindow::Connected, fDisconnectAction, &QAction::setEnabled );
    fExitAction = new QAction( "&Exit", this );

    fFileMenu = menuBar()->addMenu( "&File" );
        fFileMenu->addAction( fConnectAction );
        fFileMenu->addAction( fDisconnectAction );
        fFileMenu->addSeparator();
        fFileMenu->addAction( fExitAction );
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
    // Config
    fSaveConfigAction = new QAction( "Save" );
    fLoadConfigAction = new QAction( "Load" );

    fConfigMenu = menuBar()->addMenu( "&Config" );
        fConfigMenu->addAction( fSaveConfigAction );
        fConfigMenu->addAction( fLoadConfigAction );
}

//*******************
//****** SLOTS ******
//*******************
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

void DeviceWindow::OnError( const VException& error )
{
    statusBar()->showMessage( QString( "Error %1 : %2" ).arg( QString::number( static_cast<int>(error.GetErrorCode()) ),
                                                              QString::fromStdString( error.GetErrorMessage() ) ) );
    this->DoOnError( error );
 
    ErrorMessageBox* msg = new ErrorMessageBox( error, this );
    msg->show();
}
