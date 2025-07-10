// PatternDrawDialog.h
#ifndef PATTERNDRAWDIALOG_H
#define PATTERNDRAWDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QVector>
#include <memory>
// #include "Layer.h"
#include "Pattern.h"

class PatternDrawDialog : public QDialog {
    Q_OBJECT

public:
    explicit PatternDrawDialog(std::shared_ptr<Pattern> pattern, QWidget* parent = nullptr);

    void updatePattern(std::shared_ptr<Pattern> newPattern);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event);

private:
    std::shared_ptr<Pattern> pattern;
};

#endif // PATTERNDRAWDIALOG_H
