/*
* PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QDialog>
#include <QDateTime>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class DateTimePickerDialog : public QDialog {
Q_OBJECT

public:
    explicit DateTimePickerDialog(QWidget *parent = nullptr,
                                  const QDateTime &initialDateTime = QDateTime::currentDateTime(),
                                  const QString &title = "选择日期和时间");

    ~DateTimePickerDialog();

    QDateTime getSelectedDateTime() const;

    static QDateTime getDateTime(QWidget *parent = nullptr,
                                 const QDateTime &initialDateTime = QDateTime::currentDateTime(),
                                 bool *ok = nullptr);

private slots:

    void retranslateUI();

    void onTodayClicked();

    void onTomorrowClicked();

    void onNextWeekClicked();

    void onDateChanged(const QDate &date);

private:
    void setupUI();

    void setupShortcuts();

    QDateEdit *m_dateEdit; // 改为 QDateEdit
    QTimeEdit *m_timeEdit;
    QDateTime m_selectedDateTime;
};

