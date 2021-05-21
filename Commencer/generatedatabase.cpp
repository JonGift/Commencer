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

// Thanks to jefftee at qtforums
QStringList GenerateDatabase::stringSplitter(QString line){
    QString temp = line;
    QString field;
    QStringList field_list;

    // regex explaination
    //
    //    /(?:^|,)(\"(?:[^\"]+|\"\")*\"|[^,]*)/g
    //        (?:^|,) Non-capturing group
    //            1st Alternative: ^
    //                ^ assert position at start of the string
    //            2nd Alternative: ,
    //                , matches the character , literally
    //        1st Capturing group (\"(?:[^\"]+|\"\")*\"|[^,]*)
    //            1st Alternative: \"(?:[^\"]+|\"\")*\"
    //                \" matches the character " literally
    //                (?:[^\"]+|\"\")* Non-capturing group
    //                    Quantifier: * Between zero and unlimited times, as many times as possible, giving back as needed [greedy]
    //                    1st Alternative: [^\"]+
    //                        [^\"]+ match a single character not present in the list below
    //                            Quantifier: + Between one and unlimited times, as many times as possible, giving back as needed [greedy]
    //                            \" matches the character " literally
    //                    2nd Alternative: \"\"
    //                        \" matches the character " literally
    //                        \" matches the character " literally
    //                \" matches the character " literally
    //            2nd Alternative: [^,]*
    //                [^,]* match a single character not present in the list below
    //                    Quantifier: * Between zero and unlimited times, as many times as possible, giving back as needed [greedy]
    //                    , the literal character ,
    //        g modifier: global. All matches (don't return on first match)
    //

    QString regex = "(?:^|,)(\"(?:[^\"]+|\"\")*\"|[^,]*)";

    QRegularExpression re(regex);

    if (temp.right(1) == "\n") temp.chop(1);

    QRegularExpressionMatchIterator it = re.globalMatch(temp);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        if (match.hasMatch())
        {
            field = match.captured(1);
            if (field.left(1) == "\"" && field.right(1) == "\"")
                field = field.mid(1, field.length()-2);
            field_list.push_back(field);
        }
    }

    return field_list;
}

void GenerateDatabase::toggleWidgets(bool tf){
    ui->labelPrimaryKey->setEnabled(tf); // TODO: Put this in a function
    ui->comboBox->setEnabled(tf);
    ui->listWidget->setEnabled(tf);
    ui->labelColumns->setEnabled(tf);
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

    int counter = 0;
    for(int i = 0; i < keys.length(); i++){
        if(ui->listWidget->item(i)->checkState() == Qt::Unchecked){
            counter++;
            continue;
        }
        if(i != counter){
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
        QStringList inputSplit = GenerateDatabase::stringSplitter(inputTemp);
        inputTemp = "";
        counter = 0;
        for(int i = 0; i < inputSplit.length(); i++){
            if(ui->listWidget->item(i)->checkState() == Qt::Unchecked){
                counter++;
                continue;
            }
            if(i != counter){
                inputTemp += ", ";
            }

            if(inputSplit[i].isEmpty())
                inputTemp += "NULL";
            else
                inputTemp += "\'" + inputSplit[i] + "\'";
        }
        //inputTemp = inputSplit.join(", ");
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
    keys = GenerateDatabase::stringSplitter(line);
    for(int i = 0; i < keys.length(); i++){
        keys[i] = keys[i].simplified();
        if(keys[i].isEmpty()){
            ui->comboBox->addItem("Empty column " + QVariant(i).toString());
            keys[i] = "Empty column " + QVariant(i).toString();
        }else
            ui->comboBox->addItem(keys[i]);
    }

    QStringListIterator it(keys);
    while (it.hasNext())
    {
          QListWidgetItem *listItem = new QListWidgetItem(it.next(),ui->listWidget);
          listItem->setCheckState(Qt::Unchecked);
          ui->listWidget->addItem(listItem);
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

