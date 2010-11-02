
#include <QtGui>
#include <QtNetwork>
#include <iostream>
#include <QDebug>

#include "server.h"

Server::Server(QObject* parent)
:  QThread( parent )
{

    moveToThread(this);

}
Server::~Server()
{
  quit();
  wait();
}
void Server::startAndWait()
{
  start();
  // this will block until the event queue starts
  QMetaObject::invokeMethod( this, "started", Qt::BlockingQueuedConnection );
}

void Server::newConnection()
{
    QMutexLocker locker(&m_mutex);
    m_clientSocket = m_tcpServer->nextPendingConnection();
    connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    this->writeServerPart();
}
void Server::dataAvailable()
{
    QMutexLocker locker(&m_mutex);
    readClientPart();
    writeServerPart();
}
void Server::run()
{
    m_tcpServer = new QTcpServer();
    if ( !m_tcpServer->listen( QHostAddress( QHostAddress::LocalHost ), 12566 ) ) {

    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    exec();

    delete m_clientSocket;

    delete m_tcpServer;
}

void Server::started()
{
  // do nothing: this is a dummy slot used by startAndWait()
}

void Server::setScenario( const QString &scenario )
{
    QMutexLocker locker(&m_mutex);

    m_scenarios.clear();
    m_scenarios << scenario;
}

void Server::addScenario( const QString &scenario )
{
    QMutexLocker locker(&m_mutex);

    m_scenarios << scenario;


}

void Server::addScenarioFromFile( const QString &fileName )
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

bool Server::isScenarioDone( int scenarioNumber ) const
{
  QMutexLocker locker(&m_mutex);

  if ( scenarioNumber < m_scenarios.size() ) {
    return m_scenarios[scenarioNumber].isEmpty();
  } else {
    return true; // Non existent hence empty, right?
  }
}

bool Server::isAllScenarioDone() const
{
  QMutexLocker locker( &m_mutex );

  foreach ( const QString &scenario, m_scenarios ) {
    if ( !scenario.isEmpty() ) {
      return false;
    }
  }
  return true;
}
void Server::writeServerPart()
{

    QString retour = m_scenarios.isEmpty() ? QString("vide") : m_scenarios.takeFirst();
    QString scenario = "HTTP/1.0 200 Ok\r\nContent-Type: text/html; charset=\"utf-8\"\r\nSet-Cookie: name=value\r\n\r\n" + retour;
    m_clientSocket->write( scenario.toLocal8Bit() );
    //clientSocket->waitForDisconnected();
    m_clientSocket->close();

}
void Server::readClientPart()
{
    char data[512] = {"\0"};
    m_clientSocket->read(data,512);
    m_scenarios << data;
}
