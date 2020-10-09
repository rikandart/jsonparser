#include "stylesheethandler.h"
#include "mainwindow.h"

StyleSheetHandler::StyleSheetHandler(QObject *parent) : QObject(parent){}
StyleSheetHandler::StyleSheetHandler(StyleSheetHandler& stSheet) : QObject(nullptr){}

ColorMode StyleSheetHandler::stColor = ColorMode::DARK;

StyleSheetHandler::StyleSheetHandler(const QString rcFileName) : QObject(nullptr)
{
    m_stSheet.setFileName(rcFileName);
    InitStream();
}

StyleSheetHandler::~StyleSheetHandler(){
    m_stSheet.close();
}

QString StyleSheetHandler::getText(){
    return m_txtStream.readAll();
}

void StyleSheetHandler::SetFileName(const QString &rcFileName){
    m_stSheet.setFileName(rcFileName);
    reInitStream();
}

void StyleSheetHandler::reInitStream(){
    m_txtStream.flush();
    InitStream();
}

void StyleSheetHandler::InitStream(){
    try {
        m_stSheet.open(QIODevice::ReadOnly | QIODevice::ExistingOnly);
        if(m_stSheet.exists())
            m_txtStream.setDevice(&m_stSheet);
        else
            throw QString("StyleSheet file not exists!");
    } catch (const QString errStr) {
        Error err;
        err.ShowMessage(errStr);
        if(err.result() == ErrResult::CLOSE)
            MainWindow::close = true;
    }
}
