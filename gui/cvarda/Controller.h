#pragma once

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QApplication>

#include "modules/V2718.h"

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;

class Connection;

class Controller : public QMainWindow
{
    Q_OBJECT

    private :
        vmeplus::V2718 fController;

        QAction *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction *fViewStatusBarAction;

        QTabWidget *fMainTab;

        /****** Constructive methods ******/
    private :
        void CreateActions();
        void CreateMainTab();
        void CreateIOTab();
        void CreatePulserTab();

    protected :
        void closeEvent( QCloseEvent *event ) override;

        void Connect( short link, short conet );
        void Disconnect();

    private slots :
        void ToggleStatusBar();
        void OpenConnectDialog();

    signals :
        void Connected( bool );

    public :
        Controller( QWidget *parent = nullptr );
        ~Controller();

    friend class Connection;
};
