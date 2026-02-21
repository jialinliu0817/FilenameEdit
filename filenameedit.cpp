#include "filenameedit.h"
#include "./ui_filenameedit.h"

#include "about_dialog.h"
#include "update_dialog.h"
#include "theme.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSet>
#include <QUuid>
#include <QApplication>
#include <QSettings>
#include <QActionGroup>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCloseEvent>

#ifdef Q_OS_WIN
#  include <windows.h>
#  include <dwmapi.h>
// DWMWA_SYSTEMBACKDROP_TYPE is defined in Windows SDK 10.0.22000+; define it
// manually so the code compiles against older SDKs.
#  ifndef DWMWA_SYSTEMBACKDROP_TYPE
#    define DWMWA_SYSTEMBACKDROP_TYPE 38
#  endif
#  ifndef DWMSBT_NONE
#    define DWMSBT_NONE        0
#    define DWMSBT_MAINWINDOW  2
#  endif
#endif

static constexpr double kMinOpacity     = 0.20;
static constexpr double kDefaultOpacity = 0.95;

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

FilenameEdit::FilenameEdit(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FilenameEdit)
{
    ui->setupUi(this);

    // Configure table columns
    QHeaderView *hh = ui->tableWidget->horizontalHeader();
    hh->setSectionResizeMode(0, QHeaderView::Interactive);
    hh->setSectionResizeMode(1, QHeaderView::Interactive);
    hh->setSectionResizeMode(2, QHeaderView::Stretch);
    hh->setDefaultSectionSize(260);
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Make Light/Dark theme actions mutually exclusive
    auto *themeGroup = new QActionGroup(this);
    themeGroup->addAction(ui->actionLightTheme);
    themeGroup->addAction(ui->actionDarkTheme);

#ifdef Q_OS_WIN
    // Add Mica/Acrylic toggle under View → Effects (Windows only)
    auto *menuEffects = new QMenu("Effects", this);
    auto *actionMica  = new QAction("Mica/Acrylic (Windows 11)", this);
    actionMica->setCheckable(true);
    actionMica->setObjectName("actionMicaEffect");
    menuEffects->addAction(actionMica);
    ui->menuView->addSeparator();
    ui->menuView->addMenu(menuEffects);
    connect(actionMica, &QAction::triggered,
            this, &FilenameEdit::on_actionMicaEffect_triggered);
#endif

    // Wire up buttons and live-preview on substring change
    connect(ui->btnAddFiles,  &QPushButton::clicked, this, &FilenameEdit::onAddFiles);
    connect(ui->btnPreview,   &QPushButton::clicked, this, &FilenameEdit::onPreview);
    connect(ui->btnApply,     &QPushButton::clicked, this, &FilenameEdit::onApply);
    connect(ui->btnClear,     &QPushButton::clicked, this, &FilenameEdit::onClear);

    connect(ui->lineEditSubstring,      &QLineEdit::textChanged,
            this, &FilenameEdit::onSubstringChanged);
    connect(ui->checkBoxCaseInsensitive, &QCheckBox::toggled,
            this, &FilenameEdit::onSubstringChanged);

    loadSettings();
}

FilenameEdit::~FilenameEdit()
{
    delete ui;
}

// ---------------------------------------------------------------------------
// Settings
// ---------------------------------------------------------------------------

void FilenameEdit::loadSettings()
{
    QSettings settings;

    // Theme
    const QString themeStr = settings.value("theme/mode", "Light").toString();
    m_theme = Theme::fromString(themeStr, Theme::Mode::Light);
    applyTheme(m_theme);

    // Opacity (default 0.95)
    const double opacity = settings.value("window/opacity", kDefaultOpacity).toDouble();
    setWindowOpacity(qBound(kMinOpacity, opacity, 1.0));
}

void FilenameEdit::saveSettings()
{
    QSettings settings;
    settings.setValue("theme/mode",    Theme::toString(m_theme));
    settings.setValue("window/opacity", windowOpacity());
#ifdef Q_OS_WIN
    settings.setValue("window/mica", m_micaEnabled);
#endif
}

void FilenameEdit::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

// ---------------------------------------------------------------------------
// Theme helpers
// ---------------------------------------------------------------------------

void FilenameEdit::applyTheme(Theme::Mode mode)
{
    m_theme = mode;
    Theme::apply(qApp, mode);
    ui->actionLightTheme->setChecked(mode == Theme::Mode::Light);
    ui->actionDarkTheme->setChecked(mode  == Theme::Mode::Dark);
}

// ---------------------------------------------------------------------------
// Menu actions – File
// ---------------------------------------------------------------------------

void FilenameEdit::on_actionAddFiles_triggered()
{
    onAddFiles();
}

void FilenameEdit::on_actionExit_triggered()
{
    QApplication::quit();
}

// ---------------------------------------------------------------------------
// Menu actions – View
// ---------------------------------------------------------------------------

void FilenameEdit::on_actionLightTheme_triggered()
{
    applyTheme(Theme::Mode::Light);
    QSettings().setValue("theme/mode", Theme::toString(Theme::Mode::Light));
}

void FilenameEdit::on_actionDarkTheme_triggered()
{
    applyTheme(Theme::Mode::Dark);
    QSettings().setValue("theme/mode", Theme::toString(Theme::Mode::Dark));
}

void FilenameEdit::on_actionWindowOpacity_triggered()
{
    auto *dlg = new QDialog(this);
    dlg->setWindowTitle("Window Opacity");
    dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dlg->setFixedSize(340, 130);

    auto *layout = new QVBoxLayout(dlg);
    layout->setContentsMargins(20, 16, 20, 12);
    layout->setSpacing(10);

    auto *row1 = new QHBoxLayout;
    row1->addWidget(new QLabel("Opacity:", dlg));
    auto *slider  = new QSlider(Qt::Horizontal, dlg);
    slider->setRange(static_cast<int>(kMinOpacity * 100), 100);
    slider->setValue(qRound(windowOpacity() * 100.0));
    slider->setTickInterval(10);
    auto *spinBox = new QDoubleSpinBox(dlg);
    spinBox->setRange(kMinOpacity, 1.00);
    spinBox->setSingleStep(0.05);
    spinBox->setDecimals(2);
    spinBox->setValue(windowOpacity());
    spinBox->setFixedWidth(64);
    row1->addWidget(slider, 1);
    row1->addWidget(spinBox);
    layout->addLayout(row1);

    // Sync slider ↔ spinbox
    connect(slider, &QSlider::valueChanged, dlg, [spinBox, this](int v) {
        const double op = v / 100.0;
        spinBox->blockSignals(true);
        spinBox->setValue(op);
        spinBox->blockSignals(false);
        setWindowOpacity(op);
    });
    connect(spinBox, &QDoubleSpinBox::valueChanged, dlg, [slider, this](double v) {
        slider->blockSignals(true);
        slider->setValue(qRound(v * 100.0));
        slider->blockSignals(false);
        setWindowOpacity(v);
    });

    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    auto *okBtn = new QPushButton("OK", dlg);
    okBtn->setDefault(true);
    okBtn->setMinimumWidth(72);
    connect(okBtn, &QPushButton::clicked, dlg, &QDialog::accept);
    auto *cancelBtn = new QPushButton("Cancel", dlg);
    cancelBtn->setMinimumWidth(72);
    const double origOpacity = windowOpacity();
    connect(cancelBtn, &QPushButton::clicked, dlg, [this, origOpacity, dlg]() {
        setWindowOpacity(origOpacity);
        dlg->reject();
    });
    btnRow->addWidget(okBtn);
    btnRow->addWidget(cancelBtn);
    layout->addLayout(btnRow);

    if (dlg->exec() == QDialog::Accepted)
        QSettings().setValue("window/opacity", windowOpacity());
}

// ---------------------------------------------------------------------------
// Menu actions – Help
// ---------------------------------------------------------------------------

void FilenameEdit::on_actionAbout_triggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void FilenameEdit::on_actionCheckForUpdates_triggered()
{
    UpdateDialog dlg(QApplication::applicationVersion(), this);
    dlg.exec();
}

// ---------------------------------------------------------------------------
// Windows 11 Mica/Acrylic effect (guarded)
// ---------------------------------------------------------------------------

#ifdef Q_OS_WIN
void FilenameEdit::on_actionMicaEffect_triggered()
{
    // Find the actionMicaEffect action to read its current check state
    QAction *action = findChild<QAction *>("actionMicaEffect");
    const bool enable = action ? action->isChecked() : false;
    if (!applyMicaEffect(enable) && enable) {
        if (action) action->setChecked(false);
        QMessageBox::information(this, "Mica Effect",
            "The Mica/Acrylic effect is only supported on Windows 11 (build 22000+).\n"
            "Your system does not support this feature.");
    } else {
        m_micaEnabled = enable;
        QSettings().setValue("window/mica", m_micaEnabled);
    }
}

bool FilenameEdit::applyMicaEffect(bool enable)
{
    // Use RtlGetVersion to reliably detect Windows 11 (build 22000+) at runtime.
    // VerifyVersionInfoW is deprecated and subject to compatibility shims.
    typedef NTSTATUS (WINAPI *RtlGetVersionFn)(PRTL_OSVERSIONINFOW);
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (!ntdll)
        return false;
    auto rtlGetVersion =
        reinterpret_cast<RtlGetVersionFn>(GetProcAddress(ntdll, "RtlGetVersion"));
    if (!rtlGetVersion)
        return false;
    RTL_OSVERSIONINFOW rovi{};
    rovi.dwOSVersionInfoSize = sizeof(rovi);
    if (rtlGetVersion(&rovi) != 0)   // STATUS_SUCCESS == 0
        return false;
    if (rovi.dwBuildNumber < 22000)
        return false;   // not Windows 11+

    HWND hwnd = reinterpret_cast<HWND>(winId());
    DWORD backdropType = enable ? DWMSBT_MAINWINDOW : DWMSBT_NONE;
    HRESULT hr = DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE,
                                       &backdropType, sizeof(backdropType));
    return SUCCEEDED(hr);
}
#endif

// ---------------------------------------------------------------------------
// Button slots
// ---------------------------------------------------------------------------

void FilenameEdit::onAddFiles()
{
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::ExistingFiles);
    if (dlg.exec() != QDialog::Accepted)
        return;

    // Append newly selected files, skipping duplicates
    const QStringList selected = dlg.selectedFiles();
    for (const QString &path : selected) {
        if (!m_filePaths.contains(path))
            m_filePaths.append(path);
    }

    // Rebuild table rows (reset row count and re-populate)
    ui->tableWidget->setRowCount(m_filePaths.size());
    for (int i = 0; i < m_filePaths.size(); ++i) {
        QFileInfo fi(m_filePaths.at(i));
        auto *origItem = new QTableWidgetItem(fi.fileName());
        origItem->setToolTip(m_filePaths.at(i)); // full path as tooltip
        ui->tableWidget->setItem(i, 0, origItem);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem());
    }

    validateAll();
}

void FilenameEdit::onPreview()
{
    validateAll();
}

void FilenameEdit::onSubstringChanged()
{
    if (!m_filePaths.isEmpty())
        validateAll();
}

void FilenameEdit::onClear()
{
    m_filePaths.clear();
    ui->tableWidget->setRowCount(0);
    ui->btnApply->setEnabled(false);
    ui->statusbar->clearMessage();
}

// ---------------------------------------------------------------------------
// Core helpers
// ---------------------------------------------------------------------------

QString FilenameEdit::computeNewBaseName(const QString &baseName) const
{
    const QString sub = ui->lineEditSubstring->text();
    if (sub.isEmpty())
        return baseName;

    const Qt::CaseSensitivity cs = ui->checkBoxCaseInsensitive->isChecked()
                                       ? Qt::CaseInsensitive
                                       : Qt::CaseSensitive;

    // Remove all (possibly overlapping) occurrences iteratively
    QString result = baseName;
    int idx;
    while ((idx = result.indexOf(sub, 0, cs)) != -1)
        result.remove(idx, sub.length());

    return result;
}

bool FilenameEdit::validateAll()
{
    if (m_filePaths.isEmpty()) {
        ui->btnApply->setEnabled(false);
        return false;
    }

    // Pre-compute proposed target paths so we can detect intra-batch duplicates
    const QSet<QString> currentPaths(m_filePaths.begin(), m_filePaths.end());
    QStringList targets(m_filePaths.size());
    QMap<QString, int> targetFrequency; // target path -> how many entries map to it

    for (int i = 0; i < m_filePaths.size(); ++i) {
        const QFileInfo fi(m_filePaths.at(i));
        const QString newBase = computeNewBaseName(fi.completeBaseName());
        // Preserve the extension (empty suffix → no dot appended)
        const QString suffix   = fi.suffix();
        const QString newName  = newBase + (suffix.isEmpty() ? QString() : "." + suffix);
        targets[i] = fi.dir().filePath(newName);
        targetFrequency[targets.at(i)]++;
    }

    // Colour constants used for each row below
    static const QColor errorColor(Qt::red);
    static const QColor okColor(0, 140, 0);    // dark green
    static const QColor grayColor(Qt::gray);

    bool allValid = true;

    for (int i = 0; i < m_filePaths.size(); ++i) {
        const QFileInfo fi(m_filePaths.at(i));
        const QString newBase = computeNewBaseName(fi.completeBaseName());
        const QString suffix  = fi.suffix();
        const QString newName = newBase + (suffix.isEmpty() ? QString() : "." + suffix);
        const QString targetPath = targets.at(i);

        // Determine per-row validity and status text
        QString statusMsg;
        bool rowValid = true;

        if (newBase.isEmpty()) {
            statusMsg = "Error: base name would be empty";
            rowValid  = false;
        } else if (newBase.contains('/') || newBase.contains('\\')) {
            statusMsg = "Error: base name contains path separator";
            rowValid  = false;
        } else if (targetFrequency.value(targetPath) > 1) {
            statusMsg = "Error: duplicate target within batch";
            rowValid  = false;
        } else if (targetPath != m_filePaths.at(i)
                   && QFile::exists(targetPath)
                   && !currentPaths.contains(targetPath)) {
            // Target already occupied by a file outside this rename batch
            statusMsg = "Error: target already exists on disk";
            rowValid  = false;
        } else if (newName == fi.fileName()) {
            statusMsg = "Unchanged";
        } else {
            statusMsg = "OK → " + newName;
        }

        // Update Preview Name cell
        auto *previewItem = ui->tableWidget->item(i, 1);
        if (!previewItem) {
            previewItem = new QTableWidgetItem();
            ui->tableWidget->setItem(i, 1, previewItem);
        }
        previewItem->setText(newName);

        // Update Status cell
        auto *statusItem = ui->tableWidget->item(i, 2);
        if (!statusItem) {
            statusItem = new QTableWidgetItem();
            ui->tableWidget->setItem(i, 2, statusItem);
        }
        statusItem->setText(statusMsg);

        // Colour-code by validity
        if (!rowValid) {
            statusItem->setForeground(errorColor);
            previewItem->setForeground(errorColor);
            allValid = false;
        } else if (statusMsg.startsWith("Unchanged")) {
            statusItem->setForeground(grayColor);
            previewItem->setForeground(grayColor);
        } else {
            statusItem->setForeground(okColor);
            previewItem->setForeground(okColor);
        }
    }

    ui->btnApply->setEnabled(allValid);
    return allValid;
}

// ---------------------------------------------------------------------------
// Apply renames
// ---------------------------------------------------------------------------

void FilenameEdit::onApply()
{
    if (!validateAll())
        return;

    // Collect entries that actually require renaming
    struct RenameEntry {
        int    row;
        QString oldPath;
        QString newPath;
        QString tempPath;   // filled during phase 1
        bool    phase1OK = false;
        bool    phase2OK = false;
    };

    QVector<RenameEntry> entries;
    entries.reserve(m_filePaths.size());

    for (int i = 0; i < m_filePaths.size(); ++i) {
        const QFileInfo fi(m_filePaths.at(i));
        const QString newBase = computeNewBaseName(fi.completeBaseName());
        const QString suffix  = fi.suffix();
        const QString newName = newBase + (suffix.isEmpty() ? QString() : "." + suffix);
        const QString newPath = fi.dir().filePath(newName);
        if (newPath != m_filePaths.at(i))
            entries.append({i, m_filePaths.at(i), newPath, {}, false, false});
    }

    if (entries.isEmpty()) {
        ui->statusbar->showMessage("No files need renaming.", 4000);
        return;
    }

    // Two-phase rename to safely handle swap / cycle cases:
    //   Phase 1 – move every file to a unique temporary name in the same directory.
    //   Phase 2 – move each temporary name to its intended final name.
    // If phase 2 fails for an entry we attempt to restore the original name.

    for (auto &e : entries) {
        const QFileInfo fi(e.oldPath);
        // Use a dot-prefixed name so it stays hidden on Unix during the transition
        e.tempPath = fi.dir().filePath(
            ".fnedit_" + QUuid::createUuid().toString(QUuid::Id128));
        e.phase1OK = QFile::rename(e.oldPath, e.tempPath);
    }

    for (auto &e : entries) {
        if (e.phase1OK)
            e.phase2OK = QFile::rename(e.tempPath, e.newPath);
    }

    // Build per-file result log and update m_filePaths / table for successes
    QStringList log;
    bool anyError = false;

    for (auto &e : entries) {
        const QFileInfo oldFi(e.oldPath);
        const QFileInfo newFi(e.newPath);

        if (e.phase1OK && e.phase2OK) {
            m_filePaths[e.row] = e.newPath;
            ui->tableWidget->item(e.row, 0)->setText(newFi.fileName());
            log.append("✓  " + oldFi.fileName() + "  →  " + newFi.fileName());
        } else if (!e.phase1OK) {
            log.append("✗  " + oldFi.fileName() + "  (could not create temp file)");
            anyError = true;
        } else {
            // Phase 2 failed – try to restore from the temp name
            const bool restored = QFile::rename(e.tempPath, e.oldPath);
            log.append("✗  " + oldFi.fileName() + "  →  " + newFi.fileName()
                       + (restored ? "  (rename failed; original restored)"
                                   : "  (rename failed; file left at temp location!)"));
            anyError = true;
        }
    }

    // Refresh preview/status after updating m_filePaths
    validateAll();

    const QString summary = anyError
                                ? "Some renames failed:"
                                : "All renames completed successfully:";
    QMessageBox::information(this, "Rename Results",
                             summary + "\n\n" + log.join("\n"));

    ui->statusbar->showMessage(
        anyError ? "Some renames failed – see the summary." : "All renames completed.", 5000);
}