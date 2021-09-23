#pragma once

#include <QString>
#include <QFrame>
#include <QPushButton>
#include <QLabel>

namespace style 
{
    static const QString white          =      "#ffffff";
    static const QString black          =      "#000000";
    static const QString grey           =      "#a3a3a3";
    static const QString greyLight      =      "#c9c9c9";

    static const QString violet         =      "#8888ff";
    static const QString violetLight    =      "#bdbfff";
    static const QString violetDark     =      "#7070ff";

    static const QString red            =      "#ff8888";
    static const QString redLight       =      "#ffbdbf";
    static const QString redDark        =      "#ff7070";

    static const QString green          =      "#88ff88";
    static const QString greenLight     =      "#bfffbd";
    static const QString greenDark      =      "#70ff70";

    static const QString yellow         =      "#c3ff88";
    static const QString yellowLight    =      "#e0ffbd";
    static const QString yellowDark     =      "#b8ff70";

    static const QString pink           =      "#ff88c3";
    static const QString pinkLight      =      "#ffbde0";
    static const QString pinkDark       =      "#ff70b8";

    static const QString blue           =      "#88c3ff";
    static const QString blueLight      =      "#bde0ff";
    static const QString blueDark       =      "#70b8ff";
}

enum class SColor_t { VIOLET, RED, GREEN, YELLOW, PINK, BLUE };
enum class SMode_t  { NORMAL, LIGHT, DARK };

struct SColor
{
    QString normal; 
    QString light; 
    QString dark; 

    SColor( SColor_t color )
    {
        switch( color )
        {
            case( SColor_t::VIOLET ) :
                normal = style::violet; light = style::violetLight; dark = style::violetDark; break;
            case( SColor_t::RED ) :
                normal = style::red; light = style::redLight; dark = style::redDark; break;
            case( SColor_t::GREEN ) :
                normal = style::green; light = style::greenLight; dark = style::greenDark; break;
            case( SColor_t::YELLOW ) :
                normal = style::yellow; light = style::yellowLight; dark = style::yellowDark; break;
            case( SColor_t::PINK ) :
                normal = style::pink; light = style::pinkLight; dark = style::pinkDark; break;
            case( SColor_t::BLUE ) :
                normal = style::blue; light = style::blueLight; dark = style::blueDark; break;
        }
    }
};

namespace ss
{
    static const QString groupBox = QString( "QGroupBox {border:2px solid %1; margin-top:3ex;"
                                             "border-top-right-radius:5px; border-bottom-left-radius:5px;"
                                             "border-bottom-right-radius:5px;}" 
                                             "QGroupBox::title {subcontrol-origin: margin; padding: 0 3px;"
                                             "background-color:%1; border-top-left-radius:5px;"
                                             "border-top-right-radius:5px; color:#ffffff}" ).arg( style::blue );

    static const QString frame = QString( "SFrame {border: 1px solid %1; border-radius: 4px}" ).arg( style::blue );
}

class SFrame : public QFrame
{
    Q_OBJECT

    public :
        SFrame( SColor_t color, QWidget *parent = nullptr );
        ~SFrame() override = default;
};

class SButton : public QPushButton
{
    Q_OBJECT

    public :
        SButton( const QString& text, SColor_t color, QWidget *parent = nullptr );
        ~SButton() override = default;
};

class SLedIndicator : public QLabel
{
    Q_OBJECT

    protected :
        SColor_t fColor;
        int      fSize;
        
    public:
        SLedIndicator( int size = 14, QWidget *parent = 0 );
        ~SLedIndicator() override = default;
        
    public slots:
        void SetState( bool state );
        void SetColor( SColor_t color ) { fColor = color; }
};

class SLedIndicatorWithLabel : public QWidget
{
    private :
        SLedIndicator *fLED;

    public :
        SLedIndicatorWithLabel( QString text = "", bool leftAlign = true, int size = 14, QWidget *parent = nullptr );
        ~SLedIndicatorWithLabel() override = default;

        void SetChecked( bool status ) { fLED->SetState( status ); }
        void SetColor( SColor_t color ) { fLED->SetColor( color ); }
};
