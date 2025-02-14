#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QToolButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void saveHtml();

private:
    Ui::MainWindow *ui;

    // UI-Elemente als Member-Variablen
    QToolButton *closeButton;
    QToolButton *minimizeButton;
    QTextEdit *htmlTextEdit;
    QLineEdit *filenameLineEdit;
    QPushButton *saveButton;

    // Drag-Funktionalität für Fensterbewegung
    bool dragging = false;  // Standardwert setzen
    QPoint dragStartPosition;

    // Hilfsfunktion für modern gestaltete Fehlermeldungen
    void showStyledMessage(const QString &title, const QString &text, QMessageBox::Icon icon);

protected:
    // Mausereignisse überschreiben für Fensterbewegung
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
