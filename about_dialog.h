#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>

// Simple "About" dialog showing app info, version, author, and GitHub link.
class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
};

#endif // ABOUT_DIALOG_H
