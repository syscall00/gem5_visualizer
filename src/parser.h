#pragma once

#include <QList>
#include <QString>
#include <QObject>
#include <QQmlEngine>
#include <QMap>



class LogRecord : public QObject {

public:

    explicit LogRecord(QObject *parent = nullptr);

    QML_ELEMENT
    // Q_OBJECT

    LogRecord(const QString stage, bool isStalling, const QString address, const QString assembly, QString cycleNumber, int fuIndex = -1)
        : stage(stage), isStalling(isStalling), address(address), assembly(assembly), fuIndex(fuIndex), cycleNumber(cycleNumber) {}

    // copy constructor and assignment operator
    LogRecord(const LogRecord& other) {
        stage = other.stage;
        isStalling = other.isStalling;
        address = other.address;
        assembly = other.assembly;
        fuIndex = other.fuIndex;
        cycleNumber = other.cycleNumber;
    }

    // assignment operator
    LogRecord& operator=(const LogRecord& other) {
        stage = other.stage;
        isStalling = other.isStalling;
        address = other.address;
        assembly = other.assembly;
        fuIndex = other.fuIndex;
        cycleNumber = other.cycleNumber;
        return *this;
    }

    // equality operator
    bool operator==(const LogRecord& other) const {
        return stage == other.stage && isStalling == other.isStalling && address == other.address && assembly == other.assembly && fuIndex == other.fuIndex && cycleNumber == other.cycleNumber;
    }

    // inequality operator
    bool operator!=(const LogRecord& other) const {
        return !(*this == other);
    }

    const QString& GetStage() const { return stage; }
    bool IsStalling() const { return isStalling; }
    const QString& GetAddress() const { return address; }
    const QString& GetAssembly() const { return assembly; }
    int GetFuIndex() const { return fuIndex; }
    QString GetCycleNumber() const { return cycleNumber; }
private:
    QString stage;
    bool isStalling;
    QString cycleNumber;
    QString address;
    QString assembly;
    int fuIndex;
};



const LogRecord EMPTY_RECORD = LogRecord("", false, "", "", "", -1);
const QString PLACEHOLDER_ASM = "<assembly>";


enum Stage {
    Fetch,
    Decode,
    Execute,
    Memory,
    WriteBack,
    ReadAfterWrite,
    WriteAfterWrite,
    Stall

};

class Instruction : public QObject {
    QML_ELEMENT
public:
    explicit Instruction(QObject *parent = nullptr) : QObject(parent) {}
    Instruction(const QString& instruction, const QString& address) : address(address), instruction(instruction) {}
    void InsertInView(enum Stage s) {
        this->pipeView.push_back(s);
    }

    Instruction(const Instruction& other) {
        address = other.address;
        instruction = other.instruction;
        pipeView = other.pipeView;
    }

    Instruction& operator=(const Instruction& other) {
        address = other.address;
        instruction = other.instruction;
        pipeView = other.pipeView;
        return *this;
    }

    bool operator==(const Instruction& other) const {
        return address == other.address && instruction == other.instruction && pipeView == other.pipeView;
    }

    bool operator!=(const Instruction& other) const {
        return !(*this == other);
    }


private:

    QString address;
    QString instruction;
    QList<Stage> pipeView;

};
