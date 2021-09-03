#include "Style.h"


//********************
//****** SFRAME ******
//********************
SFrame::SFrame( SColor_t color, QWidget *parent ) :
    QFrame( parent )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    setObjectName( "SFrame" );

    SColor c( color );
    QString s = QString( "#SFrame {border: 2px solid %1; border-radius: 4px}" ).arg( c.light );
    setStyleSheet( s );
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
    QString s = QString( "QPushButton {background-color:%1; color:%4; border-radius : 4px;"
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
