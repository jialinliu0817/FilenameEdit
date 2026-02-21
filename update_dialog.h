#ifndef UPDATE_DIALOG_H
#define UPDATE_DIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QPushButton;
class QNetworkAccessManager;
class QNetworkReply;

// Dialog that queries the GitHub Releases API and presents update information.
// Supports downloading EXE/ZIP assets with a progress indicator.
class UpdateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UpdateDialog(const QString &currentVersion, QWidget *parent = nullptr);

private slots:
    void onReleasesReply(QNetworkReply *reply);
    void onDownloadExe();
    void onDownloadZip();
    void onOpenReleasesPage();

private:
    void startCheck();
    void showChecking();
    void showNoReleases();
    void showError(const QString &msg);
    void showUpToDate(const QString &latestTag);
    void showUpdateAvailable(const QString &latestTag, const QString &releaseUrl,
                             const QString &exeUrl,    const QString &zipUrl);
    void startDownload(const QString &url, const QString &suggestedName);

    QString m_currentVersion;
    QNetworkAccessManager *m_nam = nullptr;

    QLabel      *m_statusLabel  = nullptr;
    QPushButton *m_btnExe       = nullptr;
    QPushButton *m_btnZip       = nullptr;
    QPushButton *m_btnOpenPage  = nullptr;
    QPushButton *m_btnClose     = nullptr;

    QString m_exeUrl;
    QString m_zipUrl;
    QString m_releasePageUrl;
};

#endif // UPDATE_DIALOG_H
