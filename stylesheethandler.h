#ifndef STYLESHEETHANDLER_H
#define STYLESHEETHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>
#include "error.h"


class StyleSheetHandler : public QObject
{
    Q_OBJECT
public:
     enum ColorMode{
      DARK, LIGHT
    };
    StyleSheetHandler(const QString rcFileName);
    ~StyleSheetHandler();
    QString getText();
    void SetFileName(const QString& rcFileName);
    static ColorMode stColor;
signals:
private:
    QFile m_stSheet;
    QTextStream m_txtStream;
    explicit StyleSheetHandler(QObject *parent = nullptr);
    StyleSheetHandler(StyleSheetHandler& stSheet);
    void InitStream();
    void reInitStream();
};

typedef StyleSheetHandler::ColorMode ColorMode;

#endif // STYLESHEETHANDLER_H
