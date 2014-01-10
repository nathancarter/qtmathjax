
#include "texengine.h"
#include <QCoreApplication>
#include <QWebElement>
#include <QDir>
#include <QDebug>

TeXEngine::TeXEngine ()
{
    view = new QWebView( NULL );
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
    TeXcode = TeXcode.replace( "'", "\\'" ).replace( "\n", "\\\n" )
                     .replace( "\\", "\\\\" );
    running = true;
//    qDebug() << "running" << running;
    QVariant result =
            frame->evaluateJavaScript( "UpdateMath( '" + TeXcode + "' );" );
    if ( result.toString() != "started" )
        return QString();
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
        if ( defre.indexIn( piece ) > -1 )
            defs += defre.cap();
        else if ( svgre.indexIn( piece ) > -1 )
            toLoad = QString( "<svg%1>%2%3</svg>" ).arg( svgre.cap( 1 ) )
                                                   .arg( defs )
                                                   .arg( svgre.cap( 2 ) );
    }
//    qDebug() << "returning" << toLoad;
    return toLoad;
}

void TeXEngine::MathJaxDone ()
{
    running = false;
//    qDebug() << "running" << running;
}

void TeXEngine::addJSObject ()
{
    frame->addToJavaScriptWindowObject( "qtapp", this );
}

