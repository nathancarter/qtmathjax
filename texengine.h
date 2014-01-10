
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
    Q_INVOKABLE void MathJaxDone ();
    QWebView* webView ();
private:
    QWebView* view;
    QWebFrame* frame;
    bool running;
private slots:
    void addJSObject ();
};


#endif // TEXENGINE_H

