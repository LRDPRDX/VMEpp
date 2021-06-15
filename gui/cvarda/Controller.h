#pragma once

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QApplication>

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;

class Controller : public QMainWindow
{
    Q_OBJECT

    private :
        QAction *fViewStatusBarAction;

    private slots :
        void ToggleStatusBar();

        void Connect();
        void Disconnect();

    public :
        Controller( QWidget *parent = nullptr );
        ~Controller();
};

class Connection : public QDialog
{
    Q_OBJECT

    private :
        QPushButton *fConnectButton, *fCancelButton;
        QComboBox   *fTypeCombo;
        QSpinBox    *fLinkSpin, *fNodeSpin;

    public : 
        Connection( QWidget *parent = nullptr );

    public slots :
        void Connect();
        void Cancel();
};
