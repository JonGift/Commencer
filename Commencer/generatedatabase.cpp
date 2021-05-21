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

// TODO
QStringList GenerateDatabase::stringSplitter(QString inputStr){
    QStringList finalList;
    return finalList;
}

void GenerateDatabase::toggleWidgets(bool tf){
    ui->labelPrimaryKey->setEnabled(tf); // TODO: Put this in a function
    ui->comboBox->setEnabled(tf);
}

void GenerateDatabase::accept(){
    // This function should generate the .db file based on our input parameters and such.
    // Requires: The .csv file must already have been parsed.
    qInfo() << "Wow";

    database = QSqlDatabase::addDatabase("QSQLITE", inputPath + ".db");
    database.setDatabaseName(inputPath + ".db");

    if(!database.open()){
        qInfo() << "Database was not opened.";
        return;
    }

    QSqlQuery q(database);
    q.prepare("DROP TABLE grads;");
    q.exec();
    QString fields = "";
    QString fieldsDataless = "";

    for(int i = 0; i < keys.length(); i++){
        if(i != 0){
            fields += ", ";
            fieldsDataless += ", ";
        }

        fields += "[" + keys[i] + "] string";
        fieldsDataless += "[" + keys[i] + "]";
        if(keys[i] == dbPrimaryKey)
            fields += " PRIMARY KEY";
    }
    QString query = "CREATE TABLE grads(" + fields + ");";
    qInfo() << "Executing command: " << query;
    q.prepare(query);
    q.exec();

    QFile file(inputPath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        qInfo() << "Failed to open file.";
        return;
    }

    file.readLine();

    while (!file.atEnd()) {
        QString inputTemp = file.readLine().simplified();
        QStringList inputSplit = inputTemp.split(',');
        for(int i = 0; i < inputSplit.length(); i++)
            if(inputSplit[i].isEmpty())
                inputSplit[i] = "NULL";
            else
                inputSplit[i] = "\'" + inputSplit[i] + "\'";
        inputTemp = inputSplit.join(", ");
        query = "INSERT INTO grads VALUES (" + inputTemp + ");";
        qInfo() << "Executing command: " << query;
        q.prepare(query);
        q.exec();
    }



    file.close();
    database.close();


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


// Select a csv file, this function also populates information that comes from that file.
void GenerateDatabase::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Database"), "/home", tr("CSV file extension (*.csv)"));

    qInfo() << fileName;
    ui->lineEdit->setText(fileName);
    inputPath = fileName;

    QFile file(inputPath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        qInfo() << "Unable to open .csv file";
        toggleWidgets(false);
        return;
    }
    QString line = file.readLine();
    keys = line.split(","); // TODO: Blows up on majors with a comma in their name.
    for(int i = 0; i < keys.length(); i++){
        keys[i] = keys[i].simplified();
        if(keys[i].isEmpty()){
            ui->comboBox->addItem("Empty column " + QVariant(i).toString());
            keys[i] = "Empty column " + QVariant(i).toString();
        }else
            ui->comboBox->addItem(keys[i]);
    }

    toggleWidgets(true);
    file.close();
    //QStringList wordList;
    //while (!file.atEnd()) {
      //  qInfo() << file.readLine();
        //wordList.append(line.split(',').first());
    //}

    //qDebug() << wordList;
}

// When the combo box changes, the primary key is updated to reflect that.
void GenerateDatabase::on_comboBox_currentIndexChanged(int index)
{
    dbPrimaryKey = ui->comboBox->itemText(index);
    qInfo() << dbPrimaryKey;
}

