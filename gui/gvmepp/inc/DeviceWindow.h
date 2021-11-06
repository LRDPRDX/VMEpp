#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QVariant>
#include <QMessageBox>

#include "Prelude.h"

class QPushButton;
class SFrame;
class QAction;
class QMenu;

using namespace vmepp;


class DeviceWindow : public QMainWindow
{
    Q_OBJECT

        /* Constructive members (called on construction) */
        /* May be hidden in an inherited window */
    private :
        void CreateMenu();
        void CreateBottomFrame();

    protected :
        QMenu           *fFileMenu;
        QAction         *fConnectAction, *fDisconnectAction, *fExitAction;

        QMenu           *fConfigMenu;
        QAction         *fSaveConfigAction, *fLoadConfigAction;

        QMenu           *fViewMenu;
        QAction         *fViewStatusBarAction;

        QPushButton         *fWriteButton, *fReadButton;
        SFrame          *fBottomFrame;

    protected :
        void closeEvent( QCloseEvent *event ) override;
        virtual void DoOnError( const VException& error ) = 0;

    public slots :
        void ToggleStatusBar();

        virtual void WriteConfig() = 0;
        virtual void ReadConfig() = 0;
        virtual void Connect() = 0;
        virtual void Disconnect() = 0;
        virtual QVariant CollectConfig() = 0;
        virtual void SpreadConfig( const QVariant& config ) = 0;

        void OnError( const VException& error );

    public :
        template<typename T>
        void SaveConfig();

        template<typename T>
        void LoadConfig();


    signals :
        void Connected( bool );
        void Programmed( bool );
        void Error( const VException& error );

    public :
        DeviceWindow( QWidget *parent = nullptr );
        ~DeviceWindow() override = default;
};

template <typename T>
void DeviceWindow::SaveConfig()
{
    QString fileName = QFileDialog::getSaveFileName( this, "Save config file", "./", "Text files (*.json)" );

    if( fileName.isEmpty() )
    {
        return;
    }
    else
    {
        UConfig<T> cfg = qvariant_cast< UConfig<T> >( this->CollectConfig() );

        try
        {
            WriteConfigToFile(  cfg, fileName.toStdString() );
        }
        catch( const VException& e )
        {
            QMessageBox::warning( this,
                                  e.what(),
                                  e.GetInfo().c_str(),
                                  QMessageBox::Ok );
        }
    }
}

template <typename T>
void DeviceWindow::LoadConfig()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Load config file", "./", "Text files (*.json)" );

    if( fileName.isEmpty() )
    {
        return;
    }
    else
    {
        try
        {
            UConfig<T> cfg;
            ReadConfigFromFile( cfg, fileName.toStdString() );
            QVariant qv;
            qv.setValue( cfg );
            this->SpreadConfig( qv );
        }
        catch( const VException& e )
        {
            QMessageBox::warning( this,
                                  e.what(),
                                  e.GetInfo().c_str(),
                                  QMessageBox::Ok );
        }
    }
}
