#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <QListView>
#include <QValidator>
#include <QRegExp>
#include <cstdarg>
#include <QHash>
#include "stylesheethandler.h"
#include "jsoncomposer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const double POSITIVE_POWER_10[] = {1, 1000, 1000000, 1000000000};
const double NEGATIVE_POWER_10[] = {0.001, 0.000001, 0.000000001};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static bool close;

public slots:


private slots:
    void on_pushButton_clicked();

    void on_UTF8_released();

    void on_Windows1251_released();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsDropShadowEffect* m_shadowEffect;
    QListView* m_listView1, *m_listView2, *m_listView3, *m_listView4, *m_listView5;
    QRegExpValidator* m_countryValid, * m_freqValid, *m_doubValid, *m_nameValid;
    StyleSheetHandler* m_hStSh;
    JSONComposer* m_jc;
    HTMLComposer* m_hc;
    QHash<QString, QString> m_dataTable;
    QString encoding = "UTF-8";
    void InitShadow();
    void InitComboUi();
    void InitInputValidation();
    void InitMainStyleSheet();
    // elipsis causing segmentation fault somehow
    // void setValidator(quint32 count, QLineEdit*...);
    void setValidator();
};

class ExQStr: public QString{
public:
    ExQStr(QString& other) : QString(other){};
    ExQStr(QString other) : QString(other){};
    void asDoubStr(QString other){
        if (other.contains(','))
            other.replace(other.indexOf(','), 1, '.');
            *this = QString(other);
    };
    // operator QString()const {return QString(*this);};
};

#endif // MAINWINDOW_H
