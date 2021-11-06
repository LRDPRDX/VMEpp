#pragma once

#include <QString>
#include <QFrame>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>

namespace style
{
    static const QString white          =      "#ffffff";
    static const QString black          =      "#191919";
    static const QString gray           =      "#808080";
    static const QString darkGray       =      "#353535";
    static const QString violet         =      "#8888ff";
    static const QString red            =      "#ff8888";
    static const QString green          =      "#88ff88";
    static const QString yellow         =      "#c3ff88";
    static const QString pink           =      "#ff88c3";
    static const QString blue           =      "#88c3ff";
}

inline void ColorButton( QPushButton* button, QColor color )
{
    QPalette palette = button->palette();
    palette.setColor( QPalette::Active, QPalette::ButtonText, color );
    button->setPalette( palette );
    button->update();
}

enum class SColor_t { VIOLET, RED, GREEN, YELLOW, PINK, BLUE };
enum class SMode_t  { NORMAL, LIGHT, DARK };

class SFrame : public QFrame
{
    Q_OBJECT

    public :
        SFrame( SColor_t color, QWidget* parent = nullptr );
        ~SFrame() override = default;
};

class SGroupBox : public QGroupBox
{
    Q_OBJECT

    public :
        SGroupBox( const QString& title, QWidget* parent = nullptr );
        ~SGroupBox() override = default;
};

class SLedIndicator : public QLabel
{
    Q_OBJECT

    protected :
        QColor   fColor;
        int      fSize;

    public:
        SLedIndicator( int size = 14, QWidget *parent = 0 );
        ~SLedIndicator() override = default;

    public slots:
        void SetState( bool state );
        void SetColor( QColor color ) { fColor = color; }
};

class SLedIndicatorWithLabel : public QWidget
{
    private :
        SLedIndicator *fLED;

    public :
        SLedIndicatorWithLabel( QString text = "", bool leftAlign = true, int size = 14, QWidget *parent = nullptr );
        ~SLedIndicatorWithLabel() override = default;

        void SetChecked( bool status ) { fLED->SetState( status ); }
        void SetColor( QColor color ) { fLED->SetColor( color ); }
};
