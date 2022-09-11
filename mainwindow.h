#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class QWebEngineView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readSettings();

    void saveSettings();

signals:
    void sigLoadUrl(const QString &);
private slots:

    void onLoadUrl(const QString &path);

    void on_addBtn_clicked();

    void slotOpenUrls();

    void on_deleteBtn_clicked();

    void on_clearBtn_clicked();

    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_setTime_clicked();

private:
    Ui::MainWindow *ui;
    QStringList m_openUrls;
    QTimer* m_timer{nullptr};
    int m_timeMSec = 60000 ;//1min
    bool isStop = false;
    QList <QWebEngineView *>m_listWeb;
};

#endif // MAINWINDOW_H
