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

class Connection;

class Controller : public QMainWindow
{
    Q_OBJECT

    private :
        vmeplus::V2718 fController;

        QAction *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction *fViewStatusBarAction;

        /****** Constructive methods ******/
    private :
        void CreateActions();

    protected :
        void closeEvent( QCloseEvent *event ) override;

        void Connect( short link, short conet );
        void Disconnect();

    private slots :
        void ToggleStatusBar();
        void OpenConnectDialog();

    signals :
        void Connected();
        void Disconnected();

    public :
        Controller( QWidget *parent = nullptr );
        ~Controller();

    friend class Connection;
};
