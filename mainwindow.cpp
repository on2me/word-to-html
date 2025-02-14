#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QLabel>
#include <QMouseEvent>
#include <QClipboard>
#include <QApplication>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QIcon>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setze das App-Icon, das auch in der Taskleiste angezeigt wird
    setWindowIcon(QIcon(":/Aufgaben-App.png"));

    // Fenster ohne Rahmen, fixierte Größe und moderner, dunkler Farbverlauf-Hintergrund
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(600, 500);
    setStyleSheet("QMainWindow { background: #1E1E1E; }");

    // Zentrales Widget und Layout erstellen
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Titelzeile für Fenstersteuerung (Minimieren/Schließen) und App-Titel
    QHBoxLayout *titleBarLayout = new QHBoxLayout();
    titleBarLayout->setContentsMargins(10, 10, 10, 0);
    titleBarLayout->setSpacing(10);

    // App-Titel
    QLabel *titleLabel = new QLabel("HTML Saver", this);
    titleLabel->setStyleSheet("QLabel { color: white; font-size: 18px; }");

    // Abstandhalter zwischen Titel und Buttons
    QSpacerItem *titleSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Minimieren-Button (mit Text "-")
    QToolButton *minimizeButton = new QToolButton(this);
    minimizeButton->setText("-");
    minimizeButton->setToolTip("Minimieren");
    minimizeButton->setStyleSheet(
        "QToolButton { background: transparent; color: white; font-size: 18px; }"
        "QToolButton:hover { background-color: rgba(255, 255, 255, 0.1); }"
        );
    connect(minimizeButton, &QToolButton::clicked, this, &MainWindow::showMinimized);

    // Schließen-Button (mit Text "x")
    QToolButton *closeButton = new QToolButton(this);
    closeButton->setText("x");
    closeButton->setToolTip("Schließen");
    closeButton->setStyleSheet(
        "QToolButton { background: transparent; color: white; font-size: 18px; }"
        "QToolButton:hover { background-color: rgba(255, 0, 0, 0.5); }"
        );
    connect(closeButton, &QToolButton::clicked, this, &MainWindow::close);

    // Komponenten zum Titel-Layout hinzufügen
    titleBarLayout->addWidget(titleLabel);
    titleBarLayout->addItem(titleSpacer);
    titleBarLayout->addWidget(minimizeButton);
    titleBarLayout->addWidget(closeButton);

    mainLayout->addLayout(titleBarLayout);

    // Inhaltlicher Bereich mit etwas Padding
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setStyleSheet("background: #252525; border-top-left-radius: 15px; border-top-right-radius: 15px;");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(15);

    // HTML-Code Label und TextEdit
    QLabel *htmlLabel = new QLabel("Gib deinen HTML-Code ein:", this);
    htmlLabel->setStyleSheet("QLabel { color: white; font-size: 16px; }");
    contentLayout->addWidget(htmlLabel);

    htmlTextEdit = new QTextEdit(this);
    htmlTextEdit->setFixedHeight(150);
    htmlTextEdit->setStyleSheet("QTextEdit { background-color: #333333; color: white; border: none; border-radius: 10px; padding: 10px; font-size: 14px; }");
    htmlTextEdit->setAcceptDrops(true);
    contentLayout->addWidget(htmlTextEdit);

    // Dateiname Label und LineEdit
    QLabel *filenameLabel = new QLabel("Dateiname (optional):", this);
    filenameLabel->setStyleSheet("QLabel { color: white; font-size: 14px; }");
    contentLayout->addWidget(filenameLabel);

    filenameLineEdit = new QLineEdit(this);
    filenameLineEdit->setStyleSheet("QLineEdit { background-color: #333333; color: white; border: none; border-radius: 10px; padding: 5px; font-size: 14px; }");
    // Erlaubte Zeichen: Buchstaben, Zahlen und die Sonderzeichen '-', '_' und '!'
    QRegularExpression rx("^[a-zA-Z0-9\\-\\_\\!]*$");
    filenameLineEdit->setValidator(new QRegularExpressionValidator(rx, this));
    contentLayout->addWidget(filenameLineEdit);

    // Speichern-Button
    saveButton = new QPushButton("Speichern", this);
    saveButton->setFixedHeight(40);
    saveButton->setStyleSheet("QPushButton { background-color: #007ACC; color: white; border: none; border-radius: 10px; font-size: 16px; }"
                              "QPushButton:hover { background-color: #005A8D; }");
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveHtml);
    contentLayout->addWidget(saveButton);

    contentLayout->addStretch();

    // Link am unteren Rand
    QLabel *linkLabel = new QLabel("<a href=\"https://wordtohtml.net/\">https://wordtohtml.net/</a>");
    linkLabel->setAlignment(Qt::AlignCenter);
    linkLabel->setStyleSheet("QLabel { color: #00A9E0; font-size: 14px; }"
                             "QLabel:hover { text-decoration: underline; }");
    linkLabel->setOpenExternalLinks(true);
    contentLayout->addWidget(linkLabel);

    mainLayout->addWidget(contentWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Hilfsfunktion zum Anzeigen gestylter MessageBoxen
void MainWindow::showStyledMessage(const QString &title, const QString &text, QMessageBox::Icon icon)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(icon);
    msgBox.setStyleSheet(
        "QLabel { color: white; font-size: 14px; }"
        "QMessageBox { background-color: #2D2D30; }"
        "QPushButton { background-color: #007ACC; color: white; border: none; border-radius: 5px; padding: 5px 10px; }"
        "QPushButton:hover { background-color: #005A8D; }"
        );
    msgBox.exec();
}

void MainWindow::saveHtml()
{
    QString htmlCode = htmlTextEdit->toPlainText().trimmed();
    QString filename = filenameLineEdit->text().trimmed();

    if (htmlCode.isEmpty()) {
        showStyledMessage("Fehler", "Bitte gib HTML-Code ein!", QMessageBox::Critical);
        return;
    }

    // Falls kein Dateiname angegeben wurde, verwende einen Zeitstempel
    if (filename.isEmpty()) {
        filename = QString::number(QDateTime::currentSecsSinceEpoch());
    }
    else {
        // Erlaubt sind jetzt nur Buchstaben, Zahlen und die Sonderzeichen '-', '_' und '!'
        QRegularExpression filenameRegEx("^[a-zA-Z0-9\\-\\_\\!]+$");
        QRegularExpressionMatch match = filenameRegEx.match(filename);
        if (!match.hasMatch()) {
            showStyledMessage("Fehler",
                              "Der Dateiname darf nur Buchstaben, Zahlen und die Sonderzeichen '-', '_' und '!' enthalten!",
                              QMessageBox::Critical);
            return;
        }
    }

    QString filePath = "articles/" + filename + ".html";
    QDir dir;
    if (!dir.exists("articles")) {
        dir.mkpath("articles");
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << htmlCode;
        file.close();
        showStyledMessage("Erfolg", "HTML-Datei wurde gespeichert: " + filePath, QMessageBox::Information);
    } else {
        showStyledMessage("Fehler", "Fehler beim Speichern der Datei.", QMessageBox::Critical);
    }
}

// Überschreibe Mausereignisse, um das Fenster (auch über den Titelbereich) per Linksklick zu ziehen
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dragStartPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}
