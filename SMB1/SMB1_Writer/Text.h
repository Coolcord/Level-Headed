#ifndef TEXT_H
#define TEXT_H

#include "Byte_Writer.h"
#include <QByteArray>
#include <QString>

class Text : public Byte_Writer {
public:
    Text(QFile *file, Level_Offset *levelOffset);
    ~Text() {}
    bool Replace_Castle_With_Text(const QString &text);
    bool Replace_Princess_With_Text(const QString &text);
    bool Replace_Thank_You_Player_One_With_Text(const QString &text);
    bool Replace_Thank_You_Player_Two_With_Text(const QString &text);
    bool Set_Mario_Name(const QString &name);
    bool Set_Luigi_Name(const QString &name);
    void Set_Special_P1_Name(const QString &name);
    void Set_Special_P2_Name(const QString &name);
    QByteArray Convert_String_To_SMB_Bytes(const QString &string);
    QString Convert_SMB_Bytes_To_String(const QByteArray &bytes);

private:
    bool Find_And_Replace_Next_Instance(const QString &oldText, QString newText, qint64 startingOffset, qint64 endingOffset);
    QString Get_Centered_Text(const QString &oldText, const QString &newText);
    QString Read_Thank_You_Mario_Text();
    QString Read_Thank_You_Mario_Text_Without_Name();
    QString Read_Thank_You_Mario_Text_Without_Thank_You();
    QString Read_Thank_You_Luigi_Text();
    QString Read_Thank_You_Luigi_Text_Without_Name();
    QString Read_Thank_You_Luigi_Text_Without_Thank_You();

    QString specialP1Name;
    QString specialP2Name;
    QString p1Name;
    QString p2Name;
    QString p1ThankYouText;
    QString p2ThankYouText;
};

#endif // TEXT_H
