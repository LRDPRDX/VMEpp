#include <QApplication>

//#include "V2718Window.h"
#include "MainWindow.h"

#include "Style.h"

int main( int argc, char *argv[] )
{

    QApplication app( argc, argv );

    //V2718Window window;
    MainWindow window;

    window.setWindowTitle( "gVME++" );
    window.show();

    qApp->setStyleSheet( ss::groupBox );
    qApp->setStyleSheet( qApp->styleSheet().append (ss::frame) );

    return app.exec();
}
