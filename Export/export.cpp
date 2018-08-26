#include "export.h"

#include <QDialog>
#include <QList>
#include <QListWidget>
#include <QCalendarWidget>
#include <QDateTime>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

#include "Database/projectstable.h"
#include "Objects/project.h"

#include "Config/config.h"


void Export::getProjectsAndDates(QList<QString>& projects, QString& start, QString& end)
{
    QDialog* dialog = new QDialog;

    QRect screen = Config::screenGeometry();

    dialog->setMinimumSize(screen.width() * 0.5, screen.height() * 0.5 );

    QVBoxLayout* layout = new QVBoxLayout;

    _listWidget = new QListWidget;

    QList<Project> projs = ProjectsTable::projects();
    foreach (const Project& proj, projs)
    {
        QDateTime projTime = QDateTime::fromString(proj.createdAt());
        QListWidgetItem* item = new QListWidgetItem(QString("%1").arg(proj.value()));
        item->setCheckState(Qt::Unchecked);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        _listWidget->addItem(item);

        _projectDates.insert(proj.value(), projTime.date());
    }

    QCalendarWidget* calendarWidget = new QCalendarWidget;
    calendarWidget->setGridVisible(true);
    connect(calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(dateClicked(QDate)));

    //layout->addWidget(calendarWidget);

    QVBoxLayout* editLayout = new QVBoxLayout;
    _start = new QTextEdit;
    _start->setReadOnly(false);
    _start->setMaximumSize(screen.width() * 0.1, screen.height() * 0.1);

    _end = new QTextEdit;
    _end->setReadOnly(false);
    _end->setMaximumSize(screen.width() * 0.1, screen.height() * 0.1);

    QPushButton* button = new QPushButton("Export");
    button->setMaximumWidth(100);
    connect(button, SIGNAL(clicked(bool)), dialog, SLOT(close()));

    editLayout->addWidget(_start);
    editLayout->addWidget(_end);
    //editLayout->addSpacerItem(new QSpacerItem(0, 100, QSizePolicy::Expanding));
    editLayout->addWidget(button);

    QHBoxLayout* horLayout = new QHBoxLayout;
    horLayout->addWidget(calendarWidget);
    horLayout->addLayout(editLayout);

    layout->addLayout(horLayout);
    layout->addStretch(10);
    layout->addWidget(_listWidget);
    dialog->setLayout(layout);

    dialog->exec();

    for(int i = 0; i < _listWidget->count(); ++i)
    {
        QListWidgetItem* item = _listWidget->item(i);
        if(item && (item->checkState() == Qt::Checked))
        {
            projects.push_back(item->text());
        }
    }

    start = _start->toPlainText();
    end = _end->toPlainText();
}

void Export::dateClicked(const QDate& date)
{
    if(_start->toPlainText().isEmpty())
        _start->append(date.toString());
    else if(_end->toPlainText().isEmpty())
        _end->append(date.toString());

    if(!_start->toPlainText().isEmpty() && !_end->toPlainText().isEmpty())
    {
        for(int i = 0; i < _listWidget->count(); ++i)
        {
            QListWidgetItem* item = _listWidget->item(i);
            if((_projectDates[item->text()] >= QDate::fromString(_start->toPlainText())) &&
               (_projectDates[item->text()] <= QDate::fromString(_end->toPlainText())))
            {
                item->setCheckState(Qt::Checked);
            }
            else
            {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
    else
    {
        for(int i = 0; i < _listWidget->count(); ++i)
        {
            QListWidgetItem* item = _listWidget->item(i);
            item->setCheckState(Qt::Unchecked);
        }
    }
}

void Export::exportToTxt()
{
    QList<QString> projects;
    QString start, end;

    getProjectsAndDates(projects, start, end);

    QString filename = "Data.txt";
    QFile file(filename);
    if(file.exists())
        file.remove();

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        QList<Project> projs = ProjectsTable::projects();
        foreach (const Project& proj, projs)
        {
            if(projects.contains(proj.value()))
            {
                stream << proj.url() << "\r\n\r\n";
                stream << proj.value() << "\r\n\r\n";

                QList<Word> data = proj.words();
                int index = 0;
                foreach (Word word, data)
                {
                    stream << QString::number(++index) << ". " << word.value() << "\r\n";
                    QList<Definition> defs = word.definitions();
                    foreach (Definition def, defs)
                    {
                        stream << "Definition: " << def.value() << "\r\n";
                        QList<Example> exs = def.examples();
                        foreach (Example ex, exs)
                        {
                            stream << "Example: " << ex.value() << "\r\n";
                        }
                    }
                   stream << "\r\n";
                }
            }
        }
    }
}
