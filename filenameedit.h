#ifndef FILENAMEEDIT_H
#define FILENAMEEDIT_H

#include <QMainWindow>
#include <QStringList>

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
    void onAddFiles();
    void onPreview();
    void onApply();
    void onClear();
    void onSubstringChanged();
    void on_actionAddFiles_triggered();
    void on_actionExit_triggered();

private:
    // Compute the new base name by removing the configured substring.
    QString computeNewBaseName(const QString &baseName) const;

    // Populate Preview Name and Status columns for every row.
    // Returns true only if every entry is valid (Apply may proceed).
    bool validateAll();

    Ui::FilenameEdit *ui;
    QStringList m_filePaths; // absolute paths of the files currently listed
};

#endif // FILENAMEEDIT_H
