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

//***********************
//****** SGROUPBOX ******
//***********************
SGroupBox::SGroupBox( const QString& title, QWidget* parent ) :
    QGroupBox( title, parent )
{
    SColor c( SColor_t::VIOLET );

    QString s = QString( "QGroupBox {border:2px solid %1; margin-top:3ex;}" ).arg( c.normal );

    QString t = QString( "QGroupBox::title {subcontrol-origin: margin; padding: 0 3px; background-color:%1; border-top-left-radius:5px; border-top-right-radius:5px; color:#ffffff}" ).arg(c.normal);

    setStyleSheet( s + t );
};

SGroupBox::~SGroupBox()
{
}
