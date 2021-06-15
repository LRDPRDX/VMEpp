#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QApplication>

class QAction;

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
