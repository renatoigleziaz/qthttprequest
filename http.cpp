#include "http.h"

http::http(QQuickItem *parent) {

    Q_UNUSED(parent);

    // inicialização
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_response = 0;
    cValue = 0;
    cMaxValue = 0;
}

void http::post(const QString &filename) {

    // post method
    sendfile(filename, "POST");
}

void http::get(const QString &filename) {

    // get method
    sendfile(filename, "GET");
}

QString http::buildJSon(const QJsonObject &data) {

    // obtem um JSON

    /*
     * Formato requerido
     *
        QJsonObject object {

            {"property1", 1},
            {"property2", 2}
        };
    */

    // pega object
    QJsonDocument doc(data);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return "[" + strJson + "]";
}

bool http::saveJSon(const QString &filename, const QString &data) {

    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Não conseguiu ler o arquivo.");
        return false;
    }

    saveFile.write(data.toLatin1());
    saveFile.close();
    return true;
}

int http::getUploadState() {

    // resposta de processo
    return m_response;
}

qint64 http::getUploadValue() {

    return cValue;
}

qint64 http::getUploadMaxValue() {

    return cMaxValue;
}

void http::uploadFinished() {
    // callback de retorno

    QString buffer = m_networkReply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(buffer.toUtf8());
    QJsonArray resp = jsonResponse.array();

    delete m_networkReply;

    if (resp[0].toString() == "ok") {

        emit postFinish();
        m_response = 2;
    }
    else if (resp[0].toString() == "error") {

        emit postError(-1, resp[1].toString());
        m_response = -1;
    }
    else {

        m_response = -1;
    }
}

void http::getFinished() {

    // callback de retorno

    QString buffer = m_networkReply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(buffer.toUtf8());
    QJsonArray resp = jsonResponse.array();

    delete m_networkReply;

    if (resp[0].toString() == "error") {

        emit getError(-1, resp[1].toString());
        m_response = -1;
    }
    else {

        emit getFinish(resp);
        m_response = 2;
    }
}

void http::error(QNetworkReply::NetworkError buffer) {
    Q_UNUSED(buffer);

    emit postError(-1, "Sem rede!");
}

void http::uploadprocess(qint64 c, qint64 e) {

    qDebug() << "Enviando " << c << e;

    cValue    = c;
    cMaxValue = e;
}

void http::sendfile(const QString &filename, const QString &method) {

    // upload de arquivo

    // trata o arquivo
    QFileInfo fileInfo(filename);
    QFile* file = new QFile(filename);

    if (!file->open(QIODevice::ReadOnly)) {

        m_response = -1;
        emit postError(-1, "Arquivo não encontrado...");
        return;
    }

    QByteArray postData;
    QString ext = fileInfo.suffix();
    m_response = 1;

    // cria um header que o server possa reconhecer como POST
    postData.insert(0,"--AaB03x\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" + fileInfo.fileName().toLatin1() + "\"\r\nContent-Type:" + ext + "\r\n\r\n");
    postData.append(file->readAll());
    postData.append("\r\n--AaB03x--\r\n");

    // prepara o destino e o conteudo
    QUrl url = QUrl("http://localhost/JSON/server/server.php?method=" + method);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data; boundary=AaB03x");
    QVariant l=postData.length();
    req.setHeader(QNetworkRequest::ContentLengthHeader,l.toString());

    file->close();
    delete(file);

    // POST method
    m_networkReply = m_networkAccessManager->post(req, postData);

    // trata os sinais de retorno
    connect(m_networkReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadprocess(qint64,qint64)));
    connect(m_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    if (method == "POST") {
        connect(m_networkReply, SIGNAL(finished()), this, SLOT(uploadFinished()));
    }
    else if (method == "GET") {
        connect(m_networkReply, SIGNAL(finished()), this, SLOT(getFinished()));
    }
}
