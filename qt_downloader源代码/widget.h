#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QPaintEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *myfile;
    qint8 i;
    bool m_isstop;
    QString url ;
    QString rec ;
    QString path;//文件路径
    QString filename;
    QString file;
    QNetworkRequest request;
    qint64 mDownloadSizeAtPause;
    qint64 bytesTotal;
    void Init();
    void doProcessReadyRead();
    void paintEvent(QPaintEvent *event);

    void doProcessFinished();
    void doProcessDownloadProgress(qint64,qint64);
    void doProcessDownloadProgress_con(qint64 recv_total, qint64 all_total);
    void doProcessReadyRead_con();
    void doProcessError(QNetworkReply::NetworkError code);
private slots:
    void on_pushButton_clicked();

    void on_suspendButton_clicked();



    void on_pathbutton_clicked();

    void on_continueButton_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_openpath_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

