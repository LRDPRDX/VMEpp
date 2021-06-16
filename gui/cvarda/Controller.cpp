#include "Controller.h"
#include "Dialogs.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "VException.h"

Controller::Controller( QWidget *parent ) :
    QMainWindow( parent )
{
    CreateActions();

    statusBar()->showMessage( "Ready..." );
}

Controller::~Controller() {}

/****** Constructive methods ******/
void Controller::CreateActions()
{
    fConnectAction = new QAction( "&Connect", this );
    fDisconnectAction = new QAction( "&Disconnect", this );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );

    connect( fConnectAction, &QAction::triggered, this, &Controller::OpenConnectDialog );
    connect( fDisconnectAction, &QAction::triggered, this, &Controller::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &Controller::close );

    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    QMenu *viewMenu = menuBar()->addMenu( "&View" );
        viewMenu->addAction( fViewStatusBarAction );

    connect( fViewStatusBarAction, &QAction::triggered, this, &Controller::ToggleStatusBar );
}

void Controller::OpenConnectDialog()
{
    Connection *cDialog = new Connection( this );
        cDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        cDialog->setModal( true );
        cDialog->resize( 300, 150 );
        cDialog->setWindowTitle( "Connection dialog" );
    cDialog->show();
}

void Controller::Connect( short link, short conet )
{
    qInfo() << "Openning controller...";
    //Exception may be thrown here
    fController.Open( link, conet );
    qInfo() << "\n...OK";

    setWindowTitle( "gVME++ (V2718)" );
    statusBar()->showMessage( "Connected..." );

    emit Connected(); 
}

void Controller::Disconnect()
{
    qInfo() << "Disconnecting...";
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
