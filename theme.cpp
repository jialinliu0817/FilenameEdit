#include "theme.h"

#include <QApplication>
#include <QPalette>

// ---------------------------------------------------------------------------
// Light stylesheet — Windows-11-inspired, Fusion-based, cross-platform
// ---------------------------------------------------------------------------
static const char *kLightQSS = R"(
QMainWindow, QDialog {
    background: #f3f3f3;
}
QWidget {
    color: #1a1a1a;
}
QTableWidget {
    border: 1px solid #d1d1d1;
    gridline-color: #e5e5e5;
    background: #ffffff;
    border-radius: 6px;
    outline: none;
}
QTableWidget::item:selected {
    background: #cce4f7;
    color: #000000;
}
QHeaderView::section {
    background: #f0f0f0;
    border: none;
    border-right: 1px solid #d1d1d1;
    border-bottom: 1px solid #d1d1d1;
    padding: 5px 8px;
    font-weight: 600;
}
QPushButton {
    min-width: 90px;
    padding: 5px 14px;
    border: 1px solid #d1d1d1;
    border-radius: 4px;
    background: #f9f9f9;
    color: #1a1a1a;
}
QPushButton:hover {
    background: #e9e9e9;
    border-color: #b8b8b8;
}
QPushButton:pressed {
    background: #e0e0e0;
}
QPushButton#btnApply {
    background: #0067c0;
    color: white;
    border: 1px solid #0067c0;
    border-radius: 4px;
    font-weight: 600;
}
QPushButton#btnApply:hover    { background: #005da0; border-color: #005da0; }
QPushButton#btnApply:pressed  { background: #004e88; }
QPushButton#btnApply:disabled { background: #a0c4e8; border-color: #a0c4e8; color: #e8e8e8; }
QLineEdit {
    border: 1px solid #c8c8c8;
    border-radius: 4px;
    padding: 4px 8px;
    background: #ffffff;
    selection-background-color: #0067c0;
}
QLineEdit:focus {
    border-color: #0067c0;
}
QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border-radius: 3px;
    border: 1px solid #b0b0b0;
}
QCheckBox::indicator:checked {
    background: #0067c0;
    border-color: #0067c0;
}
QStatusBar  { color: #555555; }
QMenuBar    { background: #f3f3f3; }
QMenuBar::item:selected { background: #e0e0e0; border-radius: 4px; }
QMenu {
    background: #ffffff;
    border: 1px solid #d1d1d1;
    border-radius: 6px;
}
QMenu::item { padding: 6px 20px 6px 16px; }
QMenu::item:selected { background: #e5f0fb; color: #000000; border-radius: 4px; }
QMenu::separator { height: 1px; background: #e0e0e0; margin: 4px 8px; }
QSlider::groove:horizontal {
    border: 1px solid #d1d1d1;
    height: 4px;
    background: #e0e0e0;
    border-radius: 2px;
}
QSlider::handle:horizontal {
    background: #0067c0;
    border: none;
    width: 14px;
    height: 14px;
    border-radius: 7px;
    margin: -5px 0;
}
QLabel { color: #1a1a1a; }
)";

// ---------------------------------------------------------------------------
// Dark stylesheet
// ---------------------------------------------------------------------------
static const char *kDarkQSS = R"(
QMainWindow, QDialog {
    background: #202020;
}
QWidget {
    color: #f0f0f0;
}
QTableWidget {
    border: 1px solid #3a3a3a;
    gridline-color: #2e2e2e;
    background: #2a2a2a;
    border-radius: 6px;
    outline: none;
    color: #f0f0f0;
}
QTableWidget::item:selected {
    background: #1c4a72;
    color: #ffffff;
}
QHeaderView::section {
    background: #2d2d2d;
    border: none;
    border-right: 1px solid #3a3a3a;
    border-bottom: 1px solid #3a3a3a;
    padding: 5px 8px;
    font-weight: 600;
    color: #e0e0e0;
}
QPushButton {
    min-width: 90px;
    padding: 5px 14px;
    border: 1px solid #4a4a4a;
    border-radius: 4px;
    background: #3a3a3a;
    color: #f0f0f0;
}
QPushButton:hover {
    background: #4a4a4a;
    border-color: #606060;
}
QPushButton:pressed {
    background: #555555;
}
QPushButton#btnApply {
    background: #0067c0;
    color: white;
    border: 1px solid #0067c0;
    border-radius: 4px;
    font-weight: 600;
}
QPushButton#btnApply:hover    { background: #005da0; border-color: #005da0; }
QPushButton#btnApply:pressed  { background: #004e88; }
QPushButton#btnApply:disabled { background: #1a3a5a; border-color: #1a3a5a; color: #6a8aaa; }
QLineEdit {
    border: 1px solid #4a4a4a;
    border-radius: 4px;
    padding: 4px 8px;
    background: #2d2d2d;
    color: #f0f0f0;
    selection-background-color: #0067c0;
}
QLineEdit:focus {
    border-color: #0067c0;
}
QCheckBox { color: #f0f0f0; }
QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border-radius: 3px;
    border: 1px solid #606060;
    background: #2d2d2d;
}
QCheckBox::indicator:checked {
    background: #0067c0;
    border-color: #0067c0;
}
QLabel      { color: #f0f0f0; }
QStatusBar  { color: #b0b0b0; background: #202020; }
QMenuBar    { background: #2a2a2a; color: #f0f0f0; }
QMenuBar::item:selected { background: #3a3a3a; border-radius: 4px; }
QMenu {
    background: #2a2a2a;
    border: 1px solid #3a3a3a;
    border-radius: 6px;
    color: #f0f0f0;
}
QMenu::item { padding: 6px 20px 6px 16px; }
QMenu::item:selected { background: #1c4a72; color: #ffffff; border-radius: 4px; }
QMenu::separator { height: 1px; background: #3a3a3a; margin: 4px 8px; }
QDialog { background: #202020; color: #f0f0f0; }
QSlider::groove:horizontal {
    border: 1px solid #4a4a4a;
    height: 4px;
    background: #3a3a3a;
    border-radius: 2px;
}
QSlider::handle:horizontal {
    background: #0067c0;
    border: none;
    width: 14px;
    height: 14px;
    border-radius: 7px;
    margin: -5px 0;
}
)";

// ---------------------------------------------------------------------------

QString Theme::styleSheet(Mode mode)
{
    return mode == Mode::Dark ? QString(kDarkQSS) : QString(kLightQSS);
}

void Theme::apply(QApplication *app, Mode mode)
{
    app->setStyleSheet(styleSheet(mode));

    if (mode == Mode::Dark) {
        QPalette p;
        p.setColor(QPalette::Window,          QColor(32,  32,  32));
        p.setColor(QPalette::WindowText,      QColor(240, 240, 240));
        p.setColor(QPalette::Base,            QColor(42,  42,  42));
        p.setColor(QPalette::AlternateBase,   QColor(50,  50,  50));
        p.setColor(QPalette::ToolTipBase,     QColor(32,  32,  32));
        p.setColor(QPalette::ToolTipText,     QColor(240, 240, 240));
        p.setColor(QPalette::Text,            QColor(240, 240, 240));
        p.setColor(QPalette::Button,          QColor(58,  58,  58));
        p.setColor(QPalette::ButtonText,      QColor(240, 240, 240));
        p.setColor(QPalette::BrightText,      Qt::red);
        p.setColor(QPalette::Link,            QColor(66,  135, 245));
        p.setColor(QPalette::Highlight,       QColor(28,  74,  114));
        p.setColor(QPalette::HighlightedText, Qt::white);
        app->setPalette(p);
    } else {
        app->setPalette(QPalette()); // reset to platform default
    }
}

Theme::Mode Theme::fromString(const QString &s, Mode defaultMode)
{
    if (s.compare(QStringLiteral("Dark"),  Qt::CaseInsensitive) == 0) return Mode::Dark;
    if (s.compare(QStringLiteral("Light"), Qt::CaseInsensitive) == 0) return Mode::Light;
    return defaultMode;
}

QString Theme::toString(Mode mode)
{
    return mode == Mode::Dark ? QStringLiteral("Dark") : QStringLiteral("Light");
}
