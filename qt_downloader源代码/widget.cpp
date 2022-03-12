#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include  <QDesktopServices>
#include <QPainter>
#include <QPixmap>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Init();
    ui->progressBar->setValue(0);
    ui->lineEdit->setText("http://dlie.sogoucdn.com/se/sogou_explorer_11.0.1_0722.exe");
    ui->lineEdit_path->setText("D:/test/");

}

void Widget::Init()
{

    manager = new QNetworkAccessManager(this);
    myfile = new QFile(this);
    m_isstop = 1;
    rec = "\0";
    path = "D:/test/";
    filename = "\0";
    i=1;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap("://images/background.png"),QRect());		//传入资源图片路径
}

void Widget::on_pushButton_clicked()//确认键
{
    if(m_isstop==1){
        m_isstop=0;
    url = ui->lineEdit->text();
    request.setUrl(QUrl(url));

    reply = manager->get(request);              //发送请求


    connect(reply,&QNetworkReply::readyRead,this,&Widget::doProcessReadyRead);                //可读
    connect(reply,&QNetworkReply::finished,this,&Widget::doProcessFinished);                  //结束
    connect(reply,&QNetworkReply::downloadProgress,this,&Widget::doProcessDownloadProgress);  //大小


    QStringList list = url.split("/");
    filename = list.at(list.length()-1);
    file = path + filename;//文件路径
    myfile->setFileName(file);
   bool ret = myfile->open(QIODevice::WriteOnly|QIODevice::Truncate);    //创建文件
    if(!ret)
    {
        QMessageBox::warning(this,"warning","下载失败");
        return;
    }

    ui->progressBar->setValue(0);             //设置进度条初始化为0
    ui->progressBar->setMinimum(0);}
    else {
        QMessageBox::warning(this,"提示","已开始下载");
    }

}

void Widget::doProcessReadyRead()             //读取并写入
{
    while(!reply->atEnd())
    {
        QByteArray ba = reply->readAll();
        myfile->write(ba);
    }
//    myfile->close();
}
void Widget::doProcessReadyRead_con()             //读取并写入
{
    while(!reply->atEnd())
    {
        QByteArray ba = reply->readAll();
        myfile->write(ba);
    }
//    myfile->close();
}


void Widget::doProcessFinished()
{
    m_isstop=1;
    QStringList list = ui->lineEdit->text().split("/");
    QString filename = list.at(list.length()-1);
    QString data =  "下载成功--" + filename;
    rec += filename + "---from \"" + url + "\"\n";
    ui->textEdit_rec->setText(rec);
    ui->textEdit->setText(data);
    QMessageBox::warning(this,"提示","下载成功");
    myfile->close();
}

void Widget::doProcessDownloadProgress(qint64 recv_total, qint64 all_total)      //显示
{
    ui->progressBar->setMaximum(all_total);
    ui->progressBar->setValue(recv_total);
    bytesTotal = all_total;
    ui->textEdit->setText("正在下载...");


}
void Widget::doProcessDownloadProgress_con(qint64 recv_total, qint64 all_total)
{
    ui->progressBar->setValue(myfile->size());
    ui->progressBar->setMaximum(bytesTotal);
    ui->textEdit->setText("正在下载...");
}

void Widget::doProcessError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_suspendButton_clicked()
{

    if( reply == 0 ) {
            return;
        }
    disconnect(reply,&QNetworkReply::readyRead,this,&Widget::doProcessReadyRead);                //可读
    disconnect(reply,&QNetworkReply::finished,this,&Widget::doProcessFinished);                  //结束
    disconnect(reply,&QNetworkReply::downloadProgress,this,&Widget::doProcessDownloadProgress);  //大小

    reply->abort();
    reply->deleteLater();
    reply = nullptr;
    myfile->close();
    mDownloadSizeAtPause = myfile->size();
    ui->textEdit->setText("已暂停下载");
    QMessageBox::warning(this,"提示","已暂停下载");
}





void Widget::on_pathbutton_clicked()
{
    path = ui->lineEdit_path->text();
    QMessageBox::warning(this,"提示","更改成功");
}


void Widget::on_continueButton_clicked()
{   qDebug()<<"1";

    qDebug()<<"1"<<mDownloadSizeAtPause;
        QString rangeHeaderValue =  QString("bytes=%1-").arg(mDownloadSizeAtPause);
        qDebug()<<"1"<<bytesTotal;
        request.setRawHeader("Range",rangeHeaderValue.toUtf8());
        reply = manager->get(request);
        myfile->open(QIODevice::WriteOnly|QIODevice::Append);

      {
            connect(reply,&QNetworkReply::readyRead,this,&Widget::doProcessReadyRead_con);                //可读
            connect(reply,&QNetworkReply::finished,this,&Widget::doProcessFinished);                  //结束
            connect(reply,&QNetworkReply::downloadProgress,this,&Widget::doProcessDownloadProgress_con);  //大小
      }


}


void Widget::on_pushButton_delete_clicked()
{
    QFileInfo fileInfo(file);
    if(fileInfo.exists())
        QFile::remove(file);
    QMessageBox::warning(this,"提示","删除成功");
    ui->progressBar->setValue(0);
    qDebug() << "removeFile Func";
}


void Widget::on_pushButton_openpath_clicked()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

