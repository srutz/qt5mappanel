
#include "searchpanel.h"
#include "datafetcher.h"
#include "maputil.h"
#include "util.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

SearchPanel::SearchPanel(MapPanel *mapPanel, QWidget *parent) : QWidget(parent), m_mapPanel(mapPanel)
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(9, 27, 13, 0);
    setLayout(layout);

    setAutoFillBackground(true);
    setStyleSheet(
        "SearchPanel { font-family: 'Gravitas One'; background-color: #ffffff; }"
        "QLabel { font-family: 'Gravitas One'; }");

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
    // headerLabel->setStyleSheet("font-family: 'Gravitas One'; ");

    layout->addWidget(header);
    layout->addSpacing(12);

    auto searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Enter search query");
    layout->addWidget(searchEdit, 1);
    auto searchButton = new QPushButton("Search", this);
    Util::applyFlatButtonStyle(searchButton);
    connect(searchButton, &QPushButton::clicked, this, [this, searchEdit, searchButton] {
        // Disable button and set wait cursor
        searchButton->setEnabled(false);
        QApplication::setOverrideCursor(Qt::WaitCursor);

        auto encodedQuery = QString::fromUtf8(QUrl::toPercentEncoding(searchEdit->text().trimmed()));
        auto url = QString("https://nominatim.openstreetmap.org/search?q=%1&format=json&limit=1000").arg(encodedQuery);
        auto fetcher = new DataFetcher("nominatim search", this);
        DataFetcher::FetchOptions options{.url = url};
        connect(fetcher, &DataFetcher::responseReceived, this, [this, fetcher, searchButton](const QByteArray &data) {
            QString s = QString::fromUtf8(data);
            auto results = NominatimResult::fromJsonString(s);
            applyResults(results);

            // Re-enable button and restore cursor
            searchButton->setEnabled(true);
            QApplication::restoreOverrideCursor();
            fetcher->deleteLater();
        });
        connect(fetcher, &DataFetcher::error, this, [fetcher, searchButton](const QString &message) {
            qDebug() << "Error fetching search results:" << message;

            // Re-enable button and restore cursor
            searchButton->setEnabled(true);
            QApplication::restoreOverrideCursor();
            fetcher->deleteLater();
        });
        fetcher->fetch(options);
    });
    connect(searchEdit, &QLineEdit::returnPressed, this, [this, searchButton]() { searchButton->click(); });
    layout->addWidget(searchButton, 0, Qt::AlignRight);
    layout->addSpacing(12);

    auto resultsLabel = new QLabel("Results:", this);
    Util::applyLabelStyle(resultsLabel);
    layout->addWidget(resultsLabel, 0, Qt::AlignLeft);
    m_resultsTable = new QTableView(this);
    m_resultsTable->setStyleSheet("QTableView { font-size: 13px; border: 1px solid #dddddd; }");
    m_resultsTable->setSortingEnabled(false);
    m_resultsTable->horizontalHeader()->setStretchLastSection(true);
    m_resultsTable->verticalHeader()->hide();
    m_resultsTable->horizontalHeader()->hide();
    m_resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(m_resultsTable, 1);
    layout->addSpacing(12);

    layout->addStretch();

    QTimer::singleShot(1, this, [searchEdit]() { searchEdit->setFocus(); });
}

SearchPanel::~SearchPanel() {}

void SearchPanel::applyResults(const QVector<NominatimResult> &results)
{
    m_mapPanel->setMarkers(results);

    // Zoom to fit all results with padding
    if (!results.isEmpty()) {
        m_mapPanel->zoomToFitMarkers(0.15);
    }

    // set the results in the table view
    // for simplicity, we will just show the display_name in a single column
    auto model = new QStandardItemModel(results.size(), 1, this);
    model->setHeaderData(0, Qt::Horizontal, "Place");
    for (int row = 0; row < results.size(); ++row) {
        auto item = new QStandardItem(results[row].display_name);
        model->setItem(row, 0, item);
    }
    m_resultsTable->setModel(model);

    // Reconnect selection handler after model change
    connect(
        m_resultsTable->selectionModel(), &QItemSelectionModel::currentChanged, this,
        [this](const QModelIndex &current, const QModelIndex &) {
            const auto &results = m_mapPanel->markers();
            if (current.isValid() && current.row() < results.size()) {
                const auto &result = results[current.row()];
                auto pos = MapUtil::latLonToPosition(result.lat, result.lon, m_mapPanel->zoom());
                m_mapPanel->setMapPositionCentered(pos);
            }
        });
}
