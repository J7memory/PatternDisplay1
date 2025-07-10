// LayerDrawDialog.h
#ifndef LAYERDRAWDIALOG_H
#define LAYERDRAWDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QVector>
#include <memory>
 #include "Layer.h"

class LayerDrawDialog : public QDialog {
    Q_OBJECT

public:
    explicit LayerDrawDialog(QWidget* parent = nullptr);
     void setLayers(const QVector<std::shared_ptr<Layer>>& layersList);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event);

private:
     QVector<std::shared_ptr<Layer>> layers;  // 一个Pattern可以包含多个Layer
};

#endif // LAYERDRAWDIALOG_H