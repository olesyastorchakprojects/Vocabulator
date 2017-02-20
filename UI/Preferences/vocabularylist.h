#ifndef VOCABULARYLIST_H
#define VOCABULARYLIST_H

#include <QWidget>

class QTreeView;
class TreeModelPreferences;

class VocabularyList : public QWidget
{
    Q_OBJECT
public:
    explicit VocabularyList(QWidget *parent = 0);

private slots:
    void addVocabulary();
    void deleteVocabulary();
    void editVocabulary();

private:
    void setupLayout();

private:
    QTreeView* _view;
    TreeModelPreferences* _model;
};

#endif // VOCABULARYLIST_H
