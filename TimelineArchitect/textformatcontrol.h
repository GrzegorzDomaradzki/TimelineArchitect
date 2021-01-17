#ifndef TEXTFORMATCONTROL_H
#define TEXTFORMATCONTROL_H
#include <QString>
#include <cctype>


class TextFormatControl
{

public:
    static bool TagName(QString tag, QString& info);
    static bool TitleTextName(QString text, QString& info);


};

#endif // TEXTFORMATCONTROL_H
