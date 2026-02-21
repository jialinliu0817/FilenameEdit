#ifndef THEME_H
#define THEME_H

#include <QString>

class QApplication;

// Manages Light/Dark theme switching and persistence.
class Theme
{
public:
    enum class Mode { Light, Dark };

    static QString styleSheet(Mode mode);
    static void apply(QApplication *app, Mode mode);
    static Mode fromString(const QString &s, Mode defaultMode = Mode::Light);
    static QString toString(Mode mode);
};

#endif // THEME_H
