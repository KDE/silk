#include <QtGui>
#include <QtNetwork>
#include <iostream>
#include <QDebug>

#include "fakeserver.h"

FakeServer::FakeServer(QObject* parent)
:  QThread( parent )
{

    moveToThread(this);

}
FakeServer::~FakeServer()
{
  quit();
  wait();
}
void FakeServer::startAndWait()
{
  start();
  // this will block until the event queue starts
  QMetaObject::invokeMethod( this, "started", Qt::BlockingQueuedConnection );
}

void FakeServer::newConnection()
{
    QMutexLocker locker(&m_mutex);
    m_clientSocket = m_tcpServer->nextPendingConnection();
    connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    //this->writeServerPart();
}
void FakeServer::dataAvailable()
{
    QMutexLocker locker(&m_mutex);
    readClientPart();
    writeServerPart();
}
void FakeServer::run()
{
    m_tcpServer = new QTcpServer();
    if ( !m_tcpServer->listen( QHostAddress( QHostAddress::LocalHost ), 12566 ) ) {

    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    exec();

    delete m_clientSocket;

    delete m_tcpServer;
}

void FakeServer::started()
{
  // do nothing: this is a dummy slot used by startAndWait()
}

void FakeServer::setScenario( const QString &scenario )
{
    QMutexLocker locker(&m_mutex);

    m_scenarios.clear();
    m_scenarios << scenario;
}

void FakeServer::addScenario( const QString &scenario )
{
    QMutexLocker locker(&m_mutex);

    m_scenarios << scenario;


}

void FakeServer::addScenarioFromFile( const QString &fileName )
{
  QFile file( fileName );
  file.open( QFile::ReadOnly );
  QTextStream in(&file);

  QString scenario;

  // When loading from files we never have the authentication phase
  // force jumping directly to authenticated state.

  while ( !in.atEnd() ) {
    scenario.append( in.readLine() );
  }

  file.close();

  addScenario( scenario );
}

bool FakeServer::isScenarioDone( int scenarioNumber ) const
{
  QMutexLocker locker(&m_mutex);

  if ( scenarioNumber < m_scenarios.size() ) {
    return m_scenarios[scenarioNumber].isEmpty();
  } else {
    return true; // Non existent hence empty, right?
  }
}

bool FakeServer::isAllScenarioDone() const
{
  QMutexLocker locker( &m_mutex );

  foreach ( const QString &scenario, m_scenarios ) {
    if ( !scenario.isEmpty() ) {
      return false;
    }
  }
  return true;
}
void FakeServer::writeServerPart()
{

    QString retour = m_scenarios.isEmpty() ? QString("vide") : m_scenarios.takeFirst();
    QString scenario = "HTTP/1.0 200 Ok\r\nContent-Type: text/html; charset=\"utf-8\"\r\nSet-Cookie: name=value\r\n\r\n" + retour;
    m_clientSocket->write( scenario.toLocal8Bit() );
    //clientSocket->waitForDisconnected();
    m_clientSocket->close();

}
void FakeServer::readClientPart()
{
    if (m_clientSocket->canReadLine()) {
        QStringList token = QString(m_clientSocket->readAll()).split(QRegExp("[ \r\n][ \r\n]*"));
        FakeServer::Request request;
        request.type = token.empty() ? QString("") : token[0];
        request.agent = token.size() < 4 ? QString("") : token[4];
        request.value = token.size() < 17 ? QString("") : token[17];
        m_request << request;
    }
}
