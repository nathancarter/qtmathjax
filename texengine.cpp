

#include "texengine.h"
#include <QCoreApplication>
#include <QWebElement>
#include <QDir>


TeXEngine::TeXEngine ()
    : running( false ), isReady( false )
{
    view = new QWebView( NULL );
    connect( view, SIGNAL(loadFinished(bool)), this, SLOT(ready(bool)) );
    QString toLoad = "qrc:/main.html";
    view->load( QUrl( toLoad ) );
    frame = view->page()->mainFrame();
    connect( frame, SIGNAL(javaScriptWindowObjectCleared()),
             this, SLOT(addJSObject()) );
    cache.setMaxCost( 10000000 );
}

QWebView* TeXEngine::webView ()
{
    return view;
}

bool TeXEngine::hasComputed ( QString TeXcode )
{
    return cache.contains( TeXcode );
}

QString TeXEngine::TeX2SVG ( QString TeXcode )
{
    if ( hasComputed( TeXcode ) ) {
        QString *ptr = cache[TeXcode];
        return ptr ? QString( ptr->constData() ) : QString();
    }
    while ( !isReady ) {
        if ( !lastError.isEmpty() ) {
            computeNextInBackground();
            return QString();
        }
        QCoreApplication::processEvents( QEventLoop::AllEvents, 20 );
    }
    currentInput = TeXcode;
    TeXcode = TeXcode.replace( "\\", "\\\\" ).replace( "'", "\\'" )
                     .replace( "\n", "\\\n" );
    running = true;
    lastError = QString();
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
        computeNextInBackground();
        return QString();
    }
    while ( running ) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 20 );
    }
    computeNextInBackground();
    QString *ptr = cache[TeXcode];
    return ptr ? QString( ptr->constData() ) : QString();
}

void TeXEngine::asyncTeX2SVG ( QString TeXcode )
{
    if ( hasComputed( TeXcode ) || queue.contains( TeXcode ) )
        return;
    queue << TeXcode;
    computeNextInBackground();
}

void TeXEngine::computeNextInBackground ()
{
    if ( queue.isEmpty() || running )
        return;
    currentInput = queue.takeLast();
    QString TeXcode = currentInput;
    TeXcode = TeXcode.replace( "\\", "\\\\" ).replace( "'", "\\'" )
                     .replace( "\n", "\\\n" );
    running = true;
    lastError = QString();
    QString toRun = QString( "var TeX2SVG_result = null;"
                             "try {"
                             "    TeX2SVG_result = UpdateMath( '%1' );"
                             "} catch ( e ) {"
                             "    TeX2SVG_result = e + '';"
                             "}"
                             "TeX2SVG_result" ).arg( TeXcode );
    QVariant result = frame->evaluateJavaScript( toRun );
    if ( result.toString() != "started" )
        lastError = result.toString();
}

void TeXEngine::MathJaxDone ()
{
    running = false;
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
            QString result = QString( "<svg%1>%2%3</svg>" ).arg( svgre.cap( 1 ) )
                                                           .arg( defs )
                                                           .arg( svgre.cap( 2 ) );
            cache.insert( currentInput, new QString( result ), result.count() );
        }
    }
}

void TeXEngine::MathJaxError ( QString errorMessage )
{
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
    computeNextInBackground();
}

