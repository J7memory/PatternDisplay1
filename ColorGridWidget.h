#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QList>

// 自定义颜色网格控件
class ColorGridWidget : public QWidget {
    Q_OBJECT
public:
    explicit ColorGridWidget(QWidget* parent = nullptr);

    void setGridSize(int width, int height);
    void setPixelColors(const QList<QColor>& colors);
    QList<QColor> getPixelColors() const;

signals:
    void pixelClicked(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    int CELL_SIZE = 40;  // 允许修改大小
    int gridWidth;
    int gridHeight;
    QList<QColor> colors;
};
