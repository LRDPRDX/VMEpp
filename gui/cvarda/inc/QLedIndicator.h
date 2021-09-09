#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QString>

class QLedIndicator : public QLabel
{
    Q_OBJECT

    public:
        static constexpr int size = 14;

        QLedIndicator( QWidget *parent = 0 );
        virtual ~QLedIndicator();
        
    public slots:
        void setState( bool state );
};

class QLedIndicatorWithLabel : public QWidget
{
    private :
        QLedIndicator *fLED;

    public :
        QLedIndicatorWithLabel( QString text = "", bool leftAlign = true, QWidget *parent = nullptr );
        virtual ~QLedIndicatorWithLabel();

        void SetChecked( bool status ) { fLED->setState( status ); }
};

#endif // QLEDLABEL_H
