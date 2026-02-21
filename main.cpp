#include "filenameedit.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Apply Fusion style for a clean, cross-platform look
    a.setStyle(QStyleFactory::create("Fusion"));

    // Minimal light stylesheet for consistent aesthetics
    a.setStyleSheet(
        "QMainWindow { background: #f5f5f5; }"
        "QTableWidget {"
        "  border: 1px solid #c8c8c8;"
        "  gridline-color: #e0e0e0;"
        "  background: #ffffff;"
        "}"
        "QHeaderView::section {"
        "  background: #efefef;"
        "  border: none;"
        "  border-right: 1px solid #c8c8c8;"
        "  border-bottom: 1px solid #c8c8c8;"
        "  padding: 4px 6px;"
        "  font-weight: bold;"
        "}"
        "QPushButton {"
        "  min-width: 90px;"
        "  padding: 4px 12px;"
        "}"
        "QPushButton#btnApply {"
        "  background: #0078d4;"
        "  color: white;"
        "  border-radius: 3px;"
        "}"
        "QPushButton#btnApply:hover { background: #106ebe; }"
        "QPushButton#btnApply:disabled { background: #a0c4e8; color: #e0e0e0; }"
        "QLineEdit {"
        "  border: 1px solid #c8c8c8;"
        "  border-radius: 3px;"
        "  padding: 3px 6px;"
        "}"
        "QStatusBar { color: #555555; }"
    );

    FilenameEdit w;
    w.show();
    return a.exec();
}
