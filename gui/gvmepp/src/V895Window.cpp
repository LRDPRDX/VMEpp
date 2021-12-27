#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>
#include "qnamespace.h"

#include "V2718Window.h"
#include "V895Window.h"
#include "Style.h"

#include "VException.h"
#include "modules/V895.h"

using namespace vmepp;


V895Window::V895Window( uint32_t address, V2718Window *parent ) :
    SlaveWindow( parent )
{
    fDevice = new V895( address );

    setWindowTitle( "V895" );
    CreateActions();
    CreateCentralWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

void V895Window::CreateActions()
{
    connect( fSaveConfigAction, &QAction::triggered, this, &DeviceWindow::SaveConfig<V895> );
    connect( fLoadConfigAction, &QAction::triggered, this, &DeviceWindow::LoadConfig<V895> );
}

void V895Window::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    QHBoxLayout *chLayout = new QHBoxLayout();
    SGroupBox *chGroup = new SGroupBox( "Channels" );

    for( int nG = 0; nG < N_GROUPS; ++nG )
    {
        SGroupBox *groupOfEight = new SGroupBox( QString::number(nG * N_CH_IN_GROUP) +
                                                 "-" + QString::number((nG + 1) * N_CH_IN_GROUP - 1) );
        QGridLayout *gridLayout = new QGridLayout();
        for( uint8_t i = 0; i < N_CH_IN_GROUP; ++i )
        {
            uint8_t ch = i + N_CH_IN_GROUP * nG;
            QLabel* thrLabel = new QLabel( QString("Threshold %1 :").arg( ch ) );
            fThrSpin[ch] = new QSpinBox();
                fThrSpin[ch]->setRange( 0, 0xFF );
                fThrSpin[ch]->setValue( 0xFF );

            fEnableCheck[ch] = new QCheckBox( "Enable" );

            gridLayout->addWidget( thrLabel, i, 0, Qt::AlignRight );
            gridLayout->addWidget( fThrSpin[ch], i, 1 );
            gridLayout->addWidget( fEnableCheck[ch], i, 2 );
            if( i == N_CH_IN_GROUP - 1 )
            {
                QLabel* widthLabel = new QLabel( "Width :" );
                fWidthSpin[nG] = new QSpinBox();
                    fWidthSpin[nG]->setRange( 4, 40 );
                gridLayout->addWidget( widthLabel, i, 3, Qt::AlignRight );
                gridLayout->addWidget( fWidthSpin[nG], i, 4 );
            }
        }
        groupOfEight->setLayout( gridLayout );
        chLayout->addWidget( groupOfEight );
    }
    chGroup->setLayout( chLayout );
    vLayout->addWidget( chGroup );

    SGroupBox *commonGroup = new SGroupBox( "Common" );
    QHBoxLayout *commLayout = new QHBoxLayout();

    QLabel *majLabel = new QLabel( "Majority level: " );

    fMajLevelSpin = new QSpinBox();
        fMajLevelSpin->setRange( 0, N_CH );
        fMajLevelSpin->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );

    fTestButton = new QPushButton( "TEST" );
        ColorButton( fTestButton, style::yellow );
        fTestButton->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
        connect( this, &V895Window::Connected, fTestButton, &QPushButton::setEnabled );
        connect( fTestButton, &QPushButton::clicked, this, &V895Window::SendTest );

    commLayout->addWidget( majLabel, 1, Qt::AlignRight );
    commLayout->addWidget( fMajLevelSpin, 10, Qt::AlignLeft );
    commLayout->addWidget( fTestButton, 1 );
    commonGroup->setLayout( commLayout );
    commonGroup->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    vLayout->addWidget( commonGroup );

    vLayout->addWidget( fBottomFrame );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V895Window::SendTest()
{
    try
    {
        static_cast<V895*>(fDevice)->SendTest();
    }
    catch( const VException &e )
    {
        emit Error( e );
    }
}

void V895Window::WriteConfig()
{
    try
    {
        UConfig<V895> cfg = qvariant_cast< UConfig<V895> >( this->CollectConfig() );
        static_cast<V895*>(fDevice)->WriteConfig( cfg );
        emit Programmed( true );
    }
    catch( const VException& e )
    {
        emit Programmed( false );
        emit Error( e );
    }
}

void V895Window::ReadConfig()
{
    UConfig<V895> cfg;

    try
    {
        static_cast<V895*>(fDevice)->ReadConfig( cfg );
        QVariant qv; qv.setValue( cfg );
        SpreadConfig( qv );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V895Window::SpreadConfig( const QVariant& qConfig )
{
    UConfig<V895> cfg = qvariant_cast< UConfig<V895> >( qConfig );
    for( uint8_t i = 0; i < N_CH; ++i )
    {
        fThrSpin[i]->setValue( cfg.THRESHOLDS.at( i ) );
        fEnableCheck[i]->setChecked( cfg.MASK & (1 << i) );
    }
    fWidthSpin[0]->setValue( cfg.WIDTH_L );
    fWidthSpin[1]->setValue( cfg.WIDTH_L );
    fMajLevelSpin->setValue( cfg.MAJORITY );
}

QVariant V895Window::CollectConfig()
{
    UConfig<V895> cfg;

    for( uint8_t i = 0; i < N_CH; ++i )
    {
        cfg.THRESHOLDS.at( i ) = fThrSpin[i]->value();
        if( fEnableCheck[i]->isChecked() ) { cfg.MASK |= (1 << i); }
        else                               { cfg.MASK &= ~(1 << i);}
    }
    cfg.WIDTH_L = fWidthSpin[0]->value();
    cfg.WIDTH_H = fWidthSpin[1]->value();
    cfg.MAJORITY = fMajLevelSpin->value();

    QVariant qv; qv.setValue( cfg );

    return qv;
}