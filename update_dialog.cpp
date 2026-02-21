#include "update_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVersionNumber>
#include <QProgressDialog>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QMessageBox>

static const QString kReleasesApiUrl =
    QStringLiteral("https://api.github.com/repos/jialinliu0817/FilenameEdit/releases/latest");
static const QString kReleasesPageUrl =
    QStringLiteral("https://github.com/jialinliu0817/FilenameEdit/releases/latest");
static const QString kNewReleasesPageUrl =
    QStringLiteral("https://github.com/jialinliu0817/FilenameEdit/releases");

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

UpdateDialog::UpdateDialog(const QString &currentVersion, QWidget *parent)
    : QDialog(parent)
    , m_currentVersion(currentVersion)
{
    setWindowTitle("Check for Updates");
    setMinimumWidth(420);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 16);

    m_statusLabel = new QLabel("Checking for updates\u2026", this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    // Download buttons (hidden until an update is found with assets)
    auto *downloadRow = new QHBoxLayout;
    downloadRow->setSpacing(8);
    m_btnExe = new QPushButton("Download .exe", this);
    m_btnExe->setVisible(false);
    m_btnZip = new QPushButton("Download .zip", this);
    m_btnZip->setVisible(false);
    downloadRow->addStretch();
    downloadRow->addWidget(m_btnExe);
    downloadRow->addWidget(m_btnZip);
    downloadRow->addStretch();
    mainLayout->addLayout(downloadRow);

    // "Open releases page" button (shown when no releases exist)
    m_btnOpenPage = new QPushButton("Open Releases Page", this);
    m_btnOpenPage->setVisible(false);
    mainLayout->addWidget(m_btnOpenPage, 0, Qt::AlignCenter);

    mainLayout->addStretch();

    // Close / dismiss button
    auto *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    m_btnClose = new QPushButton("Close", this);
    connect(m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
    btnRow->addWidget(m_btnClose);
    mainLayout->addLayout(btnRow);

    connect(m_btnExe,      &QPushButton::clicked, this, &UpdateDialog::onDownloadExe);
    connect(m_btnZip,      &QPushButton::clicked, this, &UpdateDialog::onDownloadZip);
    connect(m_btnOpenPage, &QPushButton::clicked, this, &UpdateDialog::onOpenReleasesPage);

    m_nam = new QNetworkAccessManager(this);

    startCheck();
}

// ---------------------------------------------------------------------------
// Network – query latest release
// ---------------------------------------------------------------------------

void UpdateDialog::startCheck()
{
    QUrl apiUrl(kReleasesApiUrl);
    QNetworkRequest req(apiUrl);
    req.setRawHeader("Accept", "application/vnd.github+json");
    req.setRawHeader("User-Agent", "FilenameEdit-UpdateChecker/1.0");
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReleasesReply(reply);
        reply->deleteLater();
    });
}

void UpdateDialog::onReleasesReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        // HTTP 404 = no releases published yet
        const int httpStatus =
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (httpStatus == 404) {
            showNoReleases();
        } else {
            showError(reply->errorString());
        }
        return;
    }

    QJsonParseError parseErr;
    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        showError("Unexpected response from GitHub.");
        return;
    }

    const QJsonObject obj = doc.object();
    const QString latestTag  = obj["tag_name"].toString();
    const QString releaseUrl = obj["html_url"].toString();
    if (latestTag.isEmpty()) {
        showNoReleases();
        return;
    }

    // Find EXE and ZIP asset download URLs
    QString exeUrl, zipUrl;
    for (const QJsonValue &val : obj["assets"].toArray()) {
        const QJsonObject asset = val.toObject();
        const QString name = asset["name"].toString().toLower();
        const QString url  = asset["browser_download_url"].toString();
        if (!url.isEmpty()) {
            if (name.endsWith(".exe") && exeUrl.isEmpty())
                exeUrl = url;
            else if (name.endsWith(".zip") && zipUrl.isEmpty())
                zipUrl = url;
        }
    }

    // Compare version strings using QVersionNumber for proper semantic ordering
    QVersionNumber current = QVersionNumber::fromString(m_currentVersion);
    QVersionNumber latest  = QVersionNumber::fromString(latestTag.startsWith('v', Qt::CaseInsensitive)
                                                        ? latestTag.mid(1) : latestTag);
    if (current.isNull()) current = QVersionNumber::fromString(m_currentVersion);

    if (!latest.isNull() && current >= latest) {
        showUpToDate(latestTag);
    } else {
        showUpdateAvailable(latestTag, releaseUrl, exeUrl, zipUrl);
    }
}

// ---------------------------------------------------------------------------
// UI states
// ---------------------------------------------------------------------------

void UpdateDialog::showNoReleases()
{
    m_statusLabel->setText(
        "No releases have been published yet.\n"
        "Visit the repository page to check for future releases.");
    m_releasePageUrl = kNewReleasesPageUrl;
    m_btnOpenPage->setVisible(true);
    adjustSize();
}

void UpdateDialog::showError(const QString &msg)
{
    m_statusLabel->setText("Could not check for updates:\n" + msg);
    m_releasePageUrl = kReleasesPageUrl;
    m_btnOpenPage->setVisible(true);
    adjustSize();
}

void UpdateDialog::showUpToDate(const QString &latestTag)
{
    m_statusLabel->setText(
        QString("You are using the latest version (%1).").arg(latestTag));
    adjustSize();
}

void UpdateDialog::showUpdateAvailable(const QString &latestTag,
                                       const QString &releaseUrl,
                                       const QString &exeUrl,
                                       const QString &zipUrl)
{
    m_statusLabel->setText(
        QString("<b>New version %1 is available!</b><br>"
                "You are currently using version %2.<br><br>"
                "Download the new version below or open the releases page.")
            .arg(latestTag, m_currentVersion));
    m_statusLabel->setTextFormat(Qt::RichText);

    m_exeUrl        = exeUrl;
    m_zipUrl        = zipUrl;
    m_releasePageUrl = releaseUrl.isEmpty() ? kReleasesPageUrl : releaseUrl;

    m_btnExe->setVisible(!exeUrl.isEmpty());
    m_btnZip->setVisible(!zipUrl.isEmpty());
    m_btnOpenPage->setVisible(true);
    adjustSize();
}

// ---------------------------------------------------------------------------
// Button handlers
// ---------------------------------------------------------------------------

void UpdateDialog::onDownloadExe()
{
    if (!m_exeUrl.isEmpty())
        startDownload(m_exeUrl, "FilenameEdit-Setup.exe");
}

void UpdateDialog::onDownloadZip()
{
    if (!m_zipUrl.isEmpty())
        startDownload(m_zipUrl, "FilenameEdit.zip");
}

void UpdateDialog::onOpenReleasesPage()
{
    QDesktopServices::openUrl(QUrl(m_releasePageUrl.isEmpty() ? kReleasesPageUrl : m_releasePageUrl));
}

// ---------------------------------------------------------------------------
// Download logic
// ---------------------------------------------------------------------------

void UpdateDialog::startDownload(const QString &url, const QString &suggestedName)
{
    // Ask user for save location
    const QString downloadsDir =
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    const QString dest = QFileDialog::getSaveFileName(
        this, "Save Download",
        downloadsDir + "/" + suggestedName,
        "All files (*)");
    if (dest.isEmpty())
        return;

    // Disable download buttons during transfer
    m_btnExe->setEnabled(false);
    m_btnZip->setEnabled(false);

    QUrl downloadUrl(url);
    QNetworkRequest req(downloadUrl);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setRawHeader("User-Agent", "FilenameEdit-UpdateChecker/1.0");

    QNetworkReply *reply = m_nam->get(req);

    // Progress dialog
    auto *progress = new QProgressDialog("Downloading\u2026", "Cancel", 0, 0, this);
    progress->setWindowTitle("Downloading Update");
    progress->setWindowModality(Qt::WindowModal);
    progress->setMinimumDuration(0);
    progress->setValue(0);

    connect(reply, &QNetworkReply::downloadProgress,
            this, [progress](qint64 received, qint64 total) {
        if (total > 0) {
            // Use percentage (0-100) to avoid int overflow for large files
            const int pct = static_cast<int>(received * 100 / total);
            progress->setMaximum(100);
            progress->setValue(pct);
        }
    });

    connect(progress, &QProgressDialog::canceled, reply, &QNetworkReply::abort);

    connect(reply, &QNetworkReply::finished, this, [this, reply, progress, dest]() {
        progress->close();
        progress->deleteLater();

        m_btnExe->setEnabled(true);
        m_btnZip->setEnabled(true);

        if (reply->error() != QNetworkReply::NoError) {
            if (reply->error() != QNetworkReply::OperationCanceledError) {
                QMessageBox::warning(this, "Download Failed",
                                     "The download failed:\n" + reply->errorString());
            }
            reply->deleteLater();
            return;
        }

        QFile file(dest);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Save Failed",
                                 "Could not write to:\n" + dest);
            reply->deleteLater();
            return;
        }
        file.write(reply->readAll());
        file.close();
        reply->deleteLater();

        QMessageBox::information(this, "Download Complete",
                                 "File saved to:\n" + dest);
    });
}
