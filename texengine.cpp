
#include "texengine.h"
#include <QCoreApplication>
#include <QWebElement>
#include <QDir>
#include <QDebug>

TeXEngine::TeXEngine ()
    : running( false ), isReady( false )
{
    view = new QWebView( NULL );
    connect( view, SIGNAL(loadFinished(bool)), this, SLOT(ready(bool)) );
    QString toLoad = "qrc:/main.html";
//    qDebug() << toLoad;
    view->load( QUrl( toLoad ) );
    frame = view->page()->mainFrame();
    connect( frame, SIGNAL(javaScriptWindowObjectCleared()),
             this, SLOT(addJSObject()) );
}

QWebView* TeXEngine::webView ()
{
    return view;
}

QString TeXEngine::TeX2SVG ( QString TeXcode )
{
    while ( !isReady ) {
        if ( !lastError.isEmpty() )
            return "\\mbox{MathJax error}";
        QCoreApplication::processEvents( QEventLoop::AllEvents, 20 );
    }
    TeXcode = TeXcode.replace( "\\", "\\\\" ).replace( "'", "\\'" )
                     .replace( "\n", "\\\n" );
    running = true;
//    qDebug() << "lastError = QString()";
    lastError = QString();
//    qDebug() << "running" << running;
    QString toRun = QString( "var TeX2SVG_result = null;"
                             "try {"
                             "    TeX2SVG_result = UpdateMath( '%1' );"
                             "} catch ( e ) {"
                             "    TeX2SVG_result = e + '';"
                             "}"
                             "TeX2SVG_result" ).arg( TeXcode );
    QVariant result = frame->evaluateJavaScript( toRun );
    if ( result.toString() != "started" ) {
        lastError = result.toString();
        return QString();
    }
//    qDebug() << "\t" << TeXcode;
    while ( running ) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 20 );
//        qDebug() << "processed events...";
    }
    QWebElementCollection es = frame->findAllElements( "svg" );
    QString toLoad;
    QString defs;
    QRegExp svgre( "<svg([^>]*)>(.*)</svg>" );
    QRegExp defre( "<defs.*defs>" );
    foreach ( QWebElement e, es ) {
        QString piece = e.parent().toInnerXml();
        if ( defre.indexIn( piece ) > -1 ) {
            defs += defre.cap();
        } else if ( svgre.indexIn( piece ) > -1 ) {
            toLoad = QString( "<svg%1>%2%3</svg>" ).arg( svgre.cap( 1 ) )
                                                   .arg( defs )
                                                   .arg( svgre.cap( 2 ) );
        }
    }
//    qDebug() << "returning" << toLoad;
    return toLoad;
}

void TeXEngine::MathJaxDone ()
{
    running = false;
//    qDebug() << "running" << running;
}

void TeXEngine::MathJaxError ( QString errorMessage )
{
//    qDebug() << "lastError =" << errorMessage;
    lastError = errorMessage;
}

QString TeXEngine::error ()
{
    return lastError;
}

void TeXEngine::addJSObject ()
{
    frame->addToJavaScriptWindowObject( "qtapp", this );
}

void TeXEngine::ready ( bool loadSucceeded )
{
    if ( loadSucceeded ) {
        isReady = true;
    } else {
        lastError = "Failed to load MathJax engine";
    }
}

