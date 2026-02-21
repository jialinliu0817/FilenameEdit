#include "filenameedit.h"

#include <QApplication>
#include <QStyleFactory>
#include <QSettings>
#include "theme.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Application identity (used by QSettings)
    a.setOrganizationName("FilenameEdit");
    a.setOrganizationDomain("github.com/jialinliu0817/FilenameEdit");
    a.setApplicationName("FilenameEdit");
    a.setApplicationVersion("0.1.0");

    // Apply Fusion style for a clean, cross-platform look
    a.setStyle(QStyleFactory::create("Fusion"));

    // Apply persisted theme before showing the window to avoid a visual flash
    QSettings settings;
    const Theme::Mode initialTheme =
        Theme::fromString(settings.value("theme/mode", "Light").toString());
    Theme::apply(&a, initialTheme);

    FilenameEdit w;
    w.show();
    return a.exec();
}
