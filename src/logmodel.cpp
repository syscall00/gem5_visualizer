#include "logmodel.h"

int LogModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
 \
    return m_data.count();
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const LogRecord &data = m_data.at(index.row());
    
    switch(role)
    {
        case Stage:
            return data.GetStage();
        case IsStalling:
            return data.IsStalling();
        case Address:
            return data.GetAddress();
        case Assembly:
            return data.GetAssembly();
        case CycleNumber:
            return data.GetCycleNumber();
        case FuIndex:
            return data.GetFuIndex();
        default:
            return QVariant();
    }   

}
