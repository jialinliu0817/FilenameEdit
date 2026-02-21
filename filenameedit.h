#ifndef FILENAMEEDIT_H
#define FILENAMEEDIT_H

#include <QMainWindow>
#include <QStringList>
#include "theme.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FilenameEdit; }
QT_END_NAMESPACE

class FilenameEdit : public QMainWindow
{
    Q_OBJECT

public:
    FilenameEdit(QWidget *parent = nullptr);
    ~FilenameEdit();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAddFiles();
    void onPreview();
    void onApply();
    void onClear();
    void onSubstringChanged();
    void on_actionAddFiles_triggered();
    void on_actionExit_triggered();

    // View menu
    void on_actionLightTheme_triggered();
    void on_actionDarkTheme_triggered();
    void on_actionWindowOpacity_triggered();

    // Help menu
    void on_actionAbout_triggered();
    void on_actionCheckForUpdates_triggered();

private:
    // Compute the new base name by removing the configured substring.
    QString computeNewBaseName(const QString &baseName) const;

    // Populate Preview Name and Status columns for every row.
    // Returns true only if every entry is valid (Apply may proceed).
    bool validateAll();

    void applyTheme(Theme::Mode mode);
    void loadSettings();
    void saveSettings();

    Ui::FilenameEdit *ui;
    QStringList       m_filePaths;   // absolute paths of the files currently listed
    Theme::Mode       m_theme = Theme::Mode::Light;

#ifdef Q_OS_WIN
    // Windows 11 Mica/Acrylic backdrop (nice-to-have)
    void on_actionMicaEffect_triggered();
    bool applyMicaEffect(bool enable);
    bool m_micaEnabled = false;
#endif
};

#endif // FILENAMEEDIT_H