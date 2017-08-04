#ifndef HTTP_H
#define HTTP_H

/*
*
*  C++ Header
*
*  por Renato Igleziaz
*  18/10/2016
*
*/

#include <QQuickItem>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>

class http: public QQuickItem
{

Q_OBJECT

public:
    explicit http(QQuickItem* parent = 0);

public Q_SLOTS:
    virtual void post(const QString &filename);
    virtual void get(const QString &filename);
    virtual QString buildJSon(const QJsonObject& data);
    virtual bool saveJSon(const QString &filename, const QString &data);

Q_SIGNALS:
    void postFinish();
    void postError(int errorCode, const QString& errorMessage);  
    void getFinish(QJsonArray data);
    void getError(int errorCode, const QString& errorMessage);

private slots:
    void uploadFinished();
    void getFinished();
    void error(QNetworkReply::NetworkError buffer);
    void uploadprocess(qint64 c, qint64 e);
    void sendfile(const QString& filename, const QString& method);
    int getUploadState();
    qint64 getUploadValue();
    qint64 getUploadMaxValue();

private:
    QNetworkAccessManager* m_networkAccessManager;
    QNetworkReply* m_networkReply;
    int m_response;
    qint64 cValue;
    qint64 cMaxValue;

signals:
    void fileUploaded(int errorCode, const QString& errorMessage);

};


#endif // HTTP_H
