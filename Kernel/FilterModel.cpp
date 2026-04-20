#include "FilterModel.h"


FilterModel::FilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

QString FilterModel::templatePattern() const
{
    return m_template;
}

void FilterModel::setTemplatePattern(const QString &newName)
{
    if (m_template == newName)
      return;


    qDebug() << "new templat " << newName;
    beginFilterChange();
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    QString pattern = QRegularExpression::escape(newName);

    QRegularExpression regularExpression(pattern, options);
    setFilterRegularExpression(regularExpression);

    m_template = newName;
    endFilterChange();
}

void FilterModel::setRoleToFilter(int role)
{
    if (m_role2Filter == role)
        return;
    m_role2Filter = role;
}


bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    const QString data = index.data(m_role2Filter).toString();

    bool templateEmpty = m_template.isEmpty();
    if(templateEmpty)
        return true;

    if(m_template.length()<3)
        return true;

    return data.contains(filterRegularExpression());
}
