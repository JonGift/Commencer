#ifndef GENERATEDATABASE_H
#define GENERATEDATABASE_H

#include <QDialog>
#include <QDebug>

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

private:
    Ui::GenerateDatabase *ui;
};

#endif // GENERATEDATABASE_H
