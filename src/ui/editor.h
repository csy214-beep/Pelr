
#pragma once
#include <QLineEdit>
#include <QWidget>

namespace Ui
{
    class editor;
}

class EditorWidget : public QWidget
{
    Q_OBJECT
signals:
    void accepted();

public:
    void setData(const QString &category, const QString &name,
                 const QString &path, const QString &icon,
                 const QString &description);

    void selectFile(QLineEdit *lineEdit);

    QPair<QList<QString>, QList<QString>> getAllInfo();

    explicit EditorWidget(QWidget *parent = nullptr);

    ~EditorWidget();

private:
    Ui::editor *ui;
};
