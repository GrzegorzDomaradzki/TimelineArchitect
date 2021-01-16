#ifndef TAGS_H
#define TAGS_H

#include <QObject>

class Tags : public QObject
{
    Q_OBJECT
private:
    std::map<QString,unsigned> _tagsNames;
    std::map<int,std::vector<const unsigned*>> _tagOwners;
    unsigned _now;

public:
    explicit Tags(QObject *parent = nullptr);
    bool HasTag(QString);
    int RegisterTag(QString);
    int RenameTag(QString, QString);
    std::vector<QString> ListActiveTag();
    int RegisterTagOwner(QString,const unsigned*);
    int UnregisterTagOwner(QString,const unsigned*);
    int DeleteTag(QString);
    std::vector<const unsigned *> ProvideTagged(QString);

public slots:

signals:
    void OnDeletedTag(QString);
    void OnTagModify();

};

#endif // TAGS_H
