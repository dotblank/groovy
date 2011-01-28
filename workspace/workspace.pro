TEMPLATE = subdirs

CONFIG += ordered

exists("$$PWD/../../qjson") {
    # So that we're able to deploy the project directly from Qt Creator
    # with all dependencies, the QJson library needs INSTALLS for Maemo
    !infile("$$PWD/../../qjson/src/src.pro",HAS_MAEMO_INSTALLS,yes) {
        win: CMD="type"
        else: CMD="cat"
        system("$$CMD \"$$PWD/qjson.pri\" >> $$PWD/../../qjson/src/src.pro")
    }
    SUBDIRS += ../../qjson
}
exists($$PWD/../../libgroove): SUBDIRS += ../../libgroove
SUBDIRS += ../groovy.pro
