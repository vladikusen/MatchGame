#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QAbstractListModel>

#include <string>
#include <random>
#include <vector>
#include <thread>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class GameBoard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rows CONSTANT)
    Q_PROPERTY(int columns READ columns CONSTANT)
    Q_PROPERTY(int pointsCounter READ pointsCounter CONSTANT)
    Q_PROPERTY(int movesCounter READ movesCounter CONSTANT)
    Q_PROPERTY(int ifFirstRow READ ifFirstRow CONSTANT)
public:
    GameBoard(QObject* parent = nullptr);

    enum {
        colorRole = Qt::UserRole
    };

    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void move(int, int);
    Q_INVOKABLE bool checkBoard();
    Q_INVOKABLE bool checkIfThereIsMatches(int, int);
    Q_INVOKABLE void newGame();
    Q_INVOKABLE void increaseMoveCounter() { m_movesCounter++; }
    Q_INVOKABLE bool checkIfThereIsValidMoves();


    int columns() const;
    int rows() const;
    int pointsCounter() const;
    int movesCounter() const;

    bool ifFirstRow() const;

private:
    void shuffle();
    std::string getRandomColor();

    std::vector<std::string> m_colorsSet;
    std::vector<std::string> m_colorsArray;

    int m_columns;
    int m_rows;

    int m_pointsCounter;
    int m_movesCounter;

    std::pair<int, int> findMatchInRow(int);
    void fillRowWithBlank(std::pair<int, int>);
    void moveRowToTop(int);

    std::pair<int, int> findMatchInColumn(int);
    void fillColumnWithBlank(std::pair<int, int>);
    void moveColumnToTop(int);

    bool exchangeBlanks();

    bool checkRow(int);
    bool checkColumn(int);

    bool checkRows();
    bool checkColumns();

    void checkBoardInCycle();

    bool m_ifFirstRow;
};

#endif // GAMEBOARD_H
