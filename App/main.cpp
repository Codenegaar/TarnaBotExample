#include <QCoreApplication>
#include <QNetworkProxy>

#include "gamebot.h"
using namespace Telegram;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GameBot bot("token",
                QNetworkProxy(QNetworkProxy::Socks5Proxy, "localhost", 9050),
                1000);
    return a.exec();
}
