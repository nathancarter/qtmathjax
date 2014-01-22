
#ifndef TEXENGINE_H
#define TEXENGINE_H


#include <QWebView>
#include <QWebFrame>


class TeXEngine : QObject
{
    Q_OBJECT
public:
    TeXEngine ();
    QString TeX2SVG ( QString TeXcode );
    QString error ();
    Q_INVOKABLE void MathJaxDone ();
    Q_INVOKABLE void MathJaxError ( QString errorMessage );
    QWebView* webView ();
private:
    QWebView* view;
    QWebFrame* frame;
    bool running;
    QString lastError;
private slots:
    void addJSObject ();
};


#endif // TEXENGINE_H

