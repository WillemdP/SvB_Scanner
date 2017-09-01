#ifndef SVBWINDOW_H
#define SVBWINDOW_H

#include <QMainWindow>
#include <QtSql/QSql>
#include <QSqlTableModel>
#include <QTcpSocket>

#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>

#include <QDebug>

namespace Ui {
class SvbWindow;
}

class SvbWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SvbWindow(QWidget *parent = 0);
    ~SvbWindow();

public slots:
    void connected();
    void disconnected();
    void byteswritten(qint64 bytes);
    void readyRead();

private:
    void getQRcode();
    void connectToHost();

    Ui::SvbWindow *ui;
    QTcpSocket *socket;
};

#endif // SVBWINDOW_H
