/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "editor.h"
#include  "ui_editor.h"
#include  <QFileDialog>
#include <QMessageBox>

EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent), ui(new Ui::editor) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/assets/image/live_2d.png"));
    connect(ui->pushButton_2, &QPushButton::clicked, [&]() {
        selectFile(ui->lineEdit_2);
    });
    connect(ui->pushButton_3, &QPushButton::clicked, [&]() {
        selectFile(ui->lineEdit_3);
    });
    connect(ui->pushButton, &QPushButton::clicked, this, &EditorWidget::accepted);
}

QPair<QList<QString>, QList<QString> > EditorWidget::getAllInfo() {
    QString name = ui->lineEdit->text();
    QString path = ui->lineEdit_2->text();
    QString icon = ui->lineEdit_3->text();
    QString desc = ui->textEdit->toPlainText();
    QList<QString> category;
    if (name.isEmpty() || path.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("名称和路径不能为空"));
        return {};
    }
    if (ui->checkBox->isChecked()) {
        category.append("Star");
    }
    if (ui->checkBox_2->isChecked()) {
        category.append("App");
    }
    if (ui->checkBox_3->isChecked()) {
        category.append("Link");
    }
    if (ui->checkBox_4->isChecked()) {
        category.append("Scripts");
    }
    if (category.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("至少选择一个分类"));
        return {};
    }
    return {category, {name, path, icon, desc}};
}

void EditorWidget::selectFile(QLineEdit *lineEdit) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (fileName.isEmpty()) {
        return;
    }
    lineEdit->setText(fileName);
}

void EditorWidget::setData(const QString &category, const QString &name,
                           const QString &path, const QString &icon, const QString &description) {
    ui->lineEdit->setText(name);
    ui->lineEdit_2->setText(path);
    ui->lineEdit_3->setText(icon);
    ui->textEdit->setPlainText(description);

    // 设置分类复选框
    ui->checkBox->setChecked(category == "Star");
    ui->checkBox_2->setChecked(category == "App");
    ui->checkBox_3->setChecked(category == "Link");
    ui->checkBox_4->setChecked(category == "Scripts");
}

EditorWidget::~EditorWidget() {
    delete ui;
}
