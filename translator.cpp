#include "translator.h"
#include "ui_translator.h"

Translator::Translator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Translator)
{
    ui->setupUi(this);

    ui->pl->setChecked(true);
    ui->en->setChecked(true);
    ui->de->setChecked(true);
    refreshLanguages();
    ui->tabWidget->setTabText(0, "Tłumacz");
    ui->tabWidget->setTabText(1, "Języki");
    ui->tabWidget->setCurrentIndex(0);
}

Translator::~Translator()
{
    delete ui;
}

void Translator::on_translateButton_clicked()
{
    translateAction();
}

void Translator::translateAction()
{
    QString textToTranslate = ui->textToTranslate->toPlainText();

    if(getTranslateFromDb() == false)
        getTranslate();
}

bool Translator::getTranslateFromDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qtrans");

    if (!db.open())
    {
        ui->msg->setText("Błąd: nie można się połączyć z bazą!");
        return false;
    }

    QSqlQuery query;
    query.exec("create table if not exists translations (id int primary key, "
               "text varchar(1000), translated varchar(1000), lang varchar(10))");

    QSqlQuery get;
    get.exec("SELECT * FROM translations WHERE text = '" + ui->textToTranslate->toPlainText() + "' AND lang = '" + getLang()+"'");

    if (get.next())
    {
        QString gText = get.value(1).toString();
        QString gTranslated = get.value(2).toString();
        QString gLang = get.value(3).toString();

        ui->translatedText->setPlainText(gTranslated);
        ui->msg->setText("Pobrano tłumaczenie z bazy");
        ui->correctButton->setEnabled(true);

        return true;
    }

    return false;
}

QString Translator::getLang()
{
    return languages.at(ui->sourceComboBox->currentIndex()).code+"-"+languages.at(ui->destComboBox->currentIndex()).code;
}

void Translator::getTranslate()
{
    mgr = new QNetworkAccessManager(this);
    QNetworkRequest req;
    textToTranslate = ui->textToTranslate->toPlainText();
    if (textToTranslate == "") {
        return;
    }
    QString lang = getLang();
    QString key = "[...]";
    QUrl url("https://translate.yandex.net/api/v1.5/tr.json/translate?key="+key+"&text="+textToTranslate+"&lang="+lang);//("https://translation.googleapis.com/language/translate/v2");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setUrl(url);
    req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.154 Safari/537.36");

    resp = mgr->get(req);
    while(!resp->isFinished())
    {
    qApp->processEvents();
    }

    QString strReply = (QString)resp->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();
    QJsonArray jsonArray = jsonObj["text"].toArray();
    QString translatedResponseText = jsonArray.at(0).toString().toUtf8();

    // {"code":502,"message":"Invalid parameter: text"}

    ui->translatedText->setPlainText(translatedResponseText);
    ui->correctButton->setEnabled(true);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qtrans");

    if (!db.open())
    {
        ui->msg->setText("Błąd: nie można się połączyć z bazą!");
        return;
    }

    QSqlQuery query;
    query.exec("insert into translations (text, translated, lang) values ('"+textToTranslate.toHtmlEscaped()+"', '"+translatedResponseText+"', '"+lang+"')");
    ui->msg->setText("Zapisano tłumaczenie do bazy");
}

void Translator::on_textToTranslate_textChanged()
{
    ui->correctButton->setEnabled(false);
}

void Translator::on_correctButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qtrans");

    if (!db.open())
    {
        ui->msg->setText("Błąd: nie można się połączyć z bazą!");
        return;
    }

    textToTranslate = ui->textToTranslate->toPlainText();
    QString lang = getLang();
    QString translated = ui->translatedText->toPlainText();

    QSqlQuery query;
    query.exec("update translations set translated = '"+translated+"' where text = '"+textToTranslate+"' and lang = '"+lang+"'");

    ui->msg->setText("Zapisano poprawione tłumaczenie do bazy");
    ui->correctButton->setEnabled(false);
}

void Translator::refreshLanguages()
{
    languages.clear();
    if (ui->pl->isChecked())
        languages.append(Lang {"pl", "Polski"});
    if (ui->en->isChecked())
        languages.append(Lang {"en", "Angielski"});
    if (ui->de->isChecked())
        languages.append(Lang {"de", "Niemiecki"});
    if (ui->fr->isChecked())
        languages.append(Lang {"fr", "Francuski"});
    if (ui->cs->isChecked())
        languages.append(Lang {"cs", "Czeski"});
    if (ui->es->isChecked())
        languages.append(Lang {"es", "Hiszpański"});
    if (ui->ru->isChecked())
        languages.append(Lang {"ru", "Rosyjski"});
    if (ui->fi->isChecked())
        languages.append(Lang {"fi", "Fiński"});
    if (ui->ko->isChecked())
        languages.append(Lang {"ko", "Koreański"});
    if (ui->sv->isChecked())
        languages.append(Lang {"sv", "Szwedzki"});
    if (ui->ja->isChecked())
        languages.append(Lang {"ja", "Japoński"});
    if (ui->uk->isChecked())
        languages.append(Lang {"uk", "Ukraiński"});
    if (ui->ar->isChecked())
        languages.append(Lang {"ar", "Arabski"});
    if (ui->el->isChecked())
        languages.append(Lang {"el", "Grecki"});

    ui->sourceComboBox->clear();
    ui->destComboBox->clear();
    for (int i = 0; i < languages.size(); ++i) {
        ui->sourceComboBox->addItem(languages.at(i).name);
        ui->destComboBox->addItem(languages.at(i).name);
    }
    if (languages.size() > 1)
        ui->destComboBox->setCurrentIndex(1);
}

void Translator::on_pl_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_en_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_de_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_fr_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_cs_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_es_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_ru_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_fi_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_ko_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_sv_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_ja_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_uk_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_ar_stateChanged(int arg1)
{
    refreshLanguages();
}

void Translator::on_el_stateChanged(int arg1)
{
    refreshLanguages();
}
