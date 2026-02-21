#include "about_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QFont>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("About FilenameEdit");
    setFixedSize(400, 260);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(28, 24, 28, 20);

    // App name
    auto *nameLabel = new QLabel(QApplication::applicationName(), this);
    QFont nameFont = nameLabel->font();
    nameFont.setPointSize(16);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    nameLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(nameLabel);

    // Version
    auto *versionLabel = new QLabel("Version " + QApplication::applicationVersion(), this);
    versionLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(versionLabel);

    // Description
    auto *descLabel = new QLabel(
        "A lightweight tool for batch renaming files by removing\n"
        "substrings from file base names while preserving extensions.",
        this);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    // Author
    auto *authorLabel = new QLabel("Author: <b>Dennis</b>", this);
    authorLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(authorLabel);

    // GitHub link
    auto *linkLabel = new QLabel(
        "<a href=\"https://github.com/jialinliu0817/FilenameEdit\">"
        "github.com/jialinliu0817/FilenameEdit</a>",
        this);
    linkLabel->setAlignment(Qt::AlignCenter);
    linkLabel->setOpenExternalLinks(true);
    layout->addWidget(linkLabel);

    layout->addStretch();

    // OK button
    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    auto *okBtn = new QPushButton("OK", this);
    okBtn->setDefault(true);
    okBtn->setMinimumWidth(80);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnRow->addWidget(okBtn);
    layout->addLayout(btnRow);
}
