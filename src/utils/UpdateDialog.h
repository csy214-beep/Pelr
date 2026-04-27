/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include "VersionChecker.h"

class QTextBrowser;
class QPushButton;
class QComboBox;
class QLabel;
class QVBoxLayout;

class UpdateDialog : public QDialog {
    Q_OBJECT

public:
    explicit UpdateDialog(const VersionCheckSummary &summary, QWidget *parent = nullptr);

    ~UpdateDialog();

private slots:
    void onDownloadClicked();

    void onSourceSelectionChanged(int index);

private:
    void setupUi();

    void updateDetailsForSource(const SourceCheckResult &result);

    VersionCheckSummary m_summary;
    QList<SourceCheckResult> m_successfulSources; // 只保留成功的源

    QLabel *m_localVersionLabel;
    QLabel *m_statusLabel;
    QComboBox *m_sourceComboBox;
    QTextBrowser *m_bodyBrowser;
    QPushButton *m_downloadButton;
    QPushButton *m_closeButton;
    QVBoxLayout *m_mainLayout;
};

#endif // UPDATEDIALOG_H
