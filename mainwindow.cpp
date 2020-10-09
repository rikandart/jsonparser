#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef QT_DEBUG
#include <qDebug>
#endif

bool MainWindow::close = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifndef QT_DEBUG
    ui->pushButton_2->setVisible(false);
    ui->Sql->setTextInteractionFlags(Qt::TextBrowserInteraction);
#endif
    // setting up window type
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    InitShadow();
    InitComboUi();
    InitInputValidation();
    InitMainStyleSheet();
    // init here merely to be initialized once
    m_jc = new JSONComposer();
    m_hc = new HTMLComposer(ui->Sql->document());
}

MainWindow::~MainWindow()
{
    if(m_jc) delete m_jc;
    if(m_hc) delete m_hc;
    delete m_hStSh;
    delete m_countryValid;
    delete m_doubValid;
    delete m_freqValid;
    delete m_listView5;
    delete m_listView4;
    delete m_listView3;
    delete m_listView2;
    delete m_listView1;
    delete m_shadowEffect;
    delete ui;
}


void MainWindow::InitShadow(){
    // adding shadow to ui elements
    m_shadowEffect = new QGraphicsDropShadowEffect();
    m_shadowEffect->setColor(QColor(0, 0, 0, 255 * 0.2));
    m_shadowEffect->setXOffset(1);
    m_shadowEffect->setYOffset(1);
    m_shadowEffect->setEnabled(true);
    ui->freqLowerB->setGraphicsEffect(m_shadowEffect);
    ui->freqUpperB->setGraphicsEffect(m_shadowEffect);
    ui->antennaGain->setGraphicsEffect(m_shadowEffect);
    ui->pulsePower->setGraphicsEffect(m_shadowEffect);
    ui->pulsePower_2->setGraphicsEffect(m_shadowEffect);
    ui->pulseLength->setGraphicsEffect(m_shadowEffect);
    ui->pulseLength_2->setGraphicsEffect(m_shadowEffect);
    ui->pulsePeriod->setGraphicsEffect(m_shadowEffect);
    ui->pulsePeriod_2->setGraphicsEffect(m_shadowEffect);
    ui->Azimuth->setGraphicsEffect(m_shadowEffect);
    ui->EAng->setGraphicsEffect(m_shadowEffect);
    ui->BeamWidth->setGraphicsEffect(m_shadowEffect);
    ui->BeamWidth_2->setGraphicsEffect(m_shadowEffect);
    ui->BeamWidth_3->setGraphicsEffect(m_shadowEffect);
    ui->BeamWidth_4->setGraphicsEffect(m_shadowEffect);
    ui->ScanSector->setGraphicsEffect(m_shadowEffect);
    ui->ScanSector_2->setGraphicsEffect(m_shadowEffect);
    ui->ScanSector_3->setGraphicsEffect(m_shadowEffect);
    ui->ScanSector_4->setGraphicsEffect(m_shadowEffect);
    ui->UTF8->setGraphicsEffect(m_shadowEffect);
    ui->Windows1251->setGraphicsEffect(m_shadowEffect);
    ui->pushButton->setGraphicsEffect(m_shadowEffect);

}

void MainWindow::InitComboUi(){
    // setting up QListView pointer for using
    // styles inside QComboBox instance
    ui->freqUnitBox->setView(m_listView1 = new QListView());
    ui->polarizationTypeBox->setView(m_listView2 = new QListView());
    ui->timeUnitBox->setView(m_listView3 = new QListView());
    ui->timeUnitBox_2->setView(m_listView4 = new QListView());
    ui->siBox->setView(m_listView5 = new QListView());
}

// ellipsis function
// va_list is used to get values from the ellipsis
// and declared in <cstdarg> header
/*void MainWindow::setValidator(quint32 count, QLineEdit*...){
    va_list lineEditList;
    // intializing va_list
    va_start(lineEditList, count);
    for(quint32 i = 0; i < count; i++){
        // va_arg returns next specified type object in the chain
        va_arg(lineEditList, QLineEdit*)->setValidator(m_doubValid);
    }
    // cleaning up va_list when done
    va_end(lineEditList);
}*/

void MainWindow::InitInputValidation(){
    // validating lineEdits to be real numbers
    QRegExp name("[^\\/\\:\\*\\?\"\\<\\>\\|]+");
    m_nameValid = new QRegExpValidator(name);
    ui->RadarName->setValidator(m_nameValid);
    QRegExp positiveDouble("\\d*\\.?\\d*|\\d*,?\\d*", Qt::CaseInsensitive);
    m_freqValid = new QRegExpValidator(positiveDouble);
    QRegExp anyDouble("[-+]?\\d*\\.?\\d*|[-+]?\\d*,?\\d*", Qt::CaseInsensitive);
    m_doubValid = new QRegExpValidator(anyDouble);
    // setValidator(4, ui->lineEdit, ui->lineEdit_2, ui->lineEdit_3, ui->lineEdit_4);
    ui->freqLowerB->setValidator(m_freqValid);
    ui->freqUpperB->setValidator(m_freqValid);
    ui->antennaGain->setValidator(m_doubValid);
    ui->pulsePower->setValidator(m_doubValid);
    ui->pulsePower_2->setValidator(m_doubValid);
    ui->pulseLength->setValidator(m_doubValid);
    ui->pulseLength_2->setValidator(m_doubValid);
    ui->pulsePeriod->setValidator(m_doubValid);
    ui->pulsePeriod_2->setValidator(m_doubValid);
    ui->Azimuth->setValidator(m_doubValid);
    ui->EAng->setValidator(m_doubValid);
    ui->BeamWidth->setValidator(m_doubValid);
    ui->BeamWidth_2->setValidator(m_doubValid);
    ui->BeamWidth_3->setValidator(m_doubValid);
    ui->BeamWidth_4->setValidator(m_doubValid);
    ui->ScanSector->setValidator(m_doubValid);
    ui->ScanSector_2->setValidator(m_doubValid);
    ui->ScanSector_3->setValidator(m_doubValid);
    ui->ScanSector_4->setValidator(m_doubValid);

    m_countryValid = new QRegExpValidator(QRegExp("[a-z]+|[\u0410-\u044f]+", Qt::CaseInsensitive));
    ui->Country->setValidator(m_countryValid);

}

void MainWindow::InitMainStyleSheet(){
    m_hStSh = new StyleSheetHandler(":/stylesheets/mainframe_dark.qss");
    this->setStyleSheet(m_hStSh->getText());
}

void MainWindow::on_pushButton_clicked()
{
    // radar name
    ExQStr szIntermediate = ui->RadarName->text();
    szIntermediate.replace(QRegExp("/"), "\\");
    if(!szIntermediate.compare("")){
        Error er(ErrType::ERROR);
        er.ShowMessage(QString("Radar name field have to be a value!"));
        return;
    }
    m_dataTable.insert("radar_name", szIntermediate);

    //country
    szIntermediate = ui->Country->text();
    m_dataTable.insert("country", szIntermediate.compare("")?szIntermediate:QString(""));

    // frequency represented in Hz
    szIntermediate.asDoubStr(ui->freqLowerB->text());
    qDebug () << szIntermediate;
    szIntermediate = QString::number(szIntermediate.toDouble() * POSITIVE_POWER_10[ui->freqUnitBox->currentIndex()]);
    m_dataTable.insert("freq_low", szIntermediate.compare("0")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->freqUpperB->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * POSITIVE_POWER_10[ui->freqUnitBox->currentIndex()]);
    m_dataTable.insert("freq_high", szIntermediate.compare("0")?szIntermediate:QString("''"));

    // antenna gain stored in dB
    szIntermediate.asDoubStr(ui->antennaGain->text());
    m_dataTable.insert("antenna_gain", szIntermediate.compare("")?szIntermediate:QString("''"));

    // pulse power stored in watts
    szIntermediate.asDoubStr(ui->pulsePower->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * POSITIVE_POWER_10[ui->siBox->currentIndex()+1]);
    m_dataTable.insert("pulse_power_min", szIntermediate.compare("0")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->pulsePower_2->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * POSITIVE_POWER_10[ui->siBox->currentIndex()+1]);
    m_dataTable.insert("pulse_power_max", szIntermediate.compare("0")?szIntermediate:QString("''"));

    // pulse length stored in seconds
    szIntermediate.asDoubStr(ui->pulseLength->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * NEGATIVE_POWER_10[ui->timeUnitBox->currentIndex()]);
    m_dataTable.insert("pulse_len_min", szIntermediate.compare("0")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->pulseLength_2->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * NEGATIVE_POWER_10[ui->timeUnitBox->currentIndex()]);
    m_dataTable.insert("pulse_len_max", szIntermediate.compare("0")?szIntermediate:QString("''"));

    // pulse period stored in seconds
    szIntermediate.asDoubStr(ui->pulsePeriod->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * NEGATIVE_POWER_10[ui->timeUnitBox_2->currentIndex()]);
    m_dataTable.insert("pulse_period_min", szIntermediate.compare("0")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->pulsePeriod_2->text());
    szIntermediate = QString::number(szIntermediate.toDouble() * NEGATIVE_POWER_10[ui->timeUnitBox_2->currentIndex()]);
    m_dataTable.insert("pulse_period_max", szIntermediate.compare("0")?szIntermediate:QString("''"));

    // angle values stored in degrees
    szIntermediate.asDoubStr(ui->Azimuth->text());
    m_dataTable.insert("azimuth", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->EAng->text());
    m_dataTable.insert("elev_ang", szIntermediate.compare("")?szIntermediate:QString("''"));

    // rotation rate
    szIntermediate.asDoubStr(ui->RotationRate->text());
    m_dataTable.insert("rotation_rate", szIntermediate.compare("")?szIntermediate:QString("''"));

    // beamwidth in degrees
    szIntermediate.asDoubStr(ui->BeamWidth->text());
    m_dataTable.insert("beamwidth_vert_min", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->BeamWidth_2->text());
    m_dataTable.insert("beamwidth_vert_max", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->BeamWidth_3->text());
    m_dataTable.insert("beamwidth_horiz_min", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->BeamWidth_4->text());
    m_dataTable.insert("beamwidth_horiz_max", szIntermediate.compare("")?szIntermediate:QString("''"));

    // scan sector
    szIntermediate.asDoubStr(ui->ScanSector->text());
    m_dataTable.insert("scan_sector_vert_min", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->ScanSector_2->text());
    m_dataTable.insert("scan_sector_vert_max", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->ScanSector_3->text());
    m_dataTable.insert("scan_sector_horiz_min", szIntermediate.compare("")?szIntermediate:QString("''"));
    szIntermediate.asDoubStr(ui->ScanSector_4->text());
    m_dataTable.insert("scan_sector_horiz_max", szIntermediate.compare("")?szIntermediate:QString("''"));

    // polarization type
    szIntermediate.asDoubStr(ui->polarizationTypeBox->currentText());
    m_dataTable.insert("pol_type", szIntermediate.compare("")?szIntermediate:QString("''"));

    m_jc->put(m_dataTable);
    m_jc->save(this, encoding);

    m_hc->build(m_dataTable);
    ui->Sql->setPlainText(m_hc->getHTML());
}


void MainWindow::on_UTF8_released()
{
    encoding = "UTF-8";
}

void MainWindow::on_Windows1251_released()
{
    encoding = "Windows-1251";
}

void MainWindow::on_pushButton_2_clicked()
{
    StyleSheetHandler stsh("C:/Users/Pizhun_VD/Documents/parse_radarstation_to_json/resources/mainframe_dark.qss");
    this->setStyleSheet(stsh.getText());
}
