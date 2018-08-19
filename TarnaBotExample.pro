TEMPLATE = subdirs

SUBDIRS += \
    TarnaBot \
    App

TarnaBot.subdir = lib/TarnaBot
App.depends = TarnaBot
CONFIG += ordered
