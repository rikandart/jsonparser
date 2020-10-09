#ifndef HTMLCOMPOSER_H
#define HTMLCOMPOSER_H

//#define HTML

#include <QObject>
#include <QString>
#include <QChar>
#include <QStringList>
#include <QHash>
#include <QColor>
#include <QDebug>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <cassert>

// local
#include "stylesheethandler.h"

class HTMLComposer : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit HTMLComposer(QTextDocument* doc = nullptr);
    void build(QHash<QString, QString>&);
    QString getHTML();


signals:

private:
    QString html;
    QStringList keyStrList;
    QHash<const QString, QString> styles;
    void buildDark(QHash<QString, QString>& data);
    void buildLight(QHash<QString, QString>& data);
    void highlightBlock(const QString &text) override;
};

#endif // HTMLCOMPOSER_H
