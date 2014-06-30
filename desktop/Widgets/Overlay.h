#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>
#include "Overlay.h"

class Overlay : public QWidget
{
public:
    explicit Overlay(QWidget * parent = 0) : QWidget(parent) {
        if (parent) {
            parent->installEventFilter(this);
            raise();
        }
    }
protected:
    //! Catches resize and child events from the parent widget
    bool eventFilter(QObject * obj, QEvent * ev) {
        if (obj == parent()) {
            if (ev->type() == QEvent::Resize) {
                QResizeEvent * rev = static_cast<QResizeEvent*>(ev);
                resize(rev->size());
            }
            else if (ev->type() == QEvent::ChildAdded) {
                raise();
            }
        }
        return QWidget::eventFilter(obj, ev);
    }
    //! Tracks parent widget changes
    bool event(QEvent* ev) {
        if (ev->type() == QEvent::ParentAboutToChange) {
            if (parent()) parent()->removeEventFilter(this);
        }
        else if (ev->type() == QEvent::ParentChange) {
            if (parent()) {
                parent()->installEventFilter(this);
                raise();
            }
        }
        return QWidget::event(ev);
    }
};



#endif // OVERLAY_H
