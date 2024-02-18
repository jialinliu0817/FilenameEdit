#include "filenameedit.h"
#include "./ui_filenameedit.h"

#include <QFileDialog>
#include <QDebug>


QStringList strPathList;


FilenameEdit::FilenameEdit(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FilenameEdit)
{
    ui->setupUi(this);
}

FilenameEdit::~FilenameEdit()
{
    delete ui;
}

void FilenameEdit::on_actionAdd_File_triggered()
{
    ui->textEdit->setText("");
    ui->textEdit_2->setText("");

    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    if (fileDialog.exec() == QDialog::Accepted)
    {
        strPathList = fileDialog.selectedFiles();
    }

    int i, j=strPathList.count();
    QStringList filename;

    for (i = 0; i<j; i++)
    {
        QString a1 = strPathList.at(i);
        QStringList a2 = a1.split("/");
        a1 = a2.last();
        filename.append(a1);
        ui->textEdit->append(filename.at(i));
    }

}


void FilenameEdit::on_pushButton_clicked()
{
    ui->textEdit_2->setText("");
    QString strTodelete;
    QStringList strPathListNew = strPathList;

    strTodelete = ui->lineEdit->text();
    qDebug()<<strTodelete;

    int i, j = strPathListNew.count();

    for(i = 0; i < j; i++)
    {
        QString a = strPathListNew.at(i);
        strPathListNew[i] = a.replace(strTodelete, "");
        qDebug()<<strPathListNew[i];
        qDebug()<<strPathList[i];
        bool b = QFile::rename(strPathList.at(i), strPathListNew.at(i));

        if(!b)
        {
            qDebug()<<"wrong!";
        }
    }

    QStringList d1;

    for (i = 0; i<j; i++)
    {
        QString a1 = strPathListNew.at(i);
        QStringList a2 = a1.split("/");
        a1 = a2.last();
        d1.append(a1);
        ui->textEdit_2->append(d1.at(i));
    }

}

