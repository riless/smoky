#ifndef QLIGHTBOXWIDGET_H
#define QLIGHTBOXWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QEvent>
#include <QPaintEvent>
#include <QChildEvent>
#include <QPainter>

/**
 * @brief Класс перекрытия
 */
class QLightBoxWidget : public QWidget
{
	Q_OBJECT

public:
	explicit QLightBoxWidget(QWidget* _parent, bool _folowToHeadWidget = false):
	QWidget(_parent),
	m_isInUpdateSelf(false)
{

	Q_ASSERT_X(_parent, "", Q_FUNC_INFO);

	if (_folowToHeadWidget) {
		while (_parent->parentWidget() != 0) {
			_parent = _parent->parentWidget();
		}
		setParent(_parent);
	}

	_parent->installEventFilter(this);
	setVisible(false);
}

protected:
	/**
	 * @brief Переопределяется для отслеживания собитий родительского виджета
	 */
	bool eventFilter(QObject* _object, QEvent* _event){

	if (_event->type() == QEvent::ChildAdded) {
		QChildEvent* childEvent = dynamic_cast<QChildEvent*>(_event);
		if (childEvent->child() != this) {
			QWidget* parent = parentWidget();
			setParent(0);
			setParent(parent);
		}
	}

	if (isVisible()
		&& _event->type() == QEvent::Resize) {
		updateSelf();
	}
	return QWidget::eventFilter(_object, _event);
}

	/**
	 * @brief Переопределяется для того, чтобы эмитировать эффект перекрытия
	 */
	void paintEvent(QPaintEvent* _event){
	//
	// Рисуем фон
	//
	QPainter p;
	p.begin(this);
	// ... фото родительского виджета
	p.drawPixmap(0, 0, width(), height(), m_parentWidgetPixmap);
	// ... накладываем затемнённую область
	p.setBrush(QBrush(QColor(0, 0, 0, 220)));
	p.drawRect(0, 0, width(), height());
	p.end();

	//
	// Рисуем всё остальное
	//
	QWidget::paintEvent(_event);
}

	/**
	 * @brief Переопределяется для того, чтобы перед отображением настроить внешний вид
	 */
	void showEvent(QShowEvent* _event){
	//
	// Обновим себя
	//
	updateSelf();

	//
	// Показываемся
	//
	QWidget::showEvent(_event);
}

private:
	/**
	 * @brief Обновить размер и картинку фона
	 */
	void updateSelf(){
	if (!m_isInUpdateSelf) {
		m_isInUpdateSelf = true;

		{
			//
			// Обновляем отображение
			//
			hide();
			resize(parentWidget()->size());
			m_parentWidgetPixmap = grabParentWidgetPixmap();
			show();
		}

		m_isInUpdateSelf = false;
	}
}

	/**
	 * @brief Разрешает конфликт рекурсивного обновления
	 */
	bool m_isInUpdateSelf;

	/**
	 * @brief Обновить картинку фона
	 */
	QPixmap grabParentWidgetPixmap() const
{
	QPixmap parentWidgetPixmap;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	parentWidgetPixmap = parentWidget()->grab();
#else
	parentWidgetPixmap = QPixmap::grabWidget(parentWidget());
#endif

	return parentWidgetPixmap;
}

	/**
	 * @brief Картинка фона
	 */
	QPixmap m_parentWidgetPixmap;
};

#endif // QLIGHTBOXWIDGET_H
