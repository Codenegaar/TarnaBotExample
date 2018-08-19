#include "gamebot.h"
using namespace Telegram;

GameBot::GameBot(QString token, QNetworkProxy proxy, qlonglong interval, QObject *parent) :
    TarnaBot(token, proxy, interval, parent)
{
    connect(this, &GameBot::updateReceived, this, &GameBot::handleUpdate);
}

void GameBot::handleUpdate(Update update)
{
    //Determine whether a button was pressed or a /start command was sent
    if(update.hasCallbackQuery())
    {
        processQuery(update.getCallbackQuery());
    }
    if(update.hasMessage() && update.getMessage().hasText())
    {
        if(update.getMessage().getText().startsWith("/start"))
            sendMyGame(0, update.getMessage().getChat().getId(), -1);
    }
}

void GameBot::processQuery(CallbackQuery query)
{
    //check if data is valid, and decode it
    QVector<int> values = decode(query.getData());
    if(values.isEmpty())
        return;

    //Check if the game is finished
    if(values[0] == 99)
    {
        sendMessage(query.getMessage().getChat().getId(),
                    "Congratulations! You have finished the game!");
        return;
    }
    //assign new score based on the key pressed, and update the message
    sendMyGame(values[1] + values[2] == values[3] ? values[0] + 1 : values[0],
            query.getMessage().getChat().getId(),
            query.getMessage().getMessageId());
}

void GameBot::sendMyGame(int newScore, qint64 chatId, qint64 messageId)
{
    //Create random numbers, create keyboard using this numbers
    QVector<int> numbers = createRandomList();
    InlineKeyboardMarkup replyMarkup = createKeyboard(newScore, numbers);

    QString messageText = QString("Score: %1\n"
                                  "%2 + %3 = ?").arg(QString::number(newScore),
                                                     QString::number(numbers[0]),QString::number(numbers[1]));
    //Send or update message
    if(messageId < 0)
    {
        sendMessage(chatId, messageText, "", false, false, -1, &replyMarkup);
    }

    else
        editMessageText(messageText, QString::number(chatId), messageId, "", "", false, &replyMarkup);
}

QVector< int > GameBot::createRandomList()
{
    QVector< int > numbers;
    QRandomGenerator random(QDateTime::currentDateTime().toSecsSinceEpoch());
    numbers.resize(8);

    //Generate the numbers about which we'll ask
    numbers[0] = random.bounded(20) + 1;
    numbers[1] = random.bounded(20) + 1;
    //Find a random place for the correct answer
    int resultIndex = random.bounded(6) + 2;

    //Generate some random numbers
    for(int i = 2; i < 8; i++)
    {
        if(i == resultIndex)
            continue;
        numbers[i] = random.bounded(40) + 1;
    }

    numbers[resultIndex] = numbers[0] + numbers[1];
    return numbers;
}

QString GameBot::encode(QVector<int> values)
{
    //values: score, a, b, result
    QString encoded = "";

    foreach(int i, values)
    {
        if(i < 10)
            encoded += '0';
        encoded += QString::number(i);
    }
    return encoded;
}

QVector< int > GameBot::decode(QString data)
{
    QVector< int > values;
    if(data.length() != 8)
        return values;
    for(int i =  0; i < 8; i+= 2)
    {
        values.append(QString(data.mid(i, 2)).toInt());
    }
    return values;
}

InlineKeyboardMarkup GameBot::createKeyboard(int score, QVector<int> numbers)
{
    QVector< QVector< InlineKeyboardButton > > buttons;
    QVector< int > values;
    values.resize(4);
    values[0] = score;
    values[1] = numbers[0];
    values[2] = numbers[1];
    buttons.resize(2);

    for(int i = 0; i < 2; i++)
    {
        buttons[i].resize(3);
        for(int j = 0; j < 3; j++)
        {
            values[3] = numbers[i * 3 + j + 2];
            buttons[i][j] = InlineKeyboardButton(QString::number(values[3]));
            buttons[i][j].setCallbackData(encode(values));
        }
    }
    return InlineKeyboardMarkup(buttons);
}
