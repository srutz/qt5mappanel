
#include "searchpanel.h"
#include "datafetcher.h"
#include "util.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

SearchPanel::SearchPanel(MapPanel *mapPanel, QWidget *parent) : QWidget(parent), m_mapPanel(mapPanel)
{

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(9, 27, 13, 0);
    setLayout(layout);

    setStyleSheet("font-family: 'Roboto';");
    setAutoFillBackground(true);

    auto header = new QWidget(this);
    auto headerLayout = new QHBoxLayout(header);
    headerLayout->setSpacing(8);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    auto headerIcon = new QLabel(header);
    headerIcon->setStyleSheet("font-family: 'Lucide'; color: #444444;");
    Util::setLucideIcon(headerIcon, QString::fromUtf8("\uea98"), 36);
    headerLayout->addWidget(headerIcon, 0, Qt::AlignVCenter);

    auto headerLabel = new QLabel(header);
    headerLabel->setText(R"(<html>
        <h3>Search nominatim</h3>
        )");
    headerLabel->setWordWrap(true);
    headerLayout->addWidget(headerLabel, 1);

    layout->addWidget(header);
    layout->addSpacing(12);

    auto doSearch = [this](const QString &query) {
        auto url = QString("https://nominatim.openstreetmap.org/search?q=%1&format=json&limit=25").arg(query);
        auto fetcher = new DataFetcher(url, this);
        DataFetcher::FetchOptions options{.url = url};
        connect(fetcher, &DataFetcher::responseReceived, this, [this, fetcher](const QByteArray &data) {
            QString s = QString::fromUtf8(data);
            qDebug() << "Search results:" << s;
            fetcher->deleteLater();
        });
        connect(fetcher, &DataFetcher::error, this, [fetcher](const QString &message) {
            qDebug() << "Error fetching search results:" << message;
            fetcher->deleteLater();
        });
        fetcher->fetch(options);
    };
    auto searchLabel = new QLabel("Search locations:", this);
    Util::applyLabelStyle(searchLabel);
    layout->addWidget(searchLabel, 0, Qt::AlignLeft);
    auto searchEdit = new QLineEdit(this);
    connect(searchEdit, &QLineEdit::returnPressed, this, [this, searchEdit, &doSearch]() { doSearch(searchEdit->text()); });
    layout->addWidget(searchEdit, 1);
    auto searchButton = new QPushButton("Search", this);
    Util::applyFlatButtonStyle(searchButton);
    connect(searchButton, &QPushButton::clicked, this, [this, searchEdit, &doSearch] { doSearch(searchEdit->text()); });
    layout->addWidget(searchButton, 0, Qt::AlignRight);

    layout->addStretch();
}

SearchPanel::~SearchPanel() {}
