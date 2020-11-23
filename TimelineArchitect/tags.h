#ifndef TAGS_H
#define TAGS_H

#include <QObject>

class Tags : public QObject
{
    Q_OBJECT
private:
    std::map<QString,unsigned> _tagsNames;
    std::map<int,std::vector<unsigned>> _tagOwners;
    unsigned _now;

public:
    explicit Tags(QObject *parent = nullptr);
    bool HasTag(QString);
    int RegisterTag(QString);
    int RenameTag(QString, QString);
    int RegisterTagOwner(QString,unsigned);
    int UnregisterTagOwner(QString,unsigned);
    int DeleteTag(QString);

public slots:

signals:
    void OnDeletedTag(QString);
    void OnTagModify();

};

#endif // TAGS_H
