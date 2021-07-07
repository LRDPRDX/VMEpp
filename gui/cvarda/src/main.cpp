#include <QApplication>

#include "Controller.h"

int main( int argc, char *argv[] )
{

    QApplication app( argc, argv );

    Controller window;

    window.resize( 350, 250 );
    window.setWindowTitle( "gVME++" );
    window.show();

    return app.exec();
}
