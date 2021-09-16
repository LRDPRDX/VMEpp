#include "Style.h"


//********************
//****** SFRAME ******
//********************
SFrame::SFrame( SColor_t color, QWidget *parent ) :
    QFrame( parent )
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum );
    setObjectName( "SFrame" );
}

SFrame::~SFrame()
{
}


//*********************
//****** SBUTTON ******
//*********************
SButton::SButton( const QString& text, SColor_t color, QWidget *parent ) :
    QPushButton( text, parent )
{
    SColor c( color );
    QString s = QString( "QPushButton {background-color:%1; color:%4; border-radius : 4px; border: 1px solid #000000;"
                         "padding: 3px}"
                         "QPushButton:disabled {background-color:%2; color:%5;}"
                         "QPushButton:pressed {background-color:%3; color:%6}" ).arg( c.normal,
                                                                                      c.light,
                                                                                      c.dark,
                                                                                      style::white,
                                                                                      style::white,
                                                                                      style::white );
    setStyleSheet( s );
}

SButton::~SButton()
{
}
