#include "htmlcomposer.h"

HTMLComposer::HTMLComposer(QTextDocument* doc) : QSyntaxHighlighter(doc)
{
    /*styles.insert("SelectFrom", "<font style=\"color: lightblue;\">SELECT</font> * "
                                "<font style=\"color: lightblue;\">FROM</font> ");
    styles.insert("Where", "<font style=\"color: lightblue;\"> WHERE </font> ");
    styles.insert("InsertInto", "<font style=\"color: lightblue;\">INSERT INTO</font> ");
    styles.insert("Values", "<font style=\"color: lightblue;\">VALUES</font> ");
    styles.insert("SqlText", "<font style=\"color: darkgoldenrod;\">''</font>");
    styles.insert("SqlNumber", "<font style=\"color: dodgerblue;\">number</font>");*/
    keyStrList << "INSERT" << "INTO" << "SELECT" << "FROM" << "VALUES" << "WHERE";
}


void HTMLComposer::buildDark(QHash<QString, QString>& data)
{
#ifdef HTML
    auto toSQLTextStr = [this](const QString& value)->QString{
        QString tmp = this->styles["SqlText"];
        return tmp.insert(tmp.lastIndexOf("'"), value);
    };

    //
    auto toSQLNumber = [this, &toSQLTextStr](const QString& value)->QString{
        if(!QString::compare(value, "''", Qt::CaseInsensitive))
            return toSQLTextStr(value);
        QString tmp = this->styles["SqlNumber"];
        return tmp.replace(QRegExp("number"), value);
    };

    auto SelectFrom = [this](const QString& column)->QString{
        QString tmp = this->styles["SelectFrom"];
        return tmp.replace(QRegExp("*"), column);
    };
    // radar_type_table
    html = styles["InsertInto"] + "radar_type (name, band, country)<br>" +
           styles["Values"] +  "(" + toSQLTextStr(data["radar_name"]) + ", " +
           "{" + toSQLNumber(data["freq_low"]) + ", " + toSQLNumber(data["freq_high"]) + "}, "+ toSQLTextStr(data["country"]) +");<br>";
    html += styles["InsertInto"] + "work_mode (radar_id, pulse_power_min, pulse_power_max, "
                                   "pulse_len_min, pulse_len_max, pulse_period_min, pulse_period_max,"
                                   "antenna_gain, azimuth_coverage, elevation_angle_coverage, polarization"
                                   "rotation_rate, beamwidth_vert_min, beamwidth_vert_max, "
                                   "beamwidth_horiz_min, beamwidth_horiz_max, scan_sector_vert_min, scan_sector_vert_max,"
                                   "scan_sector_horiz_min, scan_sector_horiz_max)<br>" +
            styles["Values"] + "\n((" + SelectFrom("id") + "radar_type" + styles["Where"] +"name="+toSQLTextStr(data["radar_name"])+"), " +
            toSQLNumber(data["pulse_power_min"]) + ", " + toSQLNumber(data["pulse_power_max"]) + ", " + toSQLNumber(data["pulse_len_min"]) + ", " + toSQLNumber(data["pulse_len_max"]) +
            toSQLNumber(data["pulse_period_min"]) + ", " + toSQLNumber(data["pulse_period_max"]) + ", " + toSQLNumber(data["pulse_period_max"]) + ", " + toSQLNumber(data["antenna_gain"]) + ", " +
            toSQLNumber(data["azimuth"]) + ", " + toSQLNumber(data["elev_ang"]) + ", " + toSQLTextStr(data["pol_type"]) + toSQLNumber(data["rotation_rate"]) + ", " +
            toSQLNumber(data["beamwidth_vert_min"]) + ", " + toSQLNumber(data["beamwidth_vert_max"]) + ", " + toSQLNumber(data["beamwidth_horiz_min"]) + ", " + toSQLNumber(data["beamwidth_horiz_max"]) +
            toSQLNumber(data["scan_sector_vert_min"]) + ", " + toSQLNumber(data["scan_sector_vert_max"]) + ", " + toSQLNumber(data["scan_sector_horiz_min"]) + ", " + toSQLNumber(data["scan_sector_horiz_max"]) + ");";
#else
    html = "INSERT INTO radar_type (name, band, country)\n"
           "VALUES ('" + data["radar_name"] + "', {" + data["freq_low"] + ", " + data["freq_high"] + "}, '" + data["country"] + "');\n\n";
    html += "INSERT INTO work_mode (radar_id, pulse_power_min, pulse_power_max, "
            "pulse_len_min, pulse_len_max, pulse_period_min, pulse_period_max, "
            "antenna_gain, azimuth_coverage, elevation_angle_coverage, polarization, "
            "rotation_rate, beamwidth_vert_min, beamwidth_vert_max, "
            "beamwidth_horiz_min, beamwidth_horiz_max, scan_sector_vert_min, scan_sector_vert_max, "
            "scan_sector_horiz_min, scan_sector_horiz_max)\n"
            "VALUES ((SELECT id FROM radar_type WHERE name='"+ data["radar_name"] +"'), " + data["pulse_power_min"] + ", " + data["pulse_power_max"] + ", " +
            data["pulse_len_min"] + ", " + data["pulse_len_max"] + ", " + data["pulse_period_min"] + ", " + data["pulse_period_max"] + ", " + data["pulse_period_max"] + ", " +
            data["antenna_gain"] + ", " + data["azimuth"] + ", " + data["elev_ang"] + ", '" + data["pol_type"] + "', " + data["rotation_rate"] + ", " +
            data["beamwidth_vert_min"] + ", " + data["beamwidth_vert_max"] + ", " + data["beamwidth_horiz_min"] + ", " + data["beamwidth_horiz_max"] + ", " +
            data["scan_sector_vert_min"] + ", " + data["scan_sector_vert_max"] + ", " + data["scan_sector_horiz_min"] + ", " + data["scan_sector_horiz_max"] + ");";
#endif
}

void HTMLComposer::buildLight(QHash<QString, QString>& data)
{}

void HTMLComposer::build(QHash<QString, QString>& data)
{
    assert(!data.isEmpty());
    switch (StyleSheetHandler::stColor) {
        case ColorMode::DARK:
            buildDark(data);
        break;
        case ColorMode::LIGHT:
            buildLight(data);
        break;
    }
}

void HTMLComposer::highlightBlock(const QString &text){
    // true - inside string, false - other
    static bool str = false;
    // chars before space
    unsigned cBefSpace = 0;
    // begining indicator
    int beg = -1;
    // captured keyword
    QString key = "";
    auto clear = [&]()->void{
        cBefSpace = 0;
        beg = -1;
        key = "";
    };
    auto isNumber = [&](QString& key)->bool{
        bool ok = false;
        key.toDouble(&ok);
        return ok;
    };
    auto process_key = [&]()->void{
            if(str) return;
            if(cBefSpace > 0){
                if(keyStrList.contains(key, Qt::CaseInsensitive))
                    setFormat(beg, key.length(), QColor(173, 216, 230));
                else if(isNumber(key))
                    setFormat(beg, key.length(), QColor(30, 144, 255));
                else
                    setFormat(beg, key.length(), QColor(255, 255, 255));
                //clear();
            }
    };
    for(unsigned i = 0; i < text.length(); i++){
        // checking if char is apostrophe
        if(text[i] == '\''){
            str = !str;
            if(!str){
                setFormat(beg, key.length()+1, QColor(184, 134, 11));
                clear();
                continue;
            }
            clear();
            beg = i;
        }
        if(text[i] == '(' || text[i] == ')' ||
           text[i] == ' ' || text[i] == '{' ||
           text[i] == '}' || text[i] == ',' ||
           text[i] == ';'){
            clear();
            continue;
        }
        if (beg == -1)
            beg = i;
        cBefSpace++;
        key += text[i];
        process_key();
    }
}

QString HTMLComposer::getHTML()
{
    return html;
}
