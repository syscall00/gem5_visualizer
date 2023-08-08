#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QFile>
#include "./parser.h"


class LogModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(unsigned int length READ rowCount NOTIFY lengthChanged)
public:
    enum InstructionRoles {
        Stage = Qt::UserRole + 1,
        IsStalling,
        Address,
        Assembly,
        CycleNumber,
        FuIndex,
        };




    QHash<int, QByteArray> roleNames() const override{
        QHash<int, QByteArray> roles;
        roles[Stage] = "stage";
        roles[IsStalling] = "isStalling";
        roles[Address] = "address";
        roles[Assembly] = "assembly";
        roles[CycleNumber] = "cycleNumber";
        roles[FuIndex] = "fuIndex";
        return roles;
    }

    LogModel(QObject *parent = nullptr) { this->setParent(parent); }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void SetData(QList<LogRecord>& data) { m_data = data; emit dataChanged(index(0), index(m_data.count())); emit lengthChanged(); }

    const QList<LogRecord>& GetData() const { return m_data; }


signals:
    void lengthChanged();
private:
   QList<LogRecord> m_data;

};
