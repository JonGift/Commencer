#ifndef GENERATEDATABASE_H
#define GENERATEDATABASE_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace Ui {
class GenerateDatabase;
}

class GenerateDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateDatabase(QWidget *parent = nullptr);
    ~GenerateDatabase();

private slots:
    void accept();
    void on_buttonBox_rejected();

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::GenerateDatabase *ui;
    QSqlDatabase database; // Holds graduates generated from the excel/xml doc, is ordered correctly
    QString inputPath; // Holds path to original grad xml/excel

    QStringList keys; // Holds the original column keys from the .csv file
    QString dbPrimaryKey; // Holds the input from the combo box, is used to generate the .db file with primary key
};

#endif // GENERATEDATABASE_H
