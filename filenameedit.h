#ifndef FILENAMEEDIT_H
#define FILENAMEEDIT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FilenameEdit; }
QT_END_NAMESPACE

class FilenameEdit : public QMainWindow
{
    Q_OBJECT

public:
    FilenameEdit(QWidget *parent = nullptr);
    ~FilenameEdit();

private slots:
    void on_actionAdd_File_triggered();

    void on_pushButton_clicked();


private:
    Ui::FilenameEdit *ui;
};
#endif // FILENAMEEDIT_H
