#include "gameboard.h"

GameBoard::GameBoard(QObject* parent)
    : QAbstractListModel{parent}
{
    QFile file("/home/vshkilniuk/Documents/folder/MatchGame/TileGame/colorsVariety.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QJsonDocument colorSet = QJsonDocument::fromJson(QString(file.readAll()).toUtf8());
    file.close();

    QJsonObject jObject = colorSet.object();

    m_rows = jObject["rows"].toInt();

    m_columns = jObject["columns"].toInt();

    foreach(const QJsonValue& value, jObject["colors"].toArray()){
        m_colorsSet.push_back(value.toObject()["color"].toString().toStdString());
    }

    for(int i = 0; i < m_rows * m_columns; i++){
        m_colorsArray.push_back(getRandomColor());
    }

    checkBoardInCycle();


    m_pointsCounter = 0;
    m_movesCounter = 0;
}

int GameBoard::rowCount(const QModelIndex & parent = QModelIndex()) const
{
    Q_UNUSED(parent);
    return m_columns * m_rows;
}

QVariant GameBoard::data(const QModelIndex & index, int role) const
{
    switch(role){
    case colorRole:
        return QString::fromStdString(m_colorsArray[index.row()]);
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> GameBoard::roleNames() const
{
    return {{colorRole, "color"}};
}

int GameBoard::columns() const
{
    return m_columns;
}

int GameBoard::rows() const
{
    return m_rows;
}

void GameBoard::move(int sourceIndex, int destinationIndex)
{

    if(sourceIndex + 1 == destinationIndex){

        std::string buffer = m_colorsArray.at(sourceIndex);
        m_colorsArray.at(sourceIndex) = m_colorsArray.at(destinationIndex);
        m_colorsArray.at(destinationIndex) = buffer;

        emit rowsMoved(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), destinationIndex + 1, {});

        m_movesCounter++;
    }
    if(sourceIndex - 1 == destinationIndex){

        std::string buffer = m_colorsArray.at(sourceIndex);
        m_colorsArray.at(sourceIndex) = m_colorsArray.at(destinationIndex);
        m_colorsArray.at(destinationIndex) = buffer;


        emit rowsMoved(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), destinationIndex + 1, {});

        m_movesCounter++;
    }
    if(sourceIndex + m_columns == destinationIndex){

        std::string buffer = m_colorsArray.at(sourceIndex);
        m_colorsArray.at(sourceIndex) = m_colorsArray.at(destinationIndex);
        m_colorsArray.at(destinationIndex) = buffer;

        emit rowsMoved(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), destinationIndex + 1, {});
        emit rowsMoved(QModelIndex(), destinationIndex - 1, destinationIndex - 1, QModelIndex(), sourceIndex, {});

        m_movesCounter++;
    }
    if(sourceIndex - m_columns == destinationIndex){

        std::string buffer = m_colorsArray.at(sourceIndex);
        m_colorsArray.at(sourceIndex) = m_colorsArray.at(destinationIndex);
        m_colorsArray.at(destinationIndex) = buffer;

        emit rowsMoved(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), destinationIndex, {});
        emit rowsMoved(QModelIndex(), destinationIndex + 1, destinationIndex + 1, QModelIndex(), sourceIndex + 1, {});

        m_movesCounter++;
    }

}

std::pair<int, int> GameBoard::findMatchInRow(int currentRow)
{
    int startInCurrentRow = currentRow * m_columns;
    int endInCurrentRow = startInCurrentRow + m_columns - 1;

    int matchStart = -1;
    for(int i = startInCurrentRow; i < endInCurrentRow - 1; i++){
        if(m_colorsArray.at(i) == m_colorsArray.at(i + 1) && m_colorsArray.at(i) == m_colorsArray.at(i + 2)){
            matchStart = i;
            break;
        }
    }

    int matchCounter = 0;
    if(matchStart == -1){
        return {-1, -1};
    }

    std::string base = m_colorsArray.at(matchStart);
    for(int i = matchStart; i <= endInCurrentRow; i++){
        if(m_colorsArray.at(i) == base){
            matchCounter++;
        }
        else{
            break;
        }
    }
    if(matchStart / m_columns == 0){
        m_ifFirstRow = true;
    }
    return {matchStart, matchCounter};
}

void GameBoard::fillRowWithBlank(std::pair<int, int> matchInfo)
{
    for(int i = 0; i < matchInfo.second; i++){
        m_colorsArray.at(matchInfo.first + i) = "transparent";
        dataChanged(createIndex(matchInfo.first + i, 0), createIndex(matchInfo.first + i, 0));
    }
}

void GameBoard::moveRowToTop(int row)
{
    int start = row * m_columns;
    int end = (row + 1) * m_columns;

    for(int i = start; i < end; i++){
        if(m_colorsArray.at(i) == "transparent"){
            int currentRow = i;
            while(currentRow - m_columns >= 0){
                std::string buffer = m_colorsArray.at(currentRow);
                m_colorsArray.at(currentRow) = m_colorsArray.at(currentRow - m_columns);
                m_colorsArray.at(currentRow - m_columns) = buffer;

                emit rowsMoved(QModelIndex(), currentRow - m_columns, currentRow - m_columns, QModelIndex(), currentRow + 1, {});
                emit rowsMoved(QModelIndex(), currentRow - 1, currentRow - 1, QModelIndex(), currentRow - m_columns, {});

                currentRow -= m_columns;
            }
        }
    }
}


std::pair<int, int> GameBoard::findMatchInColumn(int currentColumn)
{
    int start = currentColumn;
    int end = (m_rows - 1) * m_columns + currentColumn;

    int matchStart = -1;
    for(int i = start; i + m_columns + m_columns <= end; i += m_columns){
        if(m_colorsArray.at(i) == m_colorsArray.at(i + m_columns) && m_colorsArray.at(i) == m_colorsArray.at(i + m_columns + m_columns)){
            matchStart = i;
            break;
        }
    }

    if(matchStart == -1){
        return {-1, -1};
    }

    int matchCounter = 0;
    std::string base = m_colorsArray.at(matchStart);
    for(int i = matchStart; i <= end; i += m_columns){
        if(m_colorsArray.at(i) == base){
            matchCounter++;
        }
        else{
            break;
        }
    }

    return {matchStart, matchCounter};
}

void GameBoard::fillColumnWithBlank(std::pair<int, int> matchInfo)
{
    for(int i = 0; i < matchInfo.second; i++){
        m_colorsArray.at(matchInfo.first + i * m_columns) = "transparent";
        dataChanged(createIndex(matchInfo.first + i * m_columns, 0), createIndex(matchInfo.first + i * m_columns, 0));
    }
}

void GameBoard::moveColumnToTop(int index)
{
    int start = index;
    int end = (m_rows - 1) * m_columns + index;

    for(int i = start; i <= end; i += m_columns){
        if(m_colorsArray.at(i) == "transparent"){
            int currentRow = i;
            while(currentRow - m_columns >= 0){
                std::string buffer = m_colorsArray.at(currentRow);
                m_colorsArray.at(currentRow) = m_colorsArray.at(currentRow - m_columns);
                m_colorsArray.at(currentRow - m_columns) = buffer;

                emit rowsMoved(QModelIndex(), currentRow - m_columns, currentRow - m_columns, QModelIndex(), currentRow + 1, {});
                emit rowsMoved(QModelIndex(), currentRow - 1, currentRow - 1, QModelIndex(), currentRow - m_columns, {});


                currentRow -= m_columns;
            }
        }
    }
}

bool GameBoard::exchangeBlanks()
{
    bool was = false;

    for(int i = 0; i < m_colorsArray.size(); i++){
        if(m_colorsArray.at(i) == "transparent"){
            m_pointsCounter++;
            was = true;
            emit rowsRemoved(QModelIndex(), i, i, {});
            m_colorsArray.at(i) = getRandomColor();
            emit rowsInserted(QModelIndex(), i, i, {});
        }
    }

    return was;
}

bool GameBoard::checkRow(int row)
{
    m_ifFirstRow = false;
    fillRowWithBlank(findMatchInRow(row));
    moveRowToTop(row);
    bool was = exchangeBlanks();

    return was;
}

bool GameBoard::checkColumn(int column)
{
    fillColumnWithBlank(findMatchInColumn(column));
    moveColumnToTop(column);
    bool was = exchangeBlanks();

    return was;
}

bool GameBoard::checkRows()
{
    for(int i = 0; i < m_rows; i++){
        if(checkRow(i)){
            return true;
        }
    }

    return false;
}

bool GameBoard::checkColumns()
{
    for(int i = 0; i < m_columns; i++){
        if(checkColumn(i)){
            return true;
        }
    }

    return false;
}

void GameBoard::checkBoardInCycle()
{
    bool was = checkBoard();
    if(!was){
        was = !checkIfThereIsValidMoves();
    }
    while(was){
        was = checkBoard();
        if(!was){
            was = !checkIfThereIsValidMoves();
        }
    }
}

bool GameBoard::ifFirstRow() const
{
    return m_ifFirstRow;
}

bool GameBoard::checkBoard()
{
    bool was = false;
    if(!checkRows()){
        was = checkColumns();
    }
    else{
        was = true;
    }

    return was;
}

void GameBoard::shuffle()
{
    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    std::shuffle(m_colorsSet.begin(), m_colorsSet.end(), generator);
}

std::string GameBoard::getRandomColor()
{
    shuffle();
    return m_colorsSet.at(0);
}

bool GameBoard::checkIfThereIsMatches(int sourceIndex, int destinationIndex)
{
    std::string buffer = m_colorsArray.at(sourceIndex);
    m_colorsArray.at(sourceIndex) = m_colorsArray.at(destinationIndex);
    m_colorsArray.at(destinationIndex) = buffer;

    bool is = false;
    for(int i = 0; i < m_rows; i++){
        if(findMatchInRow(i).first != -1){
            is = true;
        }
    }

    for(int i = 0; i < m_columns; i++){
        if(findMatchInColumn(i).first != -1){
            is = true;
        }
    }

    buffer = m_colorsArray.at(sourceIndex);
    m_colorsArray.at(sourceIndex) = m_colorsArray.at(destinationIndex);
    m_colorsArray.at(destinationIndex) = buffer;

    return is;
}

void GameBoard::newGame()
{

    for(int i = 0; i < m_colorsArray.size(); i++){
        m_colorsArray.at(i) = getRandomColor();
    }

    checkBoardInCycle();

    m_pointsCounter = 0;
    m_movesCounter = 0;
    emit dataChanged(createIndex(0, 0), createIndex(m_colorsArray.size() - 1, 0));
}

bool GameBoard::checkIfThereIsValidMoves()
{
    bool was = false;
    for(int i = 0; i < m_colorsArray.size() - 1 ; i++){
        if(i % (m_columns - 1) != 0){
            if(checkIfThereIsMatches(i, i + 1)){
                was = true;
            }
        }
    }

    for(int i = 0; i < m_colorsArray.size(); i++ ){
        if(i / m_columns != m_rows - 1){
            if(checkIfThereIsMatches(i, i + m_columns)){
                was = true;
            }
        }
    }

    return was;
}

int GameBoard::movesCounter() const
{
    return m_movesCounter;
}

int GameBoard::pointsCounter() const
{
    return m_pointsCounter;
}
