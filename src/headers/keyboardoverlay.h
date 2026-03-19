#pragma once
#include <QWidget>
#include <QTimer>
#include <QList>

struct TileEntry {
    QString text;
    bool isSpecial;
    enum State { Appearing, Active, Disappearing } state;
    qreal opacity;    
    qreal currentX;
    qreal targetX;
    int lifetimeMs;
};

class KeyboardOverlay : public QWidget {
    Q_OBJECT
public:
    explicit KeyboardOverlay(QWidget *parent = nullptr);

public slots:
    void onNormalKey(const QString &text);
    void onSpecialKey(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void tick();

private:
    void addTile(const QString &text, bool isSpecial);
    void clearAllTiles();
    void recomputeTargets();
    void drawTile(QPainter &painter, const TileEntry &entry, qreal rainbowAngle);

    QList<TileEntry> m_entries;
    bool m_specialMode = false;
    QTimer m_masterTimer;
    qreal m_globalRainbowAngle = 0.0;

    // 常量配置
    const int TILE_HEIGHT = 60;
    const int TILE_SPACING = 15;
    const int MAX_TILES = 10;
    const int DT_MS = 16;
    const int NORMAL_TIMEOUT = 2500;
    const int SPECIAL_TIMEOUT = 3500;
};