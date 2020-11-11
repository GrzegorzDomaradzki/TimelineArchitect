#ifndef TAGS_H
#define TAGS_H

#include <QObject>

class Tags : public QObject
{
    Q_OBJECT
private:
    std::map<std::string,int> _tagsNames;
    std::vector<std::vector<int>> _tagOwners;

public:
    explicit Tags(QObject *parent = nullptr);
    int RegisterTag(QString);
    int RenameTag(QString);
    int RegisterTagOwner(QString,unsigned);
    int UnregisterTagOwner(QString,unsigned);

signals:

};

#endif // TAGS_H
