#include "generatedatabase.h"
#include "ui_generatedatabase.h"

GenerateDatabase::GenerateDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateDatabase)
{
    ui->setupUi(this);
}

GenerateDatabase::~GenerateDatabase()
{
    delete ui;
}

void GenerateDatabase::accept(){
    // This function should generate the .db file based on our input parameters and such.
    // Requires: The .csv file must already have been parsed.
    qInfo() << "Wow";


    //QSqlQuery q();
    // TODO: Figure out a way to convert excel documents to .csv format in the program itself.
    /*QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection"); db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + inputPath);
    if(db.open()){
        qInfo() << "We opened the .xls file!";
        db.close();
    }else{
        qInfo() << "Ah shoot.";
    }*/
    /*database = QSqlDatabase::addDatabase("QSQLITE", inputPath + "wow");
    database.setDatabaseName("Graduates");
    if(database.open()){
        qInfo() << "Database was opened";
        QSqlQuery q(database);
        q.prepare() // TODO: Generate table based on input params
    }else{
        qInfo() << "Database opening failure";
    }



    database.close();*/
}


void GenerateDatabase::on_buttonBox_rejected()
{
    this->hide();
}


void GenerateDatabase::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Database"), "/home", tr("CSV file extension (*.csv)"));

    qInfo() << fileName;
    ui->lineEdit->setText(fileName);
    inputPath = fileName;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        qInfo() << "Unable to open .csv file";
        return;
    }

    while (!file.atEnd()) {
        qInfo() << file.readLine();
    }
}

