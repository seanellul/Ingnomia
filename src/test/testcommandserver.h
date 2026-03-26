#pragma once

#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>

class ImGuiBridge;
class MainWindow;

// Reads JSON commands from stdin on a worker thread
class StdinReader : public QObject
{
	Q_OBJECT

public:
	StdinReader( QObject* parent = nullptr );

public slots:
	void run();

signals:
	void commandReceived( const QJsonObject& cmd );
	void finished();
};

// Dispatches JSON commands to game systems and writes JSON responses to stdout
class TestCommandServer : public QObject
{
	Q_OBJECT

public:
	TestCommandServer( ImGuiBridge* bridge, MainWindow* window, QObject* parent = nullptr );
	~TestCommandServer();

	void start();

private slots:
	void onCommand( const QJsonObject& cmd );

private:
	void sendResponse( const QJsonObject& response );
	QJsonObject handleGetState();
	QJsonObject handleScreenshot( const QJsonObject& cmd );
	QJsonObject handleAdvanceTicks( const QJsonObject& cmd );
	QJsonObject handleLoadSave( const QJsonObject& cmd );
	QJsonObject handleSetPaused( const QJsonObject& cmd );
	QJsonObject handleSetGameSpeed( const QJsonObject& cmd );
	QJsonObject handleBuild( const QJsonObject& cmd );
	QJsonObject handleSpawnCreature( const QJsonObject& cmd );
	QJsonObject handleGetMetrics();
	QJsonObject handleNewGame( const QJsonObject& cmd );
	QJsonObject handleGetPerf();
	QJsonObject handleBenchmark( const QJsonObject& cmd );

	void onHeartbeat( int value );
	void onLoadGameDone( bool success );
	void onNewGameDone( bool success );

	ImGuiBridge* m_bridge;
	MainWindow* m_window;
	QThread m_readerThread;

	// For advance_ticks command
	int m_targetTicks = 0;
	int m_currentTicks = 0;
	quint64 m_startTick = 0; // GameState::tick at start of advance
	bool m_waitingForTicks = false;
	bool m_waitingForLoad = false;
	bool m_waitingForNewGame = false;
	QElapsedTimer m_newGameTimer;

	// For benchmark command
	bool m_benchmarkRunning = false;
	QJsonArray m_benchmarkSizes;
	int m_benchmarkTicksPerSize = 200;
	int m_benchmarkIndex = 0;
	QJsonArray m_benchmarkResults;
};
