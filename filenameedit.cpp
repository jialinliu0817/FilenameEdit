#include "filenameedit.h"
#include "./ui_filenameedit.h"

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

    // Wire up buttons and live-preview on substring change
    connect(ui->btnAddFiles,  &QPushButton::clicked, this, &FilenameEdit::onAddFiles);
    connect(ui->btnPreview,   &QPushButton::clicked, this, &FilenameEdit::onPreview);
    connect(ui->btnApply,     &QPushButton::clicked, this, &FilenameEdit::onApply);
    connect(ui->btnClear,     &QPushButton::clicked, this, &FilenameEdit::onClear);

    connect(ui->lineEditSubstring,      &QLineEdit::textChanged,
            this, &FilenameEdit::onSubstringChanged);
    connect(ui->checkBoxCaseInsensitive, &QCheckBox::toggled,
            this, &FilenameEdit::onSubstringChanged);
}

FilenameEdit::~FilenameEdit()
{
    delete ui;
}

// ---------------------------------------------------------------------------
// Menu actions
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

