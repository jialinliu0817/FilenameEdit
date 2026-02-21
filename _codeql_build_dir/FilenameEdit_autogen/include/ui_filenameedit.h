/********************************************************************************
** Form generated from reading UI file 'filenameedit.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILENAMEEDIT_H
#define UI_FILENAMEEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilenameEdit
{
public:
    QAction *actionAddFiles;
    QAction *actionExit;
    QAction *actionLightTheme;
    QAction *actionDarkTheme;
    QAction *actionWindowOpacity;
    QAction *actionAbout;
    QAction *actionCheckForUpdates;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topLayout;
    QLabel *labelRemove;
    QLineEdit *lineEditSubstring;
    QCheckBox *checkBoxCaseInsensitive;
    QPushButton *btnPreview;
    QTableWidget *tableWidget;
    QHBoxLayout *bottomLayout;
    QPushButton *btnAddFiles;
    QPushButton *btnClear;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnApply;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuTheme;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *FilenameEdit)
    {
        if (FilenameEdit->objectName().isEmpty())
            FilenameEdit->setObjectName("FilenameEdit");
        FilenameEdit->resize(820, 560);
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("accessories-text-editor");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("filenameEdit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        }
        FilenameEdit->setWindowIcon(icon);
        actionAddFiles = new QAction(FilenameEdit);
        actionAddFiles->setObjectName("actionAddFiles");
        actionExit = new QAction(FilenameEdit);
        actionExit->setObjectName("actionExit");
        actionLightTheme = new QAction(FilenameEdit);
        actionLightTheme->setObjectName("actionLightTheme");
        actionLightTheme->setCheckable(true);
        actionLightTheme->setChecked(true);
        actionDarkTheme = new QAction(FilenameEdit);
        actionDarkTheme->setObjectName("actionDarkTheme");
        actionDarkTheme->setCheckable(true);
        actionWindowOpacity = new QAction(FilenameEdit);
        actionWindowOpacity->setObjectName("actionWindowOpacity");
        actionAbout = new QAction(FilenameEdit);
        actionAbout->setObjectName("actionAbout");
        actionCheckForUpdates = new QAction(FilenameEdit);
        actionCheckForUpdates->setObjectName("actionCheckForUpdates");
        centralwidget = new QWidget(FilenameEdit);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        topLayout = new QHBoxLayout();
        topLayout->setSpacing(6);
        topLayout->setObjectName("topLayout");
        labelRemove = new QLabel(centralwidget);
        labelRemove->setObjectName("labelRemove");

        topLayout->addWidget(labelRemove);

        lineEditSubstring = new QLineEdit(centralwidget);
        lineEditSubstring->setObjectName("lineEditSubstring");

        topLayout->addWidget(lineEditSubstring);

        checkBoxCaseInsensitive = new QCheckBox(centralwidget);
        checkBoxCaseInsensitive->setObjectName("checkBoxCaseInsensitive");

        topLayout->addWidget(checkBoxCaseInsensitive);

        btnPreview = new QPushButton(centralwidget);
        btnPreview->setObjectName("btnPreview");

        topLayout->addWidget(btnPreview);


        verticalLayout->addLayout(topLayout);

        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setColumnCount(3);
        tableWidget->horizontalHeader()->setMinimumSectionSize(100);
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tableWidget);

        bottomLayout = new QHBoxLayout();
        bottomLayout->setSpacing(6);
        bottomLayout->setObjectName("bottomLayout");
        btnAddFiles = new QPushButton(centralwidget);
        btnAddFiles->setObjectName("btnAddFiles");

        bottomLayout->addWidget(btnAddFiles);

        btnClear = new QPushButton(centralwidget);
        btnClear->setObjectName("btnClear");

        bottomLayout->addWidget(btnClear);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bottomLayout->addItem(horizontalSpacer);

        btnApply = new QPushButton(centralwidget);
        btnApply->setObjectName("btnApply");
        btnApply->setEnabled(false);

        bottomLayout->addWidget(btnApply);


        verticalLayout->addLayout(bottomLayout);

        FilenameEdit->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FilenameEdit);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 820, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuTheme = new QMenu(menuView);
        menuTheme->setObjectName("menuTheme");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        FilenameEdit->setMenuBar(menubar);
        statusbar = new QStatusBar(FilenameEdit);
        statusbar->setObjectName("statusbar");
        FilenameEdit->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionAddFiles);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(menuTheme->menuAction());
        menuView->addSeparator();
        menuView->addAction(actionWindowOpacity);
        menuTheme->addAction(actionLightTheme);
        menuTheme->addAction(actionDarkTheme);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionCheckForUpdates);

        retranslateUi(FilenameEdit);

        QMetaObject::connectSlotsByName(FilenameEdit);
    } // setupUi

    void retranslateUi(QMainWindow *FilenameEdit)
    {
        FilenameEdit->setWindowTitle(QCoreApplication::translate("FilenameEdit", "FilenameEdit", nullptr));
        actionAddFiles->setText(QCoreApplication::translate("FilenameEdit", "Add Files\342\200\246", nullptr));
#if QT_CONFIG(shortcut)
        actionAddFiles->setShortcut(QCoreApplication::translate("FilenameEdit", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("FilenameEdit", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("FilenameEdit", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLightTheme->setText(QCoreApplication::translate("FilenameEdit", "Light", nullptr));
        actionDarkTheme->setText(QCoreApplication::translate("FilenameEdit", "Dark", nullptr));
        actionWindowOpacity->setText(QCoreApplication::translate("FilenameEdit", "Window Opacity\342\200\246", nullptr));
        actionAbout->setText(QCoreApplication::translate("FilenameEdit", "About FilenameEdit\342\200\246", nullptr));
        actionCheckForUpdates->setText(QCoreApplication::translate("FilenameEdit", "Check for Updates\342\200\246", nullptr));
        labelRemove->setText(QCoreApplication::translate("FilenameEdit", "Remove substring:", nullptr));
        lineEditSubstring->setPlaceholderText(QCoreApplication::translate("FilenameEdit", "Enter text to remove from file names", nullptr));
        checkBoxCaseInsensitive->setText(QCoreApplication::translate("FilenameEdit", "Case-insensitive", nullptr));
        btnPreview->setText(QCoreApplication::translate("FilenameEdit", "Preview", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("FilenameEdit", "Original Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("FilenameEdit", "Preview Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("FilenameEdit", "Status", nullptr));
        btnAddFiles->setText(QCoreApplication::translate("FilenameEdit", "Add Files\342\200\246", nullptr));
        btnClear->setText(QCoreApplication::translate("FilenameEdit", "Clear", nullptr));
        btnApply->setText(QCoreApplication::translate("FilenameEdit", "Apply (Rename)", nullptr));
        menuFile->setTitle(QCoreApplication::translate("FilenameEdit", "File", nullptr));
        menuView->setTitle(QCoreApplication::translate("FilenameEdit", "View", nullptr));
        menuTheme->setTitle(QCoreApplication::translate("FilenameEdit", "Theme", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("FilenameEdit", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FilenameEdit: public Ui_FilenameEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILENAMEEDIT_H
