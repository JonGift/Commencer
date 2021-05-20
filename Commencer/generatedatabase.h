#ifndef GENERATEDATABASE_H
#define GENERATEDATABASE_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>

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

private:
    Ui::GenerateDatabase *ui;
};

#endif // GENERATEDATABASE_H
