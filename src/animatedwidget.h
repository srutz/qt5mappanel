#ifndef ANIMATEDWIDGET_H
#define ANIMATEDWIDGET_H

#include <QPropertyAnimation>
#include <QWidget>

/*
 * A widget whose position is animated
 *
 * To use this widget either use   setPosition
 * or  setPositionA  (the A stands for animated)
 *
 * You can also use QObject::move()  but it will not update
 * the internal position managed by this object. Most likely
 * you don't want this to happen
 *
 */

class AnimatedWidget : public QWidget
{
    Q_OBJECT
public:
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)

    explicit AnimatedWidget(QWidget *parent = nullptr);
    QPoint position() const;
    void setPosition(const QPoint position);
    void setPositionA(const QPoint position, int durationMs = 350, std::function<void()> onComplete = nullptr); // animated

signals:
    void positionChanged(const QPoint point);

private:
    QPoint m_position;
    QPropertyAnimation *positionAnimation;
};

#endif // ANIMATEDWIDGET_H
