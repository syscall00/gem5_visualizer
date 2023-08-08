#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QMap>
#include <QList>
#include "src/parser.h"
#include <QFile>
#include "logmodel.h"
#include <QFileInfo>
#include <QtConcurrent>
#include <QDir>


const QString LOG_HEADER = "Log4GUI: ";
const QString SEPARATOR = ": ";

const QString STAGE_FETCH = "fetch1";
const QString STAGE_DECODE = "decode";
const QString STAGE_EXECUTE = "execute";

// TODO: Still to implement in gem5
const QString STAGE_MEMORY = "memory";
const QString STAGE_WRITEBACK = "writeback";

const QString BOOLEAN_TRUE = "1";

class Pipeline : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(LogModel *model READ getLogs NOTIFY mdChanged)
    Q_PROPERTY(bool loading MEMBER m_loading NOTIFY loadingChanged);
public:
    explicit Pipeline(QObject *parent = nullptr);
    
    QMap<int, QList<LogRecord>> ParseLogs(const QString &filename)
    {
        QVector<LogRecord> instructions;
        QMap<int, QList<LogRecord>> instructions_by_tick;

        QFile inputFile(filename);
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (line.contains(LOG_HEADER))
                {
                    const QString removeHeader = line.split(LOG_HEADER)[1];
                    const QStringList split = removeHeader.split(SEPARATOR);

                    const QString stage = split[0];
                    const QString cycleNumber = split[1];
                    const QString isStalling = split[2];
                    const QString address = split[3];
                    const QString assembly = split[4];
                    int fuIndex = stage == STAGE_EXECUTE ? split[5].toInt() : -1;

                    LogRecord inst = LogRecord(stage, isStalling == BOOLEAN_TRUE, address, assembly, cycleNumber, fuIndex);
                    instructions.append(inst);
                }
            }

            // // create a map of instructions divided by ticks
            for (const auto &instr : instructions)
            {
                // check if list exists. If not, create it
                if (!instructions_by_tick.contains(instr.GetCycleNumber().toInt()))
                {
                    instructions_by_tick.insert(instr.GetCycleNumber().toInt(), QList<LogRecord>());
                }
                instructions_by_tick[instr.GetCycleNumber().toInt()].append(instr);
            }
        }

        return instructions_by_tick;
    }

    QList<LogRecord>& GenerateInsts() {
        // starting from parsed logs, do the following:
        // 1. Discard all logs stalling

        all.clear();
        for (auto& tick : instructions.keys()) {
            // get list of instructions for this tick
            QList<LogRecord> instructionss = instructions.value(tick);
            for (const auto& instr : instructionss) {
                if (instr.IsStalling()) {
                    continue;
                }
                all.append(instr);
            }
        }

        return all;
    }

public slots: 
    void setFilename(QString filename) {

        // replace file:// with empty string
        filename = filename.replace("file://", "");

        // check if filename exists and it is a file
        QFileInfo check_file(filename);
        
        if (!(check_file.exists() && check_file.isFile())) {
            qDebug() << "File does not exist" << check_file << " " << filename;
            return;
        }

        this->filename = filename;
        init();
    }

    LogModel* getLogs() {
        return model;
    }
    const QString& getFilename() const {
        return filename;
    }

    const QMap<int, QList<LogRecord>>& getInstructions() const {
        return instructions;
    }



    QList<LogRecord>& getAll() {
        all.clear();

        // all = new QList<LogRecord>();
        for (auto& tick : instructions.keys()) {
            // get list of instructions for this tick
            QList<LogRecord> instructionss = instructions.value(tick);
            for (const auto& instr : instructionss) {
                all.append(instr);
            }
        }

        return all;
    }

    void init() {
        QtConcurrent::run([this]() {
            m_loading = true;
            emit loadingChanged();
            // Code in this block will run in another thread
            instructions = ParseLogs(filename);
            // this->model->SetData(getAll());
            this->model->SetData(GenerateInsts());
            emit mdChanged();
            m_loading = false;
            emit loadingChanged();
        });

    }
    
signals:

    void mdChanged();
    void loadingChanged();

private:
    QString filename = "/home/syscall/gem5_visualizer/out";

    QMap<int, QList<LogRecord>> instructions;
    QList<Instruction> instrs;
    LogModel* model;
    QList<LogRecord> all;
    bool m_loading;
};

