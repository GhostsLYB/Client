#ifndef GLOBALDATE_H
#define GLOBALDATE_H

#include <QObject>
#include <QMap>
#include <QList>

class GlobalDate : public QObject
{
    Q_OBJECT
public:
    explicit GlobalDate(QObject *parent = nullptr);
    static QString currentUserName(){return userName;}
    static void setCurrentUserName(QString name){userName = name;}
    static void insertUserImageUrl(QString userName, QString imageUrl);
    static QString getImageUrl(QString userName);
private:
    static QString userName;
    static QMap<QString,QString> userImageUrlMap;
signals:

public slots:
};

#endif // GLOBALDATE_H
