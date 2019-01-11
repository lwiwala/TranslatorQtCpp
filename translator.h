#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QMainWindow>
#include <QtNetwork/QNetworkReply>
#include <QList>
#include <QtSql/QSqlError>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QtNetwork>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Translator;
}

struct Lang {
    QString code;
    QString name;
};

class Translator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Translator(QWidget *parent = 0);
    ~Translator();

private slots:
    void on_translateButton_clicked();
    void on_textToTranslate_textChanged();

    void on_correctButton_clicked();

    void on_pl_stateChanged(int arg1);

    void on_en_stateChanged(int arg1);

    void on_de_stateChanged(int arg1);

    void on_fr_stateChanged(int arg1);

    void on_cs_stateChanged(int arg1);

    void on_es_stateChanged(int arg1);

    void on_ru_stateChanged(int arg1);

    void on_fi_stateChanged(int arg1);

    void on_ko_stateChanged(int arg1);

    void on_sv_stateChanged(int arg1);

    void on_ja_stateChanged(int arg1);

    void on_uk_stateChanged(int arg1);

    void on_ar_stateChanged(int arg1);

    void on_el_stateChanged(int arg1);

private:
    Ui::Translator *ui;
    QNetworkReply *resp;
    QNetworkAccessManager * mgr;
    QString textToTranslate;
    void translateAction();
    void getTranslate();
    bool getTranslateFromDb();
    QString getLang();
    QList<Lang> languages;
    void refreshLanguages();
};

#endif // TRANSLATOR_H
