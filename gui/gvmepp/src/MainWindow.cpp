#include <QStatusBar>
#include <QLabel>
#include <QPixmap>
#include <QMessageBox>
#include <QCloseEvent>

#include "MainWindow.h"

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent )
{
    CreateMenu();
    CreateCentralWidget();

    statusBar()->showMessage( "Ready..." );
}

void MainWindow::CreateMenu()
{
}

void MainWindow::CreateCentralWidget()
{
    fLogoLabel = new QLabel();
    QPixmap pixmap( "../../pics/small.xpm" );
    fLogoLabel->setPixmap( pixmap );
    setCentralWidget( fLogoLabel );
    resize( pixmap.width(), pixmap.height() );
}

void MainWindow::closeEvent( QCloseEvent *event )
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
