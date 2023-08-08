#include "pipeline.h"

Pipeline::Pipeline(QObject *parent)
    : QObject{parent}
{
    this->model = new LogModel(this);
    this->m_loading = false;
}
