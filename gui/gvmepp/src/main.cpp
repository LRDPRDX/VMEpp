#include <QApplication>

#include "V2718Window.h"

#include "Style.h"

int main( int argc, char *argv[] )
{

    QApplication app( argc, argv );

    V2718Window window;

    window.resize( 350, 250 );
    window.setWindowTitle( "gVME++" );
    window.show();

    qApp->setStyleSheet( ss::groupBox );
    qApp->setStyleSheet( qApp->styleSheet().append (ss::frame) );

    return app.exec();
}