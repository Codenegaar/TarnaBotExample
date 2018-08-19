#ifndef GAMEBOT_H
#define GAMEBOT_H

#include <QRandomGenerator>

#include <TarnaBot>
#include <InlineKeyboardMarkup>

namespace Telegram
{
    class GameBot : public TarnaBot
    {
    public:
        GameBot(QString token, QNetworkProxy proxy, qlonglong interval, QObject* parent = nullptr);

    public slots:
        void handleUpdate(Update update);

    private:
        void processQuery(CallbackQuery query);
        void sendMyGame(int newScore, qint64 chatId, qint64 messageId);
        InlineKeyboardMarkup createKeyboard(int score, QVector<int> numbers);

        QVector<int> decode(QString data);
        QString encode(QVector<int> values);
        QVector<int> createRandomList();
    };
}

#endif // GAMEBOT_H
