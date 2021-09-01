#include <QApplication>

#include "V2718Window.h"

int main( int argc, char *argv[] )
{

    QApplication app( argc, argv );

    V2718Window window;

    window.resize( 350, 250 );
    window.setWindowTitle( "gVME++" );
    window.show();

    return app.exec();
}
