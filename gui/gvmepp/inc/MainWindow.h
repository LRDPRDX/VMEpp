#pragma once

#include <QMainWindow>

class QLabel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    protected :
        QLabel *fLogoLabel;

    /* Constructive methods */
    private :
        void CreateMenu();
        void CreateCentralWidget();

    protected :
        void closeEvent( QCloseEvent *event ) override;

    public :
        MainWindow( QWidget *parent = nullptr );
        ~MainWindow() override = default;
};
