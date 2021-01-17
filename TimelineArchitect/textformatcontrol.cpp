#include "textformatcontrol.h"


bool TextFormatControl::TagName(QString tag, QString& info)
{
    if (tag == NULL || tag.length()==0)
    {
        info = "tag name can't be empty";
        return false;
    }

    if (tag[0] == '_')
    {
        info = "tag name can't start with \"_\"";
        return false;
    }
    foreach (auto c, tag.toStdString()) {
        if (isspace(c) || (ispunct(c) && c!='_'))
        {
            info = "tag name can't consist spaces or punctation characters";
            return false;
        }
    }
    return true;
}

bool TextFormatControl::TitleTextName(QString text, QString &info)
{
    if (text.contains("<") || text.contains(">"))
    {
        info = "\">\" and \"<\" are special characters, and can't be used in event description";
        return false;
    }
    return true;
}
