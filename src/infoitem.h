#ifndef INFO_ITEM_H
#define INFO_ITEM_H

#include <QLabel>
#include <QString>
#include <QWidget>

struct InfoValue {
    QString key;
    QString value;
};

class InfoItem : public QWidget
{
    Q_OBJECT
    InfoValue m_info;
    QLabel *m_keyLabel;
    QLabel *m_valueLabel;

  public:
    enum class InfoItemSize { Small, Medium, Large };

    explicit InfoItem(const InfoValue &info, InfoItemSize size, QWidget *parent = nullptr);
    ~InfoItem();

    InfoValue info() const;
    void setInfo(const InfoValue &info);
    void setKey(const QString &key);
    void setValue(const QString &value);

  private:
    void rehash();
};

#endif // INFO_ITEM_H
